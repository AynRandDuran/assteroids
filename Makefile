LIBS = -lraylib
WINLIBS = -lraylib -lgdi32 -lwinmm

game: game.cpp
	$(CXX) -static game.cpp $(LIBS) -o assteroids

win_game: game.cpp
	$(CXX) -static game.cpp $(WINLIBS) -o assteroids.exe
