#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<sys/shm.h>
#include<stdlib.h>
#include<errno.h>
typedef struct BufferArea{
    int num[5];
    char buf[5][100];
}AREA;
union semun{
    short val;
    struct semid_ds *buf;
    unsigned short *arry;
};
int ID;
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
    ShmAddr = shmat(ShmID,NULL,0);
    ID = atoi(args[3]);
    char *FileName = args[4];
    printf("生产者%d开始 %s semID:%d ShmID:%d\n",ID,FileName,SemID,ShmID);
    FILE *fp = fopen(FileName,"r");
    int cnt = fread(buffer,1,100,fp); 
    if(cnt <= 0){
        printf("read %s falid\n",FileName);
        exit(0);
    } 
    int i = 0;
    char *p = NULL;
    char *r = buffer;
    SemInit();
    Semaphore_P(0);
    i = semctl(SemID,1,GETVAL,0);
    Semaphore_V(1);
    p = ShmAddr->buf[i];
    ShmAddr->num[i] = cnt;
    for(i = 0;i < cnt;i++){
        *(p++) = *(r++); 
    }
    for(i = cnt;i < 100;i++){
        *(p++) = 0; 
    }
    Semaphore_V(0);
    printf("生产者%d结束\n",ID);
    fclose(fp);
    Semaphore_P(2);
    return 1;
}
