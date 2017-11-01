#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<sys/types.h>
#include <sys/utsname.h>
#include<string.h>
#include <sys/wait.h>
#include<signal.h>
#include<fcntl.h>
#include <stdlib.h>
#include <pwd.h>
//char input[100],output[100],extension[100];
void redirect(char input_words[][20], int number_of_words)
{
    int inp = 0;
    int out = 0;
    int extend = 0;
    int fd1,fd2,fd3;
    for(int i = 0;i<number_of_words;i++)
    {
        if(strcmp(input_words[i],"<") == 0)
        {
            inp = 1;
            fd1 = open(input_words[i+1],O_RDONLY,0);
            if(fd1 < 0)
            {
                perror("Could not open the input file");
                exit(0);
            }
            dup2(fd1,0);
            close(fd1);
            strcpy(input_words[i],"redirection");
        }
        if(strcmp(input_words[i],">") == 0)
        {
            out = 1;
            fd1 = open(input_words[i+1], O_WRONLY | O_CREAT, 0666);
            if(fd1 < 0)
            {
                perror("Could not open the output file");
                exit(0);
            }
            dup2(fd1,1);
            close(fd1);
            strcpy(input_words[i],"redirection");
        }
        if(strcmp(input_words[i],">>") == 0)
        {
            extend = 1;
            fd1 = open(input_words[i+1], O_WRONLY | O_CREAT | O_APPEND , 0666);
            if(fd1 < 0)
            {
                perror("Could not open the output file");
                exit(0);
            }
            dup2(fd1,1);
            close(fd1);
            strcpy(input_words[i],"redirection");
        }
    }
    return ;
}
