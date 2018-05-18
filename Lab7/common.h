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

typedef struct BufferArea{
	int num[5];
	char buf[5][100];
}AREA;

extern  int SemID;
extern  int ShmID;

extern  AREA *ShmAddr;

void SemInit(void);

void ShmInit(void);

void SemaphoreP(int num);

void SemaphoreV(int num);

void GetID(void);
#endif
