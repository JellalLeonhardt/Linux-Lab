#include<stdio.h>
#include "common.h"

char buffer[1000];

int main(int argv,char* args[]){
	GetID();
    int ID = atoi(args[1]);
    char *FileName = args[2];
    printf("生产者%d开始 %s\n", ID, FileName);
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
