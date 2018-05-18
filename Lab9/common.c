#include "common.h"

struct msg_st MSG0 = {
	.msg_type = 1
};

struct msg_st MSG1 = {
	.msg_type = 2
};

int msg_init(){
	int msg_id;
	if((msg_id = msgget(MSG_KEY, IPC_CREAT)) == -1){
		perror("msg get");
	}
	return msg_id;
}
