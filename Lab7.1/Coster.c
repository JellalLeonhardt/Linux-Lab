#include "common.h"

int main(int argv,char* args[]){
	GetID();
    int CreatorNum = atoi(args[1]);
    int i = 0;
    printf("消费者%d开始\n", CreatorNum);
    char *p = NULL;
    int cnt;
    while(1){
		SemaphoreP(0);
		if(semctl(SemID, 1, GETVAL, 0) == 3 && ShmAddr->head == ShmAddr->tail){
			SemaphoreV(2);
			SemaphoreV(0);
    		printf("消费者%d消费结束\n", CreatorNum);
			break;
		}
		else if(ShmAddr->head == ShmAddr->tail){
			SemaphoreV(0);
			msleep(50);
			continue;
		}
		ShmAddr->head = (ShmAddr->head + 4) % 5;
		cnt = ShmAddr->num[ShmAddr->head];
		p = ShmAddr->buf[ShmAddr->head];
        write(ShmAddr->target_fd, p, cnt);
		SemaphoreV(0);
    }
    return 0;
}
