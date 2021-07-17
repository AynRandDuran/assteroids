LIBS = -lraylib -lgdi32 -lwinmm

game: game.cpp
	$(CXX) $@.cpp $(LIBS) -o $@ 
