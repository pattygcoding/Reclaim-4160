#pragma once

#include <string>
#include <string_view>

namespace reclaim::paths {

// All runtime assets live under a single root folder. On desktop the build
// places `assets/` next to the executable; under Emscripten the folder is
// preloaded into the virtual filesystem at the same relative path, so this
// prefix works everywhere.
inline constexpr std::string_view kAssetRoot = "assets/";

[[nodiscard]] std::string image(std::string_view name);   // assets/images/<name>.png
[[nodiscard]] std::string sound(std::string_view name);   // assets/sounds/<name>.wav
[[nodiscard]] std::string font(std::string_view name);    // assets/fonts/<name>.ttf

}  // namespace reclaim::paths
