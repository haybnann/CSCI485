#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#define MAX 512

void batchMode(char *argv){
	printf("The program has been opened in Batch Mode\n");
}

void shellMode(){
	char command[MAX];
       	fgets(command, MAX, stdin);
	while(strcmp(command, "quit") != 0){
		int status;
		int cpid = fork();
		if(cpid == 0){
			printf("%s", command);
			exit(1);
		}
		else{
			waitpid(cpid, &status, 0);
			printf("Done\n");
		}
		fgets(command, MAX, stdin);
	}
}

int main(int argc, char *argv[]) {
	if(argc < 2){
		shellMode();
	}
	if(argc == 2){
		batchMode(argv[1]);
	}
	if(argc > 2){
		printf("ERROR: An invalid number of command line arguements were given to the program");
		return 0;
	}
}
