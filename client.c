#include "networking.h"
#include "game.c"

int main(int argc, char **argv) {

  int server_socket;
  char write_buffer[256];
  char read_buffer[256];

  if (argc == 2)
    server_socket = client_setup( argv[1]);
  else
    server_socket = client_setup( CS16 );

  int f = fork();

  if (f == 0){
    read(server_socket, read_buffer, sizeof(read_buffer));
    printf("%s", read_buffer);
  }
  while (1) {
    printf(":");
    fgets(write_buffer, sizeof(write_buffer), stdin);
    //*strchr(write_buffer, '\n') = 0;
    char *first, *second, *remainder, *context;
    int buffer_length = strlen(write_buffer);
    char *write_buffer_copy = (char*) calloc(buffer_length + 1, sizeof(char));
    strncpy(write_buffer_copy, write_buffer, buffer_length);
    first = strtok_r (write_buffer_copy, " ", &context);
    second = strtok_r (NULL, "\n", &context);
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
