LIBS = -lraylib -lgdi32 -lwinmm

game: game.cpp
	$(CXX) -static game.cpp $(LIBS) -o $@ 
