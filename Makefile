# A simple Makefile for SDL projects

# set the compiler
CC = g++

# set the compiler flags
CFLAGS = `sdl2-config --libs --cflags` -std=c++14 -ggdb3 -O0 -Wall -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lm


# add header files here
HDRS = $(wildcard *.h) #GameObject.h Sprite.h GameEngine.h Player.h PushableObj.h Particle.h ParticleEmitter.h

# add source files here
SRCS = $(wildcard *.cpp)

# generate names of object files
OBJS = $(SRCS:.cpp=.o) #$(SRCS:.cpp=.o)

# name of executable
EXEC = astrorun

# default recipe
all: $(EXEC)
 
#showfont: showfont.c Makefile
#    $(CC) -o $@ $@.c $(CFLAGS) $(LIBS)

#glfont: glfont.c Makefile
#    $(CC) -o $@ $@.c $(CFLAGS) $(LIBS)

# recipe for building the final executable
$(EXEC): $(OBJS) $(HDRS) Makefile
	$(CC) -o $@ $(OBJS) $(CFLAGS)

# recipe for building object files
$(OBJS): $(@:.o=.cpp) $(HDRS) Makefile
	$(CC) -o $@ $(@:.o=.cpp) -c $(CFLAGS)

# recipe to clean the workspace
clean:
	rm -f $(EXEC) 
	rm -f $(OBJS)

run:
	./$(EXEC)