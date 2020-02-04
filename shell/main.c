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
//Function that actually executes the commands
void execComm(char **args){
	pid_t pid = fork();
	if(pid == -1){
		printf("Error with fork \n");
	}else if(pid == 0){
		if(execvp(args[0], args) < 0){
			printf("Error with command \n");
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
	//This will seperate up the line into all the different arguements
	if(strstr(command, " ") != NULL){
		currArg = strtok(command, " ");
		while(currArg != NULL){
			args[i] = currArg;
			i++;
			currArg = strtok(NULL, " ");
		}
		args[i] = NULL;
	} else{
		args[0] = command;
		args[1] = NULL;
	}
}
//Seperates the line into different commands if ; is found
//Code here is untested and should be changed to return 2d array of each
//command that is seperated
char  **parseString(char* command){
	if(strstr(command, ";") != NULL){
		char* currCommand;
		currCommand = strtok(command, ";");
		while(currCommand != 0){
			currCommand = strtok(command, ";");
		}
	}else{
	}
}

void batchMode(char *argv){
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

//Takes the input from the command line and puts into command
char *getInput(){
	char *command = NULL;
	ssize_t bufferSize = 0;
       	getline(&command, &bufferSize, stdin);
	if(strcmp(command, "\n") != 0){
		return command;
	}else{
		int len = strlen(command);
		command[len-1] = '\0';
		printf("%s", command);
		return command;
	}
}

void shellMode(){
	char *command;
	char *args[100];
	command = getInput();
	while(strcmp(command, "exit") != 0){
		//What will run if more than one command is found
		//Should take 2d Array of commands and run splitSpace and
		//execComm over each one
		if(strstr(command, ";") != NULL){
			char **commands = parseString(command);
		}
		//Does nothing if command is empty
		else if(strcmp(command, "\n") != 0){	
		}
		else{
			splitSpace(command, args);
			execComm(args);
		}
		command = getInput();
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
