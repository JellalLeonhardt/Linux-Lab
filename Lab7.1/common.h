#ifndef _COMMON_H
#define _COMMON_H
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
#include<fcntl.h>

#define msleep(x) usleep(x*1000)
typedef struct BufferArea{
	int fd; //文件描述符
	int target_fd;//目标文件文件描述符
	int head;//队首
	int tail;//队尾
	int num[5];
	char buf[5][100];
}AREA;

union semun{
	int val;
	struct semid_ds *buf;
	unsigned short *arry;
};

extern  int SemID;
extern  int ShmID;

extern  AREA *ShmAddr;

void SemInit(void);

void ShmInit(void);

void SemaphoreP(int num);

void SemaphoreV(int num);

void GetID(void);
#endif
