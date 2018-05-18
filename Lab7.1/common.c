#include "common.h"

#define SEM_NUM 3

int SEM_VAL[SEM_NUM] = {1, 0, 0};//0:内存池 1:写入内存池完成标志位 2:内存池读完标志位

#define SEM_VAL_INIT int _i; \
					 union semun sem_union; \
					 for(_i = 0;_i < SEM_NUM;_i++){ \
						 sem_union.val = SEM_VAL[_i]; \
						 printf("初始化信号量%d为%d\n", _i, SEM_VAL[_i]); \
						 if(semctl(SemID, _i, SETVAL, sem_union) < 0){ \
							 perror("set sem value"); \
							 semctl(SemID, 0, IPC_RMID, sem_union); \
							 exit(0); \
						 } \
					 }

#define SEMJUDGE(x) if(x < 0 || x > SEM_NUM){ \
						exit(0); \
					}

int SemKey = 0xAE12;
int ShmKey = 0xAE86;

struct sembuf SopsP = {
	.sem_op = -1,
#ifdef MAIN
	.sem_flg = SEM_UNDO
#else
	.sem_flg = 0
#endif
};

struct sembuf SopsV = {
	.sem_op = 1,
#ifdef MAIN
	.sem_flg = SEM_UNDO
#else
	.sem_flg = 0
#endif
};

int SemID;
int ShmID;

AREA *ShmAddr = NULL;

void SemInit(){
    SemID = semget(SemKey, SEM_NUM, IPC_CREAT);
    if(SemID == -1){
        printf("创建信号量失败\n");
        exit(0);
    }
#ifdef DEBUG
    printf("SemID:%d\n",SemID);
#endif
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
#ifdef DEBUG
    printf("ShmAddr:%d\n",ShmAddr);
#endif
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

void SemaphoreP(int num){
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
void SemaphoreV(int num){
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
