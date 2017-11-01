#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<sys/types.h>
#include <sys/wait.h>
#include<signal.h>
#include<fcntl.h>
#include <stdlib.h>
int input(char input_commands[], char input_sentences[][1000], char copy[])
{
	ssize_t bufsize = 1024;
	if(getline(&input_commands,&bufsize, stdin)==-1)
	{
		printf("\n");
		exit(0);
 	}
 	strcpy(copy, input_commands);
 	char *command;
 	command = strtok(input_commands,";\n");
	int i=0;
	while(command != NULL)
	{
		strcpy(input_sentences[i++],command);
		command = strtok(NULL,";\n");
	}
	return i;
}
int split(char sentence[], char input_words[][20])
{
	char *command;
 	command = strtok(sentence," \t\r\n");
	int i=0;
	while(command != NULL)
	{
		strcpy(input_words[i++],command);
		command = strtok(NULL," \t\r\n");
	}
	strcpy(input_words[i],"over");
	return i+1;
}
