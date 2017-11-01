An attempt to implement the C shell with all it's features
# C-shell

## BUILTIN COMMANDS

cd and echo, both work exactly the same like the normal terminal one. We have not included escape characters and made different flags ( as it was mentioned in the news forum ).

(Reading user input again)
## SIGNAL HANDLING AND PIPING/REDIRECTION


All the Basic Signal Handling is done

- [x] Ctrl-D ( to exit the shell )
- [x] & ( to push a process to background )
- [x] Ctrl-C ( to give an interrupt )

## FILES

I have created four dependency files

- builtin.c : This file basically should be renamed builtin.c because it has all the builtin commands (echo, cd, pwd). There are three functions which implements the behaviour of the commands.

- main.c : This file contains the code to do the first part of the question and the basic code to call the builtin functions and other part.

- input.c : As the name of the file conveys, it is used read input, parse them (tokenise them) and give it to other builtin commands or execute another process

- makefile : Code to compile all the files and make the main executable main file.

## COMPILING AND EXECUTION

I have made a makefile which compiles each of the dependencies I have created (.c and .h files which support the main program - main.c). The makefile creates an object file after compilation and in the end compiles all of them with the main program main (because it has the main function). It creates an executable file by the name "main"

To start the shell, just execute the "main" executable file which is created after you use the make command (./main).

##**Created by Saurabh Chand Ramola and Aniket Joshi**