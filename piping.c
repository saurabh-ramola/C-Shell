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
int checkpipe(char input_words[][20], int number_of_words)
{
    int i=0;
    int count = 0;
    for(int i=0;i<number_of_words;i++)
    {
        if(strncmp(input_words[i],"|",1) == 0)
        {
            count++;
        }
    }
    return count + 1;
}
int spawn(int in, int out, char *temp[], int i,char first[][1000])
{
    printf("first is %s\n",first[i]);
    printf("temp is %s\n",temp[0]);
    pid_t pid;
    if ((pid = fork ()) == 0)
    {
        if (in!=0)
        {
            dup2 (in, 0);
            close (in);
        }
        if (out!=1)
        {
            dup2 (out, 1);
            close (out);
        }
        printf("before execvp\n");
        return execvp(first[i], temp);
    }
    return pid;
}
int implementpipe(char pip[][1000], int count)
{
    for(int i=0;i<count;i++)
    {
        printf("%s\n",pip[i]);
    }
    char *temp[1000];
    char first[1000][1000];
    for(int i=0;i<count;i++)
    {
        int x = strlen(pip[i]);
        temp[i] = pip[i];
        temp[i][x-1] = '\0';
        int len = strlen(pip[i]);
        for(int j=0;j<len;j++)
        {
            if(pip[i][j] == ' ')
                break;
            first[i][j] = pip[i][j];
        }
        printf("%s\n",temp[i]);
    }

    int i=0, in=0, fd[2];
    pid_t pid;
    while(i<count-1)
    {
        printf("while\n" );
        pipe(fd);
        printf("pipe\n");
        spawn(in, fd[1], temp+i, i, first);
        printf("spawn\n");
        close(fd[1]);
        printf("close\n");
        in = fd[0];
        printf("in is %d\n",0);
        i++;
    }
    if(in!=0)
    {
        dup2(in, 0);
    }
    int len = strlen(temp[i]);
    printf("len is %d\n",len );
    printf("%s\n",temp[i]);
    return execvp(first[i], temp + i);
}
int parse(char *line, char **argv, int x)
{
     while (*line != '\0') {       /* if not the end of line ....... */
          while (*line == ' ' || *line == '\t' || *line == '\n')
               *line++ = '\0';     /* replace white spaces with 0    */
          *argv++ = line;          /* save the argument position     */
          while (*line != '\0' && *line != ' ' &&
                 *line != '\t' && *line != '\n')
               line++;
               x++;             /* skip the argument until ...    */
     }
     *argv = '\0';
     return x;                /* mark the end of argument list  */
}
void  attempt(char **argv,int a)
{
    int i=0;
    execvp(argv[0], argv);
    return;
}

void execute(char *first)
{
    //fprintf(stderr,"asdasd %s\n", first);
    char *argv[64];
    int x = 0;
    int a = parse(first,argv,x);
    argv[a-1] = NULL;
    int i;
    int status;
    pid_t pid = fork();
    if(pid == 0)
        execvp(argv[0], argv);
    else
            {while(wait(&status)!=pid);
            //printf("after exec\n");}
        }
    return;
}
void piping(char *command)
{
    char* first = strtok_r(command,"|",&command);
    char *second = strtok_r(command,"|",&command);

    int f1 = 0;
    if(second == NULL)
    {
        execute(first);
        return;
    }
    int p[2];
    while(first)
    {
        int status;
        pipe(p);
        int pid = fork();
        if(pid == -1)
        {
            exit(0);
        }
        else if (pid == 0)
        {
            dup2(f1, 0);
            if(second!=NULL)
            {
                if (dup2(p[1],1) == -1){}
            }
            close(p[0]);

            execute(first);
            exit(0);
        }
        else
        {
            waitpid(pid,NULL,0);
            close(p[1]);
            f1 = p[0];
        }
        first = second;
        second = strtok_r(command,"|",&command);
    }
}
