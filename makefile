ifdef MEM
	DATA = -g
	VG = valgrind --leak-check=full
endif

all: smash.o
	gcc $(DATA) -o smash smash.o

smash.o: smash.c
	gcc $(DATA) -c smash.c

run:
	$(VG) ./smash $(ARGS)

clean:
	rm *.o
	rm smash