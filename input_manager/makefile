.PHONY: input_manager.o stuff.o

input_manager.o: data_structures/dqueue.o input.o stuff.o
	ld -r data_structures/dqueue.o input.o stuff.o -o input_manager.o

stuff.o: data_structures/dqueue.o input.o
	gcc -c stuff.c

input.o:
	gcc -c input.c -lncurses

data_structures/dqueue.o:
	gcc -c data_structures/dqueue.c -o data_structures/dqueue.o

clean:
	rm data_structures/*.o
	rm *.o
