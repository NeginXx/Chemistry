Flags = -g #-Wall -Wextra -pedantic

CXXFLAGS = $(Flags) $(shell pkg-config --cflags sdl2) -O2
LXXFLAGS = $(shell pkg-config --libs sdl2 SDL2_ttf)

Include = include
Src = src
Bin = bin

Cpp = $(notdir $(wildcard $(Src)/*.cpp))
Headers = $(Include)/list.h
Objects = $(addprefix $(Bin)/, $(Cpp:.cpp=.o))

out: $(Objects)
	g++ -o out $(Objects) $(LXXFLAGS)

vpath %.cpp $(Src)
$(Bin)/%.o: %.cpp $(Headers) Makefile
	g++ -c $< $(CXXFLAGS) -o $@