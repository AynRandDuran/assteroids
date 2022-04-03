LIBS = -lraylib
WINLIBS = -lgdi32 -lwinmm

# build in linux for linux
game: game.cpp
	$(CXX) game.cpp $(LIBS) -o assteroids

# build in windows for windows (raylib distros with mingw)
win_game: game.cpp
	$(CXX) game.cpp $(LIBS) $(WINLIBS) -o assteroids.exe
