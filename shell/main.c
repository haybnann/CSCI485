#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#define MAX 512

void batchMode(char *argv){
	//printf("The program has been opened in Batch Mode\n");
	FILE *fp = fopen(argv, "r");
	if(fp == NULL){
		//Error Code Here
		printf("File %s does not exist", argv);
		exit(1);
	}
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	//int count = 0;
	while(read = getline(&line, &len, fp) != -1){
		printf("%s", line);
	}

}

void shellMode(){
	char command[MAX];
       	fgets(command, MAX, stdin);
	while(strcmp(command, "quit") != 0){
		int status;
		int cpid = fork();
		if(cpid == 0){
			//printf("%s", command);
			exit(1);
		}
		else{
			waitpid(cpid, &status, 0);
			//printf("Done\n");
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
