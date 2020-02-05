Names: Brayden Faulkner and Hayden Nanney

	The best way to explain the program is by walking through each of the programs functions and what they do. The simpliest and first among them is the 
program's main function. All main does is call either the shellMode or batchMode functions based on the number of command line arguements it recieves. 
It can also return 0 and print an error message if the program recieves an incorrect number of command line arguements.
	
The next function to discuss is the shellMode function. This function runs if the program recieves no command line arguements and contains a loop that runs until 
the program revieves the exit command. It recieves input from the getInput function. It parses the string and sends it to the correct functions based on the 
characters found in the string. If the command contains a ';', then it passes it to the parseString function that splits the command into its seperate commands. 
It then runs each command through the splitSpace and execComm functions. If the command is just the newline character, the function literally does nothing. 
If the program does not contain a ;, then it runs the command through the splitSpace and execComm functions.

The getInput function reads commands from the terminal and passes them to the shellMode function. It also removes the newline character from the end of the command, 
and returns just the newline if the input is empty.

The splitSpace function takes two parameters, command and args. Command is the raw input, while args is the input split up into its seperate arguements. 
It checks for a space and only runs most of the function if one is found. If one is not found it just puts the command and a null character into the array and returns it. 

The parseString fucntion's purpose is to parse through a string and separate each command that is separated by semicolons and return them as an array of strings for use in
the splitSpace function.

The batchMode function is ran upon execution if a file argument is given. It read in each line and separates the commands using either splitSpace and/or parseString functions.
It executes all of the commands in the file and the exits if no quit command was called. Currently, there is a known issue with this function: it does not quit when quit is read from the file.

The exec commm function takes an 2d array of chars called args as a parameter. args contains the command already split up into its individual arguements. 
The function first checks if the first arguement, which contains the actual command, is exit and exits if it is. It then forks the program so it can run the command in 
a child process. It then executes the commands in the child process, and waits for the child process to exit in the parent process. The parent process then returns so 
the next command can be taken in.
