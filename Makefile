Flags = -std=c++17 -g -Wall -Wextra -pedantic -Wno-unused-parameter -Wno-unused-variable -O2

CXXFLAGS = $(Flags) $(shell pkg-config --cflags sdl2)
LXXFLAGS = $(shell pkg-config --libs sdl2 SDL2_ttf)

Include = include
Src = src
Bin = bin

Cpp = $(notdir $(wildcard $(Src)/*.cpp))
Headers = $(Include)/List.h
Objects = $(addprefix $(Bin)/, $(Cpp:.cpp=.o))

out: $(Objects)
	g++ -o out $(Objects) $(LXXFLAGS)

vpath %.cpp $(Src)
$(Bin)/%.o: %.cpp $(Headers) Makefile
	g++ -c $< $(CXXFLAGS) -o $@

# ./out 1 is also possible
run: out
	./out 0