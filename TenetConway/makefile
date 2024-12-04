
CFLAGS=-g -Wall -std=c++17

OBJECTS=lib_set.o lib_game_of_life_operations.o

main:$(OBJECTS) main.cpp
	g++ $(OBJECTS) main.cpp -o main $(CFLAGS) -lz3

lib_set.o:lib_set.cpp  
	g++ -c lib_set.cpp $(CFLAGS)

lib_game_of_life_operations.o:lib_game_of_life_operations.cpp
	g++ -c lib_game_of_life_operations.cpp $(CFLAGS) -lz3

clean: 
	rm -f main $(OBJECTS)
