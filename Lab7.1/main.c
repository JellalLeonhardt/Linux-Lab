#define MAIN
#include "common.h"

#define CREATOR_NUM 3
#define COSTER_NUM 4
int main(int args, char *argv[]){
	SemInit();
	ShmInit();
	
	ShmAddr->head = ShmAddr->tail = 0;

	AREA *Buffer = ShmAddr;
	int CosterNum, i;
	char c = 0;
	char number_i[50];
	char number_CreatorNum[50];
	pid_t pid;
	
	if(args == 2){
		int fd = open(argv[1], O_RDONLY);
		if(fd == -1){
			perror("open file");
			exit(1);
		}
		Buffer->fd = fd;
		int target_fd = open("All.txt", O_RDWR | O_TRUNC);
		if(target_fd == -1){
			perror("open file");
			exit(1);
		}
		Buffer->target_fd = target_fd;
	}

	printf("开始创建生产者\n");
	for(i = 0;i < CREATOR_NUM;i++){
		sprintf(number_i, "%d", i);
		pid = fork();
		if(pid == 0){
			execl("./Creator.o", "Creator", number_i, NULL);
			perror("execl");
			exit(0);
		}
	}

	printf("开始创建消费者\n");
	for(i = 0;i < COSTER_NUM;i++){
		sprintf(number_i, "%d", i);
		pid = fork();
		if(pid == 0){
			execl("./Coster.o", "Coster", number_i, NULL);
			perror("execl");
			exit(0);
		}
	}
	
	SemaphoreP(2);
	printf("结束了一个消费者\n");
	SemaphoreP(2);
	printf("结束了一个消费者\n");
	SemaphoreP(2);
	printf("结束了一个消费者\n");
	SemaphoreP(2);
	printf("结束了一个消费者\n");
	close(ShmAddr->fd);
	close(ShmAddr->target_fd);
	union semun sem_union;
	semctl(SemID, 0, IPC_RMID, sem_union);
	shmdt(ShmAddr);
	shmctl(ShmID, IPC_RMID, 0);
	printf("主进程结束\n");
	return 0;
}
