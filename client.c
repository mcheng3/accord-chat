#include "networking.h"
int main(int argc, char **argv) {

	int server_socket;
	char write_buffer[256];
	char read_buffer[256];

	if (argc == 2)
		server_socket = client_setup( argv[1]);
	else
		server_socket = client_setup( CS16 );


	set_display_pipe(server_socket);
	while(1){
		int retpoll = poll(poll_structs,1,0);
		if(retpoll > 0){
			if(poll_structs[0].revents & POLLIN){
				read(server_socket, read_buffer, sizeof(read_buffer));
				printf("%s", read_buffer);
			}
		}
		printf(":");
		fgets(write_buffer, sizeof(write_buffer), stdin);
		//*strchr(write_buffer, '\n') = 0;
		write(server_socket, write_buffer, sizeof(write_buffer));
	}
}
