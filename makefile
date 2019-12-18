ifdef MEM
	DATA = -g
	VG = valgrind --leak-check=full
endif

all: smash.o parse.o execute.o
	gcc $(DATA) -o smash smash.o parse.o execute.o

smash.o: smash.c
	gcc $(DATA) -c smash.c

parse.o: parse.c parse.h
	gcc $(DATA) -c parse.c

execute.o: execute.c execute.h
	gcc $(DATA) -c execute.c

run:
	$(VG) ./smash $(ARGS)

clean:
	rm *.o
	rm smash
