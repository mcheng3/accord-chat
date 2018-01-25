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
#define SHM_KEY 13377
#define SEM_KEY 22446

union semun {
    int                 val;
    struct semid_ds *   buf;
    unsigned short *    array;
    struct seminfo *    __buf;
};

union sub_fds{
  int from_sub;
  int to_sub;
};

union messages{
  int kill;
  int ready;
};

int create_sem();
int create_shm();
int down_sem(int);
int up_sem(int sem_id);
int remove_sem(int sem_id);
int remove_shm(int sem_id);
