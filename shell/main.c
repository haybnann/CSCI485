//Name: main.c
//Authors: Brayden Faulkner and Hayden Nanney
//Date: 02/04/2020


#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#define MAX 512



//Function that actually executes the commands
void execComm(char **args){
	pid_t pid = fork();
	if(pid <= -1){
		printf("Error with fork \n");
		exit(1);
	}else if(pid == 0){
		if(args[0] == "\n"){exit(1);}
		if(execvp(args[0], args) < 0){
			printf("Error with command \n");
			printf("%s\n",args[0]);
			exit(1);
		}
		exit(0);
	}else{
		wait(NULL);
		return;
	}
}

void splitSpace(char *command, char** args){

	char *currArg;
	//This will seperate up the line into all the different arguements
	if(strstr(command, " ") != NULL){
		if(command[(strlen(command)-1)] == '\n'){ 			// replace '\n' character
			 command[(strlen(command) - 1)] ='\0';
		}
		if(command[(strlen(command)-1)] == '\r'){ 			// replace '\r' character
			 command[(strlen(command) - 1)] ='\0';
		}
		int i=0;						
     		while(command[i]!='\0'){					//replace all tabs with a space
           		if(command[i]=='\t'){
				command[i]=' ';
           		}  
           		i++; 
     		}
		i =0;
		currArg = strtok(command, " ");
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
	FILE *fp = fopen(argv, "r");		//open file
	if(fp == NULL){
		//Error Code Here
		fprintf(stderr, "File %s does not exist", argv);
		exit(1);
	}
	char *line;
	size_t len = 0;
	ssize_t read;
	char *args[100];
	args[0] =NULL;
	
	while((read = (getline(&line, &len, fp)) != -1)){
		if(line[(strlen(line)-1)] == '\n'){ 			// replace '\n' character
			 line[(strlen(line) - 1)] ='\0';
		}
		if(line[(strlen(line)-1)] == '\r'){ 			// replace '\n' character
			 line[(strlen(line) - 1)] ='\0';
		}
		int i =0;
		while(line[i]!='\0'){					//replace all tabs with a space
           		if(line[i]=='\t'){
				line[i]=' ';
           		}  
           		i++; 
     		}

		if(strstr(line, ";") != NULL ){		//if more than one command, use parser
			int j = 0;
			char** arr  	= parseString(line);					//parse and split, return array of strings
			char* currArg 	= arr[j];						//assign first string to currentArg
			while(currArg != NULL && *currArg != '\n'&& (strcmp(currArg, "quit") != 0)){				//iterate through all string commands
				splitSpace(currArg,args);					//split commands into parts
				execComm(args);							//execute the commands
				j++;								//increment to next string
				currArg = arr[j];
			}
			/*if(strcmp(currArg, "quit") != 0){ 
				//wait till the others finish
				exit(0);
			}*/
		}else{					//else, split on spaces
			args[0] = NULL;
			splitSpace(line, args);
			execComm(args);
		}
	}
	//free(line);		//free allocated space
	fclose(fp);		//close file stream
	exit(0);		//exit successfully at end of file

}

//Takes the input from the command line and puts into command
char *getInput(){
	printf("%s", "prompt>");
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

	//reads input from command line
	command = getInput();


	while(strcmp(command, "quit") != 0){
		if(strstr(command, ";") != NULL){			//if more than one command

			int i=0;						
     			while(command[i]!='\0'){					//replace all tabs with a space
           			if(command[i]=='\t'){
					command[i]=' ';
           			}  
           			i++; 
     			}

			i = 0;
			char** arr  = parseString(command);		//parse,split and execute commands
			char* currArg = arr[i];
			while(currArg != NULL){				//last element was assigned to be NULL 
				splitSpace(currArg,args);
				if(*args == "quit"){break;}
				execComm(args);
				i++;
				currArg = arr[i];
			}

		}else if(strcmp(command, "\n") == 0){			//if empty, do nothing
			//do nothing
		}else{							//else, split on spaces
			splitSpace(command, args);
			execComm(args);

		}

		command = getInput();
	}

	exit(0);
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
