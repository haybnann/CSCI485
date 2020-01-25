#include <stdio.h>

void batchMode(char *argv){
	printf("The program has been opened in Batch Mode \n");
}

void shellMode(){
	printf("The program has been opened in Shell Mode \n");
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
