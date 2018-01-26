#include "control.h"

int create_sem(){
  int sem_id = semget(SEM_KEY, 0, IPC_CREAT | 0666);
  
  union semun arg;
  arg.val = 1;
    
  semctl(sem_id, 0, SETVAL, arg);
  return sem_id;
}

int create_shm(){
  int shm_id = shmget(SHM_KEY, sizeof(struct messages), IPC_CREAT | 0644);
  return shm_id;
}

int down_sem(int sem_id){
  struct sembuf buffer;
  buffer.sem_op = -1;
  buffer.sem_num = 0;
  buffer.sem_flg = SEM_UNDO;
  semop(sem_id, &buffer, 1);
  return 0;
}

int up_sem(int sem_id){
  struct sembuf buffer;
  buffer.sem_op = 1;
  buffer.sem_num = 0;
  buffer.sem_flg = SEM_UNDO;
  semop(sem_id, &buffer, 1);
  return 0;
}

int remove_sem(int sem_id){
  semctl(sem_id, 0, IPC_RMID);
  return 0;
}

int remove_shm(int shm_id){
    return shmctl(shm_id, IPC_RMID, 0);
}

