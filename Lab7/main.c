#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<sys/shm.h>
#include<stdlib.h>
#include<errno.h>
#include<sys/wait.h>
#include<string.h>
//全局变量
typedef struct BufferArea{
    int num[5];
    char buf[5][100];
}AREA;
union semun{
    short val;
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

void SemInit(){
    SemID = semget(SemKey,3,IPC_CREAT|IPC_EXCL);
    if(SemID == -1){
        printf("创建信号量失败\n");
        exit(0);
    }
    printf("SemID:%d\n",SemID);
    SopsP.sem_num = 0;
    SopsP.sem_op = -1;
    SopsP.sem_flg = SEM_UNDO;
    SopsV.sem_num = 0;
    SopsV.sem_op = 1;
    SopsV.sem_flg = SEM_UNDO;
    sem_union.val = 1;
    semctl(SemID,0,SETVAL,sem_union);
    semctl(SemID,2,SETVAL,sem_union);
    perror("semctl");
}
void ShmInit(){
    ShmID = shmget(ShmKey,sizeof(struct BufferArea),IPC_CREAT);
    if(ShmID == -1){
        printf("创建共享内存失败\n");
        exit(0);
    }
    printf("ShmID:%d\n",ShmID);
    ShmAddr = shmat(ShmID,NULL,0);
    if(ShmAddr == -1){
        printf("共享内存连接失败\n");
        semctl(SemID,0,IPC_RMID,sem_union); //删除信号量
        exit(0);
    }
    printf("ShmAddr:%d\n",ShmAddr);
}
void Semaphore_P(int num){
    SopsP.sem_num = num;
    if(semop(SemID,&SopsP,1) == -1){
        printf("P操作失败.\n");
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
        semctl(SemID,0,IPC_RMID,sem_union);
        shmdt(ShmAddr);
        shmctl(ShmID,IPC_RMID,0);
        exit(0);
    }
}

int main(int args, char *argv[]){
    SemInit();
    ShmInit();                                
    int CosterNum,i;
    char c = 0;
    char number_SemID[50];
    char number_ShmID[50];
    char number_i[50];
    char number_CreatorNum[50];
    int CreatorNum = args - 1;
    sem_union.val = 0;
    semctl(SemID,1,SETVAL,sem_union);
    int MainPid = getpid();
    printf("消费者数量：%d\n",CreatorNum);
    pid_t pid;
    char *arg[4];
    sprintf(number_SemID,"%d",SemID);
    sprintf(number_ShmID,"%d",ShmID);
    arg[0] = number_SemID;
    arg[1] = number_ShmID;
    printf("开始创建生产者和消费者\n");
    Semaphore_P(2);
    for(i = 0;i < CreatorNum;i++){
        sprintf(number_i,"%d",i);
        arg[2] = number_i;
        arg[3] = argv[i+1];
        pid = fork();
        if(pid == 0){
            execl("/home/jellal/Linux实验/实验七/Creator","Creator",arg[0],arg[1],arg[2],arg[3],(char *)0);
            perror( "execl" );
            printf("test%d\n",getpid());
            return 0;
        }
    }
    sprintf(number_CreatorNum,"%d",CreatorNum);
    arg[2] = number_CreatorNum;
    pid = fork();
    if(pid == 0){
        execl("/home/jellal/Linux实验/实验七/Coster","Coster",arg[0],arg[1],arg[2],NULL);
        perror( "execl" );
        printf("test%d\n",getpid());
        return 0;
    }
    pid = getpid();
    if(pid == MainPid){
        printf("ASS\n");
        while(c != 'q') c = getchar();
        Semaphore_V(2);
        semctl(SemID,0,IPC_RMID,sem_union);
        shmdt(ShmAddr);
        shmctl(ShmID,IPC_RMID,0);
        return 1;
    }
    return 0;
}
