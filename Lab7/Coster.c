#include<stdio.h>
#include "common.h"

char buffer[1000];

int main(int argv,char* args[]){
	GetID();
    int CreatorNum = atoi(args[1]);
    // SemID = 2719744;
    // ShmID = 219009024;
    // int CreatorNum = 0;
    ShmAddr = shmat(ShmID,NULL,0);
    int i = 0;
    i = semctl(SemID,1,GETVAL,0);
    printf("消费者开始 CreatorNum:%d\n", CreatorNum);
    char *FileName = "All.txt";
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
        if(Semaphore_P(0) != -1){
        	if(Semaphore_P(1) != -1){
        		i = semctl(SemID,1,GETVAL,0);
        		p = ShmAddr->buf[i];
        		cnt = ShmAddr->num[i];
        		fwrite(p,1,cnt,fp);
        		FinishedCnt++;
        		printf("Finishedcnt:%d\n",FinishedCnt);
        		if(FinishedCnt == CreatorNum){
        		    fclose(fp);
        		    printf("消费者结束\n");
        		    Semaphore_P(2);
        		    return 1;
        		}
		}
        Semaphore_V(0);
	}
    }
    return 0;
}
