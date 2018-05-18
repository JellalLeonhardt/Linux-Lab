#ifndef _COMMON_H
#define _COMMON_H

#include <stdio.h>
#include <unistd.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <string.h>
#define MSG_KEY 0xAE86

struct msg_st{
	long msg_type;
	char mtext[512];
};

extern struct msg_st MSG0;
extern struct msg_st MSG1;

int msg_init();

#endif
