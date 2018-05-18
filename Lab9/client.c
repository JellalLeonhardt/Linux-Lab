#include "common.h"

int main(){
	int msg_id = msg_init();
	struct msg_st buf;
	sprintf(MSG1.mtext, "%d", getpid());
	msgsnd(msg_id, &MSG1, strlen(MSG1.mtext) + 1, 0);
	while(msgrcv(msg_id, &buf, sizeof(buf.mtext), 2, 0) <= 0);
	printf("%s\n", buf.mtext);
	while(msgrcv(msg_id, &buf, sizeof(buf.mtext), 2, 0) > 0){
		printf("%s\n", buf.mtext);
	}
	return 0;
}
