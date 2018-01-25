#include "pipe_networking.h"
#include "networking.h"
#include "control.h"

void process(char *s);
void subserver(int client_socket, int mess_sem, union messages mess, int to_handler);
void print_servers();
void broadcast(union sub_fds *fds, union messages mess, int mess_sem);

int sighandler(){
  return 0;
}

int main() {
  int mess_sem = create_sem();
  int mess_shm = create_shm();
  union messages mess;
  shmat(mess_shm, &mess, 0);
  mess.kill = 0;
  mess.ready = 0;


  int f = fork();

  if (f == 0){
    union sub_fds *fds = (union sub_fds*)calloc(10, sizeof(union sub_fds));
    int free_spot = 0;
    int from_sub;
    int to_sub;
    int status;
    
    //connection handler
    while (1){
      from_sub = pserver_setup();
      to_sub = pserver_connect(from_sub);
	
      fds[free_spot].from_sub = from_sub;
      fds[free_spot].to_sub = to_sub;
      free_spot++;

      f = fork();

      if (f != 0){
	down_sem(mess_sem);
	mess.kill = 1;
	up_sem(mess_sem);
	wait(&status);
      }
	
      //broadcast handler
      else{
	broadcast(fds, mess, mess_sem);
      }
    }
  
  }
  
  else{
    
    //main server
    int listen_socket;
    listen_socket = server_setup();

    //int client_socket = server_connect(listen_socket);
    printf("Select a server\n");
    print_servers();
    
  
    while (1) {

      int client_socket = server_connect(listen_socket);
      
      f = fork();
    
      //client server
      if (f == 0){
	int to_handler;
	int from_handler = pclient_handshake(&to_handler);

	f = fork();

	//client interaction
	if (f == 0){
	  subserver(client_socket, mess_sem, mess, to_handler);
	}

	//broadcast interaction
	else{
	  while(1){
	    char *buffer = (char *) calloc(256, sizeof(char));
	    read(from_handler, buffer, 256);
	    write(client_socket, buffer, 256);
	  }
	}
      }
      else{
        close(client_socket);
      }
    }
  }
}

void broadcast(union sub_fds *fds, union messages mess, int mess_sem){
  int i;
  char *buffer = (char *)calloc(256, sizeof(char));
  while(1){
    while (mess.ready == 0){
      if (kill > 0)
	exit(0);
    }

    down_sem(mess_sem);
  
    for(i = 0; i < 10; i++){
      read(fds[i].from_sub, buffer, 256);
      write(fds[i].to_sub, buffer, 256);
    }

    mess.ready = 0;
    up_sem(mess_sem);
  }
}

void print_servers(){
  printf("only one server rn\n");
}

void subserver(int client_socket, int mess_sem, union messages mess, int to_handler) {
  char buffer[BUFFER_SIZE];

  while (read(client_socket, buffer, sizeof(buffer))) {

    printf("[subserver %d] received: [%s]\n", getpid(), buffer);
    down_sem(mess_sem);
    write(to_handler, buffer, sizeof(buffer));
    mess.ready++;
    up_sem(mess_sem);
    
  }//end read loop
  close(client_socket);
  exit(0);
}

