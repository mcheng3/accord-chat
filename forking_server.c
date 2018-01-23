#include "networking.h"
#include "pipe_networking.h"

void process(char *s);
void subserver(int from_client);
void print_servers();

int main() {
  //main server
  int listen_socket;
  int f;
  listen_socket = server_setup();

  int client_socket = server_connect(listen_socket);
  printf("Select a server\n");
  print_servers();

  f = fork();

  //broadcast server
  if (f == 0){
    int *from_subs = (int *)calloc(10, sizeof(int));
    int *to_subs = (int *)calloc(10, sizeof(int));
    int from_sub;
    int to_sub;
    
    fork();
    
    //connection handler
    if (f == 0){
      while (1){
        from_sub = pserver_setup();
        to_sub = pserver_connect();
      }
    }
  }

  else{
  
    while (1) {

      //client_socket = server_connect(listen_socket);
    
      f = fork();
    
      //client server
      if (f == 0){
      
        subserver(client_socket);
      
      }
      else{
        close(client_socket);
      }
    }
  }
}

void print_servers(){
  printf("only one server rn\n");
}

void subserver(int client_socket) {
  char buffer[BUFFER_SIZE];

  while (read(client_socket, buffer, sizeof(buffer))) {

    printf("[subserver %d] received: [%s]\n", getpid(), buffer);
    process(buffer);
    write(client_socket, buffer, sizeof(buffer));
  }//end read loop
  close(client_socket);
  exit(0);
}

void process(char * s) {
  while (*s) {
    if (*s >= 'a' && *s <= 'z')
      *s = ((*s - 'a') + 13) % 26 + 'a';
    else  if (*s >= 'A' && *s <= 'Z')
      *s = ((*s - 'a') + 13) % 26 + 'a';
    s++;
  }
}
