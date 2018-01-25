#include "networking.h"

int main(int argc, char **argv) {

  int server_socket;
  char buffer[256];

  if (argc == 2)
    server_socket = client_setup( argv[1]);
  else
    server_socket = client_setup( CS16 );

  while (1) {
    printf(":");
    fgets(buffer, sizeof(buffer), stdin);
    *strchr(buffer, '\n') = 0;
    write(server_socket, buffer, sizeof(buffer));
  }
}
