.PHONY: chat_interface_test
	

accord: client server

server: forking_server.o networking.o control.o pipe_networking.o
	gcc -o accord-server forking_server.o networking.o control.o pipe_networking.o -lncurses

client: networking.o chat_interface.c chat_interface.h dqueue.o
	gcc -o accord-client networking.o chat_interface.c dqueue.o -lncurses

client.o: client.c networking.h
	gcc -c client.c

forking_server.o: forking_server.c networking.h control.h pipe_networking.h
	gcc -c forking_server.c

networking.o: networking.c networking.h
	gcc -c networking.c

chat_interface.o: chat_interface.c chat_interface.h dqueue.o
	gcc -c chat_interface.c -lncurses

chat_interface_test: chat_interface.c chat_interface.h dqueue.o
	gcc -o chat_interface_test chat_interface.c dqueue.o -lncurses
	./chat_interface_test
control.o: control.c control.h
	gcc -c control.c

dqueue.o: dqueue.c dqueue.h
	gcc -c dqueue.c

pipe_networking.o: pipe_networking.c pipe_networking.h
	gcc -c pipe_networking.c 
clean:
	rm *.o
	killall accord-server
