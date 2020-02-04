//Name: main.c
//Authors: Brayden Faulkner and Hayden Nanny
//Date: 02/04/2020


#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

//Function that actually executes the commands
void execComm(char **args){
	if(strcmp(args[0], "exit") == 0){
		exit(0);
	}else{
		pid_t pid = fork();
		if(pid == -1){
			printf("Error with fork \n");
		}else if(pid == 0){
			if(execvp(args[0], args) < 0){
				fprintf(stderr,"Error with command \n");
			}
			exit(0);
		}else{
			wait(NULL);
			return;
		}
	}
}

void splitSpace(char *command, char** args){
	int i = 0;
	char *currArg;
	//This will seperate up the line into all the different arguements
	if(strstr(command, " ") != NULL){
		currArg = strtok(command, " ");
		//Loops through the commands to seperate the string on spaces
		while(currArg != NULL){
			args[i] = currArg;
			i++;
			currArg = strtok(NULL, " ");
		}
		args[i] = NULL;
	} else{ 						//if no spaces are found in the string
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
		fprintf(stderr, "File %s does not exist", argv);
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
	if(strcmp(command, "\n") == 0){ 			//If input is empty returns nothing
		return command;
	}else{
		int len = strlen(command);
		//removes the newline character from the end of the input
		command[len-1] = '\0';
		return command;
	}
}
//Function that controls shell mode
void shellMode(){

	char *command;
	char *args[100];
	printf("prompt> ");
	//reads input from command line
	command = getInput();
	while(strcmp(command, "exit") != 0){
		//What will run if more than one command is found
		//split commands & run concurrently
		if(strstr(command, ";") != NULL){
			char **commands = parseString(command);
			int x = 0;
			//Gets numbers of commands in the array
			int count  = sizeof commands / sizeof commands[0];
			while(x <= count ){
				command = commands[x];
				splitSpace(command, args);
				execComm(args);
				x++;
			}
		}
		//Does nothing if command is empty
		else if(strcmp(command, "\n") == 0){
		}else{
			splitSpace(command, args);
			execComm(args);
		}
		printf("prompt> ");
		command = getInput();

	}
}


int main(int argc, char *argv[]) {//argc =#strings
	if(argc < 2){
		shellMode();
	}
	if(argc == 2){
		batchMode(argv[1]);
	}
	if(argc > 2){			//gives error if too many command line arguements
		fprintf(stderr, "ERROR: An invalid number of command line arguements were given to the program \n");
		return 0;
	}
}
