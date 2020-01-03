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

union semun {
	int value;
	struct semid_ds *buffer;
	unsigned short *array;
} su;

void getcurrent() {
	FILE * open = fopen("story.txt", "r");
  	printf("The story so far:\n");
  	char line[200];
  	while(fgets(line, 200, open)) {
    		printf("%s", line);
  	}
  	fclose(open);
}

int main(int argc, char *argv[]) {
  	int sem;
  	int sharedmem;
  	char *addition;
  	int fd;

  	if (argc > 1 && !strcmp(argv[1], "-c")) {
    		sem = semget(SEM_KEY, 1, IPC_CREAT | 0644);
    		su.value = 1;
      	semctl(sem, 0, SETVAL, su);
    		printf("Semaphore created\n");
    		sharedmem = shmget(SHM_KEY, SEG_SIZE, IPC_CREAT | 0644);
    		addition = shmat(sharedmem, 0, 0);
    		if (addition){
      		strcpy(addition, "");
      		shmdt(addition);
    		}
    		printf("Shared memory created\n");
    		fd = open("story.txt", O_CREAT | O_RDWR | O_TRUNC, 0644);
      	printf("File created\n");
      	close(fd);
  	}

  	if (argc > 1 && !strcmp(argv[1], "-v")) {
    		getcurrent();
  	}

  	if (argc > 1 && !strcmp(argv[1], "-r")) {
    		printf("[[Trying to get in]]\n");
    		sem = semget(SEM_KEY, 1, 0);
    		struct sembuf buff;
    		buff.sem_num = 0;
    		buff.sem_op = -1;
    		semop(sem, &buff, 1);
    		getcurrent();
    		sharedmem = shmget(SHM_KEY, SEG_SIZE, IPC_CREAT | 0644);
    		shmctl(sharedmem, IPC_RMID, 0);
		printf("-----\n");
    		printf("Shared memory removed\n");
    		remove("story.txt");
    		printf("File removed\n");
    		semctl(sem, IPC_RMID, 0);
    		printf("Semaphore removed\n");
  	}

  	return 0;
}
