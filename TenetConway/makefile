
CFLAGS=-g -Wall -std=c++17 

OBJECTS=lib_set.o 

main:$(OBJECTS) main.cpp
	g++ $(OBJECTS) main.cpp -o main $(CFLAGS) -lz3

lib_set.o:lib_set.cpp  
	g++ -c lib_set.cpp $(CFLAGS)

clean: 
	rm -f main $(OBJECTS)
