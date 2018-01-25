.PHONY: chat_interface_test

forking: client fserver

select: sclient sserver

sserver: select_server.o networking.o
	gcc -o server select_server.o networking.o

fserver: forking_server.o networking.o
	gcc -o server forking_server.o networking.o

sclient: select_client.o networking.o
	gcc -o client select_client.o networking.o

client: client.o networking.o
	gcc -o client client.o networking.o

select_client.o: select_client.c networking.h
	gcc -c select_client.c

client.o: client.c networking.h
	gcc -c client.c

select_server.o: select_server.c networking.h
	gcc -c select_server.c

forking_server.o: forking_server.c networking.h
	gcc -c forking_server.c

networking.o: networking.c networking.h
	gcc -c networking.c

chat_interface.o: chat_interface.c chat_interface.h dqueue.o
	gcc -c chat_interface.c -lncurses

chat_interface_test: chat_interface.c chat_interface.h dqueue.o
	gcc -o chat_interface_test chat_interface.c dqueue.o -lncurses
	./chat_interface_test

dqueue.o: dqueue.c dqueue.h
	gcc -c dqueue.c

clean:
	rm *.o
	rm *~
