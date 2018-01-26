#include "networking.h"
#include "game.c"
#include "chat_interface.h"

int main(int argc, char **argv) {

	int server_socket;
	char write_buffer[256];
	char read_buffer[256];

	if (argc == 2)
		server_socket = client_setup( argv[1]);
	else
		server_socket = client_setup( CS16 );

  int f;

  if (f == 0){
    read(server_socket, read_buffer, sizeof(read_buffer));
    printf("%s", read_buffer);
  }
  set_display_pipe(server_socket);
  while (1) {
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
    
    char *first, *second, *remainder, *context;
    int buffer_length = strlen(write_buffer);
    char *write_buffer_copy = (char*) calloc(buffer_length + 1, sizeof(char));
    strncpy(write_buffer_copy, write_buffer, buffer_length);
    first = strtok_r (write_buffer_copy, " ", &context);
    second = strtok_r (NULL, "\n", &context);
    printf("%s\n", write_buffer);
    if(!strcmp(write_buffer, "!react\n")){
      react();
      endwin();
    }
    else if(!strcmp(first, "!hangman")){
      hangman(second);
      endwin();
    }
    
    write(server_socket, write_buffer, sizeof(write_buffer));
    
  }
	

}
