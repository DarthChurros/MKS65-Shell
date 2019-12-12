ifdef MEM
	DATA = -g
	VG = valgrind --leak-check=full
endif

all: smash.o parse.o
	gcc $(DATA) -o smash smash.o

smash.o: smash.c
	gcc $(DATA) -c smash.c

parse.o: parse.c parse.h
	gcc $(DATA) -c parse.c

run:
	$(VG) ./smash $(ARGS)

clean:
	rm *.o
	rm smash
