#include "common.h"

#define SEM_NUM 3

#define SEMJUDGE(x) if(x < 0 || x > SEM_NUM){ \
						exit(0); \
					}

int SEM_VAL[SEM_NUM] = {1, 0, 1};

#define SEM_VAL_INIT int _i; \
					 for(_i = 0;_i < SEM_NUM;_i++){ \
						 sem_union.val = SEM_VAL[_i]; \
						 semctl(SemID, _i, SETVAL, sem_union); \
						 perror("set sem value"); \
					 }

union semun{
	short val;
	struct semid_ds *buf;
	unsigned short *arry;
};

int SemKey = 0xAE12;
int ShmKey = 0xAE86;

struct sembuf SopsP = {
	.sem_op = -1,
	.sem_flg = SEM_UNDO
};

struct sembuf SopsV = {
	.sem_op = 1,
	.sem_flg = SEM_UNDO
};

int SemID;
int ShmID;

AREA *ShmAddr = NULL;

void SemInit(){
    SemID = semget(SemKey,3,IPC_CREAT);
    if(SemID == -1){
        printf("创建信号量失败\n");
        exit(0);
    }
#ifdef DEBUG
    printf("SemID:%d\n",SemID);
#endif
	union semun sem_union;
	SEM_VAL_INIT
}

void ShmInit(){
    ShmID = shmget(ShmKey,sizeof(struct BufferArea),IPC_CREAT);
    if(ShmID == -1){
        printf("创建共享内存失败\n");
        exit(0);
    }
#ifdef DEBUG
    printf("ShmID:%d\n",ShmID);
#endif
    ShmAddr = shmat(ShmID,NULL,0);
    if(ShmAddr == -1){
        printf("共享内存连接失败\n");
		union semun sem_union;
        semctl(SemID,0,IPC_RMID,sem_union); //删除信号量
        exit(0);
    }
    printf("ShmAddr:%d\n",ShmAddr);
}

void GetID(){
    SemID = semget(SemKey,3,IPC_CREAT);
    if(SemID == -1){
        printf("创建信号量失败\n");
        exit(0);
    }
    printf("SemID:%d\n",SemID);

    ShmID = shmget(ShmKey,sizeof(struct BufferArea),IPC_CREAT);
    if(ShmID == -1){
        printf("创建共享内存失败\n");
        exit(0);
    }
#ifdef DEBUG
    printf("ShmID:%d\n",ShmID);
#endif
    ShmAddr = shmat(ShmID,NULL,0);
    if(ShmAddr == -1){
        printf("共享内存连接失败\n");
        exit(0);
    }
#ifdef DEBUG
    printf("ShmAddr:%d\n",ShmAddr);
#endif
}

void Semaphore_P(int num){
    SopsP.sem_num = num;
    if(semop(SemID,&SopsP,1) == -1){
        printf("P操作失败.\n");
		union semun sem_union;
        semctl(SemID,0,IPC_RMID,sem_union);
        shmdt(ShmAddr);
        shmctl(ShmID,IPC_RMID,0);
        exit(0);
    }
    
}
void Semaphore_V(int num){
    SopsV.sem_num = num;
    if(semop(SemID,&SopsV,1) == -1){
        printf("V操作失败.\n");
		union semun sem_union;
        semctl(SemID,0,IPC_RMID,sem_union);
        shmdt(ShmAddr);
        shmctl(ShmID,IPC_RMID,0);
        exit(0);
    }
}
