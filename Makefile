Gpp = g++
Flags = -g $(shell pkg-config --cflags --libs sdl2 SDL2_ttf)
bin = bin
src = src
Objs = $(bin)/window.o $(bin)/shapes.o $(bin)/app.o \
       $(bin)/render.o $(bin)/CoordinateSystem.o $(bin)/main.o

out: $(Objs)
	$(Gpp) $(Objs) -o out $(Flags)

$(bin)/window.o: $(src)/window.cpp
	$(Gpp) -c $(Flags) $(src)/window.cpp -o $(bin)/window.o

$(bin)/render.o: $(src)/render.cpp
	$(Gpp) -c $(Flags) $(src)/render.cpp -o $(bin)/render.o

$(bin)/app.o: $(src)/app.cpp
	$(Gpp) -c $(Flags) $(src)/app.cpp -o $(bin)/app.o

$(bin)/shapes.o: $(src)/shapes.cpp
	$(Gpp) -c $(Flags) $(src)/shapes.cpp -o $(bin)/shapes.o

$(bin)/CoordinateSystem.o: $(src)/CoordinateSystem.cpp
	$(Gpp) -c $(Flags) $(src)/CoordinateSystem.cpp -o $(bin)/CoordinateSystem.o

$(bin)/main.o: $(src)/main.cpp
	$(Gpp) -c $(Flags) $(src)/main.cpp -o $(bin)/main.o