
CFLAGS=-g -Wall -std=c2x 

OBJECTS=main.o 

main: $(OBJECTS)
	gcc $(OBJECTS) -o main $(CFLAGS)

main.o: main.c 
	gcc -c main.c $(CFLAGS)

clean: 
	rm -f main $(OBJECTS)
