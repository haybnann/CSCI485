#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#define MAX 512

int charCount(char toCount, char *str){
	int count = 0;
	int i;
	size_t len = strlen(str);
	for(i = 0; i < len; i++){
		if(str[i] == toCount)
			count++;
	}
	return count;
}

void execComm(char **args){
	printf("%s", args[0]);
	pid_t pid = fork();
	if(pid == -1){
		printf("Error with fork");
		//perror("lsh");
	}else if(pid == 0){
		if(execvp(args[0], args) < 0){
			printf("Error with command");
		}
		exit(0);
	}else{
		wait(NULL);
		return;
	}
}

void splitSpace(char *command, char** args){
	int i = 0;
	char *currArg;
	if(strstr(command, " ") != NULL){
		currArg = strtok(command, " ");
		printf("Here \n");
		while(currArg != NULL){
			printf("%d", i);
			args[i] = currArg;
			i++;
			currArg = strtok(command, " ");
		}
		args[i] = NULL;
	} else{
		args[0] = command;
		args[1] = NULL;
	}
}

void parseString(char* command){
	if(strstr(command, ";") != NULL){
		//int commCount = charCount(';', command);
		//const char *commands[commCount];
		//commands[0] = strtok(command, ";");
		//int i = 1;
		/*while(i <= commCount){
			commands[i] = strtok(command, ";");
			i++;
		} */
		char* currCommand;
		currCommand = strtok(command, ";");
		while(currCommand != 0){
			currCommand = strtok(command, ";");
		}
	}else{
	}
}

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
	while(read = (getline(&line, &len, fp) != -1)){
		printf("%s", line);
	}

}

char *getInput(){
	char *command = NULL;
	ssize_t bufferSize = 0;
       	getline(&command, &bufferSize, stdin);
	return command;
}

void shellMode(){
	char *command;
	char *args[100];
	command = getInput();
	printf("%s", command);
	if(strstr(command, ";") != NULL){
		//What will run if more than one command is found
	}else{
		splitSpace(command, args);
		printf("%s", args[0]);
		execComm(args);
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
