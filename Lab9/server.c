#include "common.h"

int main(){
	int msg_id = msg_init();
	struct msg_st buf;
	sprintf(MSG0.mtext, "%d", getpid());
	msgsnd(msg_id, &MSG0, strlen(MSG0.mtext) + 1, 0);
	while(msgrcv(msg_id, &buf, sizeof(buf.mtext), 1, 0) <= 0);
	printf("%s\n", buf.mtext);
	while(msgrcv(msg_id, &buf, sizeof(buf.mtext), 1, 0) >  0){
		printf("%s\n", buf.mtext);	
	}
	return 0;
}
