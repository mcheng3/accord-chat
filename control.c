#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include "control.h"

int create_shm(int sem_id){
  int shm_id = shmget(SHM_KEY, sizeof(int), IPC_CREAT | 0644);
  int *shmp;
  shmp = (int *)shmat(shm_id, shmp, 0);
  return shm_id;
}


int remove_shm(int sem_id){
  struct sembuf buffer;
  buffer.sem_op = -1;
  buffer.sem_num = 0;
  buffer.sem_flg = SEM_UNDO;
  semop(sem_id, &buffer, 1);
  printf("story:\n");
  int f = fork();
  if(f){
    int s;
    wait(&s);
  }
  else{
    char *story_file[] = {"cat", "./story.txt", 0};
    execvp(story_file[0], story_file);
  }
  semctl(sem_id, 0, IPC_RMID);
  remove("./story.txt");
  printf("story removed: %d\n", semctl(semget(SEM_KEY, 0, 0), 0, IPC_RMID));
  return 0;
}

int view_shm(int sem_id){
  return 0;
}

int main(int argc, char *argv[]){
  if (argc == 1){
    printf("flags missing\n");
  }
  
  if (! strcmp(argv[1], "-c")){
    int sem_id = semget(SEM_KEY, 1, IPC_CREAT | 0644);
    int story_file = open("./story.txt", O_CREAT | O_TRUNC | O_RDWR, 0644);

    union semun arg;
    arg.val = 1;
    
    semctl(sem_id, 0, SETVAL, arg);
    //printf("%d\n", semctl(sem_id, 0, GETVAL));
    create_shm(sem_id);
    printf("story created\n");
  }
  
  if (! strcmp(argv[1], "-r")){
    int sem_id = semget(SEM_KEY, 0, 0644);
    int shm_id = shmget(SHM_KEY, sizeof(int), 0644);
    //if(sem_id == -1 || shm_id == -1){
    //  printf("error\n");
    //}
    //else{
    remove_shm(sem_id);
    shmctl(shm_id, IPC_RMID, 0);
      //}
  }
  else if (! strcmp(argv[1], "-v")){
    printf("story:\n");
    int f = fork();
    if(f){
      int s;
      wait(&s);
    }
    else{
      char *story_file[] = {"cat", "./story.txt", 0};
      execvp(story_file[0], story_file);
    }
  }
}

