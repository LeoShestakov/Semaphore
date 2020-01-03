#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <fcntl.h>

#define SHM_KEY 10282
#define SEM_KEY 11280
#define SEG_SIZE 200

int main (int argc, char *argv[]) {
	int fd;
	char input[200];
	char *addition;
	int sem;
	int sharedmem;

	sem = semget(SEM_KEY, 1, 0);
	struct sembuf buffer;
	buffer.sem_num = 0;
	buffer.sem_op = -1;
	printf("[[Trying to get in]]\n");
	semop(sem, &buffer, 1);
	sharedmem = shmget(SHM_KEY, SEG_SIZE, IPC_CREAT | 0644);
	fd = open("story.txt", O_WRONLY | O_APPEND);
	addition = shmat(sharedmem, 0, 0);
	printf("Last addition: %s\n", addition);
	printf("Your addition: ");
	fgets(input, 200, stdin);
	strcpy(addition, input);
	write(fd, input, strlen(input));
  	shmdt(addition);
  	buffer.sem_op = 1;
  	semop(sem, &buffer, 1);
	close(fd);
  	return 0;
}
