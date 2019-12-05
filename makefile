ifdef MEM
	DATA = -g
	VG = valgrind --leak-check=full
endif

all: smash.o
	gcc $(DATA) -o smash smash.o

main.o: smash.c
	gcc $(DATA) -c main.c

run:
	$(VG) ./smash $(ARGS)

clean:
	rm *.o
	rm smash
