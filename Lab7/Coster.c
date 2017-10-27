#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<sys/shm.h>
#include<stdlib.h>
#define IPC_R 000400
#define IPC_W 000200
#define IPC_M 010000

typedef struct BufferArea{
    int num[5];
    char buf[5][100];
}AREA;
union semun{
    int val;
    struct semid_ds *buf;
    unsigned short *arry;
};
int SemKey = 0xAE12;
int ShmKey = 0xAE86;
struct sembuf SopsP;
struct sembuf SopsV;
int SemID;
int ShmID;
AREA *ShmAddr = NULL;
union semun sem_union;
char buffer[1000];
void SemInit(){
    SopsP.sem_num = 0;
    SopsP.sem_op = -1;
    SopsP.sem_flg = SEM_UNDO;
    SopsV.sem_num = 0;
    SopsV.sem_op = 1;
    SopsV.sem_flg = SEM_UNDO;
}
void Semaphore_P(int num){
    SopsP.sem_num = num;
    if(semop(SemID,&SopsP,1) == -1){
        exit(0);
    }
}
void Semaphore_V(int num){
    SopsV.sem_num = num;
    if(semop(SemID,&SopsV,1) == -1){
        exit(0);
    }
}

int main(int argv,char* args[]){
    SemID = atoi(args[1]);
    ShmID = atoi(args[2]);
    int CreatorNum = atoi(args[3]);
    // SemID = 2719744;
    // ShmID = 219009024;
    // int CreatorNum = 0;
    ShmAddr = shmat(ShmID,NULL,0);
    int i = 0;
    i = semctl(SemID,1,GETVAL,0);
    printf("消费者开始 semID:%d ShmID:%d CreatorNum:%d\n",SemID,ShmID,CreatorNum);
    char *FileName = "Coster.txt";
    FILE *fp = fopen(FileName,"w+");
    if(fp == 0){
        printf("creat %s falid\n",FileName);
        exit(0);
    } 
    int FinishedCnt = 0;
    char *p = NULL;
    int cnt;
    SemInit();
    while(1){
        Semaphore_P(0);
        Semaphore_P(1);
        i = semctl(SemID,1,GETVAL,0);
        p = ShmAddr->buf[i];
        cnt = ShmAddr->num[i];
        fwrite(p,1,cnt,fp);
        FinishedCnt++;
        printf("Finishedcnt:%d\n",FinishedCnt);
        Semaphore_V(0);
        if(FinishedCnt == CreatorNum){
            fclose(fp);
            printf("消费者结束\n");
            Semaphore_P(2);
            return 1;
        }
    }
    return 0;
}
