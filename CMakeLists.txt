cmake_minimum_required(VERSION 3.20)

project(Reclaim4160 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE AND NOT EMSCRIPTEN)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "" FORCE)
endif()

# ---------------------------------------------------------------------------
#  Sources
# ---------------------------------------------------------------------------
file(GLOB_RECURSE RECLAIM_SOURCES CONFIGURE_DEPENDS
     "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_executable(reclaim ${RECLAIM_SOURCES})
target_include_directories(reclaim PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")

if(MSVC)
    target_compile_options(reclaim PRIVATE /W4 /permissive-)
else()
    target_compile_options(reclaim PRIVATE -Wall -Wextra -Wno-unused-parameter)
endif()

# ---------------------------------------------------------------------------
#  Web (Emscripten / WASM)
# ---------------------------------------------------------------------------
if(EMSCRIPTEN)
    set_target_properties(reclaim PROPERTIES SUFFIX ".html")

    # SDL2 + satellites are provided by Emscripten's port system (-sUSE_SDL*).
    # The flags must be on the compile line too, so the port include dirs exist.
    target_compile_options(reclaim PRIVATE
        "-sUSE_SDL=2"
        "-sUSE_SDL_IMAGE=2"
        "-sUSE_SDL_TTF=2"
        "-sUSE_SDL_MIXER=2"
        "-sSDL2_IMAGE_FORMATS=png"
    )

    target_link_options(reclaim PRIVATE
        "-sUSE_SDL=2"
        "-sUSE_SDL_IMAGE=2"
        "-sUSE_SDL_TTF=2"
        "-sUSE_SDL_MIXER=2"
        "-sSDL2_IMAGE_FORMATS=png"
        "-sALLOW_MEMORY_GROWTH=1"
        "-sEXPORTED_RUNTIME_METHODS=ccall,cwrap"
        "--shell-file" "${CMAKE_CURRENT_SOURCE_DIR}/web/shell.html"
        "--preload-file" "${CMAKE_CURRENT_SOURCE_DIR}/assets@assets"
    )

    message(STATUS "Reclaim-4160: building for WebAssembly via Emscripten")

# ---------------------------------------------------------------------------
#  Native (SDL2 via package config)
# ---------------------------------------------------------------------------
else()
    find_package(SDL2 CONFIG REQUIRED)
    find_package(SDL2_image CONFIG REQUIRED)
    find_package(SDL2_ttf CONFIG REQUIRED)
    find_package(SDL2_mixer CONFIG REQUIRED)

    # The SDL config packages expose ".../include/SDL2" as the include dir.
    # Our sources use <SDL2/SDL.h>, so also add the parent "include" directory.
    foreach(_tgt SDL2::SDL2 SDL2_image::SDL2_image SDL2_ttf::SDL2_ttf SDL2_mixer::SDL2_mixer)
        if(TARGET ${_tgt})
            get_target_property(_incs ${_tgt} INTERFACE_INCLUDE_DIRECTORIES)
            if(_incs)
                foreach(_inc ${_incs})
                    get_filename_component(_parent "${_inc}" DIRECTORY)
                    target_include_directories(reclaim PRIVATE "${_parent}")
                endforeach()
            endif()
        endif()
    endforeach()

    # We provide our own main(), so skip SDL's main shim.
    target_compile_definitions(reclaim PRIVATE SDL_MAIN_HANDLED)

    target_link_libraries(reclaim PRIVATE
        SDL2::SDL2
        SDL2_image::SDL2_image
        SDL2_ttf::SDL2_ttf
        SDL2_mixer::SDL2_mixer
    )

    # Copy assets next to the executable so relative "assets/..." paths work.
    add_custom_command(TARGET reclaim POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${CMAKE_CURRENT_SOURCE_DIR}/assets"
                "$<TARGET_FILE_DIR:reclaim>/assets"
        COMMENT "Copying assets next to the executable")

    # On Windows, stage the SDL runtime DLLs next to the exe if they are known.
    if(WIN32)
        foreach(_sdl_target SDL2::SDL2 SDL2_image::SDL2_image SDL2_ttf::SDL2_ttf SDL2_mixer::SDL2_mixer)
            if(TARGET ${_sdl_target})
                get_target_property(_loc ${_sdl_target} IMPORTED_LOCATION)
                if(_loc)
                    get_filename_component(_dir "${_loc}" DIRECTORY)
                    add_custom_command(TARGET reclaim POST_BUILD
                        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                                "${_loc}" "$<TARGET_FILE_DIR:reclaim>")
                endif()
            endif()
        endforeach()
    endif()
endif()
