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
	if(pid <= -1){
		printf("Error with fork \n");
		exit(1);
	}else if(pid == 0){
		if(execvp(args[0], args) < 0){
			printf("Error with command \n");
			exit(1);
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


/*splits multiple commands separated by ';' into dynamically allocated 2d array*/
char  **parseString(char* command){//mem leak if not deleted 

	char** commandArray;		

	if(strstr(command, ";") != NULL){			//if it contains a ';'

		char* copy = (char*) malloc(sizeof(command)); 	//copy of input since strtok modifies it
		strcpy(copy,command);

		int  numCommands = 0;
		char* currCommand;
		currCommand = strtok(copy, ";"); 		//split at ';'

		while(currCommand != 0){			//need to know how much space to allocate
			currCommand = strtok(NULL, ";");	//next token
			numCommands++;
		}
		commandArray = (char**) malloc((numCommands+1)*sizeof(char*)); 	//allocate rows + 1 for NULL ptr
		//add if statement to check if commandArray == NULL
		currCommand = strtok(command, ";");
		
		for(int i = 0; i < numCommands; i++){
			commandArray[i] = malloc((sizeof(currCommand)+1)*sizeof(char));//allocate each command
			if(commandArray[i] != NULL){
				strcpy(commandArray[i],currCommand);
			}else{
				perror("ERROR: No Memory");
				exit(1);
			}
			currCommand = strtok(NULL, ";");
		}
		commandArray[numCommands] = NULL;				//to find end of pointer array later
	}
	return commandArray;
}

/*Useful for debugging  - prints array of strings*/
void printParsedString(char** strArr){
	int i = 0;
	while(strArr[i] != NULL){
		printf("%s\n",strArr[i]);
		i++;
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

	while(read = (getline(&line, &len, fp) != -1)){
		printf("%s", line);
	}

}

//Takes the input from the command line and puts into command
char *getInput(){
	char *command = NULL;
	ssize_t bufferSize = 0;
       	getline(&command, &bufferSize, stdin);
	int len = strlen(command);
	command[len-1] = '\0';
	return command;
}

void shellMode(){
	char *command;
	char *args[100];
	command = getInput();
	if(strstr(command, ";") != NULL){			//if more than one command
		printf("%s\n", "parse");
		int i = 0;
		char** arr  = parseString(command);		//parse,split and execute commands
		char* currArg = arr[i];
		while(currArg != NULL){
			splitSpace(currArg,args);
			execComm(args);
			i++;
			currArg = arr[i];
		}

	}else if(strcmp(command, "\n") == 0){			//if empty, do nothing
		//do nothing
		printf("%s\n","do nothing");
	}else{							//else, split on spaces
		printf("%s\n", "SplitSpace");
		splitSpace(command, args);
		execComm(args);

	}
}

void displayPrompt(){
	printf("prompt> ");
}

int main(int argc, char *argv[]) {//argc =#strings
 	while(1){
		displayPrompt();
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
}