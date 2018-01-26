#include "pipe_networking.h"
#include "networking.h"
#include "control.h"


void process(char *s);
void subserver(int client_socket, int mess_sem, int mess_shm, int to_handler);
void print_servers();
void broadcast(struct sub_fds *fds, int mess_shm, int mess_sem, int free_spot);

int sighandler(){
  printf("Use exit\n");
  return 0;
}

int main() {
  int mess_sem = create_sem();
  int mess_shm = create_shm();

  printf("%d\n", mess_shm);

  int f = fork();

  if (f == 0){
    struct sub_fds *fds = (struct sub_fds*)calloc(10, sizeof(struct sub_fds));
    int free_spot = 0;
    int from_sub;
    int to_sub;
    int status;
   
    printf("%d\n", mess_shm); 
    struct messages *mess;
    mess = (struct messages *)shmat(mess_shm, NULL, 0);
    if (mess == (struct messages *) -1)
      printf("%s\n", strerror(errno));
    mess->kill = 0;
    mess->ready = 0;
    
    printf("[connection handler]: messages attached\n");
    
    //connection handler
    while (1){
      from_sub = pserver_setup();
      to_sub = pserver_connect(from_sub);

      //make from_sub not block upon read
      int flags = fcntl(from_sub, F_GETFL, 0);
      fcntl(from_sub, F_SETFL, flags | O_NONBLOCK);
	
      fds[free_spot].from_sub = from_sub;
      fds[free_spot].to_sub = to_sub;
      free_spot++;
      
      printf("mess.ready: %d\n", mess->ready);
      printf("[connection handler]: creating broadcast server...\n");

      printf("mess.ready: %d\n", mess->ready);
	printf("[connection handler]: adding client...\n");
	down_sem(mess_sem);
	mess->kill = 1;
	up_sem(mess_sem);
        printf("mess.ready: %d\n", mess->ready);
	wait(&status);
	printf("[connection handler]: client added\n");

	down_sem(mess_sem);
	mess->kill = 0;
	up_sem(mess_sem);
      f = fork();

      if (f != 0){
      }
	
      //broadcast handler
      else{
        printf("mess.ready: %d\n", mess->ready);
	broadcast(fds, mess_shm, mess_sem, free_spot);
      }
    }
  
  }
  
  else{
    f = fork();

    //serverside interface
    if (f != 0){
      char *buffer = (char *)calloc(6, sizeof(char));
      
      while(strcmp(buffer, "exit\n")){
	fgets(buffer, 6, stdin);
      }
      struct messages *mess;  
      mess = (struct messages *)shmat(mess_shm, NULL, 0);
      mess->ready = 0;
      mess->kill = 1;
      sleep(1);
      mess->kill = 0;
      remove_sem(mess_sem);
      remove_shm(mess_shm);
      remove("luigi");
      //close(client_socket);
      exit(0);
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
        
	sleep(1);
 
	f = fork();
    
	//client server
	if (f == 0){
	  int to_handler;
	  int from_handler = pclient_handshake(&to_handler);

	  f = fork();

	  //client interaction
	  if (f == 0){
	    subserver(client_socket, mess_sem, mess_shm,to_handler);
	    close(client_socket);
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
}

void broadcast(struct sub_fds *fds, int mess_shm, int mess_sem, int free_spot){
  int i;
  int j;
  char *buffer = (char *)calloc(256, sizeof(char));

  printf("broadcast server created\n");
  
  struct messages *mess;
  mess = (struct messages *)shmat(mess_shm, NULL, 0);
  
  printf("mess.ready: %d\n", mess->ready);
  
  while(1){
    while (mess->ready == 0){
      //printf("mess.kill: %d\n", mess->kill);

      if (mess->kill > 0){
	printf("[broadcast server]: client list updated\n");
	exit(0);
      }
    }

    printf("[broadcast server]: messages > 0\n");
    down_sem(mess_sem);
  
    for(i = 0; i < free_spot; i++){
      if (read(fds[i].from_sub, buffer, 256) > -1){
	printf("[broadcast server]: broadcasting %s", buffer);
	for(j = 0; j < free_spot; j++){
	  if (j != i)
	    write(fds[j].to_sub, buffer, 256);
	}
      }
    }

    mess->ready = 0;
    up_sem(mess_sem);
    
    printf("[broadcast server]: message(s) broadcasted\n");
  }
}

void print_servers(){
  printf("only one server rn\n");
}

void subserver(int client_socket, int mess_sem, int mess_shm, int to_handler) {
  char buffer[BUFFER_SIZE];
  struct messages *mess;
  mess = (struct messages *)shmat(mess_shm, NULL, 0);

  while (read(client_socket, buffer, sizeof(buffer))) {
    if(!strcmp(buffer,"!")){
      printf("ALERT\n");
    }
    printf("[clientserver %d] received: [%s]\n", getpid(), buffer);
    down_sem(mess_sem);
    write(to_handler, buffer, sizeof(buffer));
    printf("[clientserver %d] wrote to handler\n", getpid());
    mess->ready += 1;
    up_sem(mess_sem);
    //printf("sem val:%d\n", semget(
    printf("number of pending messages: %d\n", mess->ready);
    
    
  }//end read loop
  close(client_socket);
  exit(0);
}

