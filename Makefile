Gpp = g++
Flags = -g $(shell pkg-config --cflags --libs sdl2 SDL2_ttf)
bin = bin
src = src
Objs = $(bin)/main.o $(bin)/draw.o $(bin)/graph.o

out: $(Objs)
	$(Gpp) $(Objs) -o out $(Flags)

$(bin)/main.o: $(src)/main.cpp
	$(Gpp) -c $(Flags) $(src)/main.cpp -o $(bin)/main.o

$(bin)/draw.o: $(src)/draw.cpp
	$(Gpp) -c $(Flags) $(src)/draw.cpp -o $(bin)/draw.o

$(bin)/graph.o: $(src)/graph.cpp
	$(Gpp) -c $(Flags) $(src)/graph.cpp -o $(bin)/graph.o