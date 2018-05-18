#include "common.h"

int main(int args,char* argv[]){
	GetID();
    int ID = atoi(argv[1]);
    printf("生产者%d开始\n", ID);
    int i = 0, cnt = 1;
    char *p = NULL;
    SemaphoreP(0);
	while(cnt != 0){
		if((ShmAddr->head + 1) % 5 == ShmAddr->tail){
			SemaphoreV(0);
			msleep(50);
			SemaphoreP(0);
			continue;
		}
		ShmAddr->tail = (ShmAddr->tail + 4) % 5;
	    p = ShmAddr->buf[ShmAddr->tail];
		cnt = ShmAddr->num[ShmAddr->tail] = read(ShmAddr->fd, p, 10);
	    SemaphoreV(0);
		msleep(20);
		SemaphoreP(0);
	}
	SemaphoreV(0);
	SemaphoreV(1);
    printf("生产者%d生产结束\n", ID);
    return 1;
}
