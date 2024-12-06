FLAGS = -pedantic-errors -std=c++11

game: AI.o main.o Item.o Player.o
	g++ $(FLAGS) $^ -o $@
	
AI.o: AI.cpp AI.h Player.h Item.h
	g++ $(FLAGS) -c $<

main.o: main.cpp AI.h Player.h Item.h
	g++ $(FLAGS) -c $<

Player.o: Player.cpp Player.h Item.h
	g++ $(FLAGS) -c $<

Item.o: Item.cpp Item.h Player.h
	g++ $(FLAGS) -c $<

clean:
	rm -f game *.o

.PHONY: clean