#include "networking.h"

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
    write(server_socket, write_buffer, sizeof(write_buffer));
  }
}
