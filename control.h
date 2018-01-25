#define SHM_KEY 13377
#define SEM_KEY 22446

union semun {
    int                 val;
    struct semid_ds *   buf;
    unsigned short *    array;
    struct seminfo *    __buf;
};

int create_shm(int sem_id);
int remove_shm(int sem_id);
int view_shm(int sem_id);
