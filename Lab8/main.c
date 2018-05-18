#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(int agrs, char *argv[]){
	int fd[2];
	pid_t pid[2];
	char str0[] = "Child process 1 is sending a message!\n";
	char str1[] = "Child process 2 is sending a message!\n";

	if(pipe(fd) < 0){
		perror("pipe init");
		exit(1);
	}

	pid[0] = fork();

	if(pid[0] == 0){
		close(fd[0]);
		write(fd[1], str0, strlen(str0) + 1);
		return 0;
	}

	pid[1] = fork();

	if(pid[1] == 0){
		close(fd[0]);
		write(fd[1], str1, strlen(str1) + 1);
		return 0;
	}

	waitpid(pid[0], NULL, 0);
	waitpid(pid[1], NULL, 0);
	close(fd[1]);
	char buf0[100];
	char buf1[100];
	read(fd[0], buf0, strlen(str0) + 1);
	read(fd[0], buf1, strlen(str1) + 1);
	printf("%s", buf0);
	printf("%s", buf1);
	return 0;
}
