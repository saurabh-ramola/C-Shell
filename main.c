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
pid_t SHELLPID;
typedef struct process
{
	int num;
	char arr[60];
}process;
process * a[200];
int no = 0;
void piping(char *command);
void fg(char input_words[][20], int no);
void kjob(char input_words[][20], int no);
void implementpinfo(char input_words[][20], char home[]);
int implementcd(char input_words[][20],char curpath[], char home[],int number_of_words);
int input(char input_commands[], char input_sentences[][1000], char copy[]);
int split(char sentence[], char input_words[][20]);
void implementpwd();
char input_commands[10000];
char input_sentences[1000][1000];
char copy[10000];
void overkill(int no);
int implementEcho(char input_words[][20]);
int implementLS(char input_words[][20]);
void redirect(char input_words[][20], int number_of_words);
int checkpipe(char input_words[][20], int number_of_words);
int implementpipe(char pip[][1000], int count);
void jobs(int no);
void deleteJob(int jobpid)
{
    int i;
    int flag=0;
    for(i=0;i<no;i++)
    {
        if(a[i]!=NULL && (a[i])->num == jobpid )
            flag=1;

        if(flag==1)
            a[i]=a[i+1];
    }
	no--;
}
void printEnv()
{
	char * home = (char *)malloc(1000*sizeof(char));
	char *relative = (char*)malloc(1000*sizeof(char));
	char hostname[1024];
	char curpath[1024];
	struct passwd * myname=NULL;
	myname = getpwuid(getuid());
	gethostname(hostname, 1023);
	hostname[1023] = '\0';
	strcpy(home, getenv("PWD"));
	getcwd(curpath, 1024);
	int ret = strncmp(home, curpath, strlen(home));
	if(ret == 0)
	{
		relative[0] = '~';
		int ptr = 1;
		int i;
		for(i = strlen(home);i<strlen(curpath);i++)
		{
			relative[ptr] = curpath[i];
			ptr++;
		}
		relative[strlen(curpath)] = '\0';
	}
	else
	{
		strcpy(relative,curpath);
	}
	printf("<%s@%s>:%s ",myname->pw_name,hostname,relative);
}
void sig_handler(int sig)
{
    if(sig == SIGINT)
    {
        printf("\n");
        printEnv();
        fflush(stdout);
    }
    if(sig == SIGTSTP)
    {
        printf("\n");
        printEnv();
        fflush(stdout);
    }
    if(sig == SIGQUIT)
    {
        printf("\n");
        printEnv();
        fflush(stdout);
    }
    if(sig == SIGCHLD)
    {
        union wait wstat;
        pid_t pid;

        while(1)
        {
            pid = wait3(&wstat,WNOHANG,(struct rusage *)NULL);
            if(pid == 0 )
                return;
            else if(pid == -1)
                return;
            else
            {
                fprintf(stderr,"\nProcess with PID : %d exited with return value: %d\n",pid,wstat.w_retcode);
                deleteJob(pid);
            }
        }
    }
}

void proc_exit()
{
	union wait wstat;
	pid_t pid;

	while (1)
	{
		pid = wait3(&wstat, WNOHANG, (struct rusage *)NULL );
		if (pid == 0)
			return;
		else if (pid == -1)
			return;
		else
			fprintf (stderr, "\nProcess with pid %d has been exited normally.\n", pid);
			deleteJob(pid);
	}
	return;
}

int main()
{
	while(1)
	{
		signal(SIGINT,SIG_IGN);
    	signal(SIGQUIT,SIG_IGN);
    	signal(SIGTSTP,SIG_IGN);
    	if( signal(SIGINT,sig_handler) == 0 )
        	continue;
    	if( signal(SIGQUIT,sig_handler) == 0 )
        	continue;
    	if ( signal(SIGTSTP,sig_handler) == 0 )
			continue;
		printEnv();

		char *temp[20];
		char cwd[1024];
		cwd[1023]='\0';
		char *original=getcwd(cwd, sizeof(cwd));
		char string[1024];
		strcpy(string,original);

		int i, j = 0;
		char *pass;
		char temp1[100][100]={0};
		int background;
		char * home = (char *)malloc(1000*sizeof(char));
		char *relative = (char*)malloc(1000*sizeof(char));
		char hostname[1024];
		char curpath[1024];
		strcpy(home, getenv("PWD"));
		getcwd(curpath, 1024);
		int number_of_lines = input(input_commands,input_sentences,copy);
		for(i=0;i<number_of_lines;i++)
		{
			char input_words[20][20]={0};
			int number_of_words = split(input_sentences[i],input_words);
			int count = checkpipe(input_words, number_of_words-1);
			if(count>1)
			{
				char command[2000]={0};
				int j=0;
				while(strcmp(input_words[j],"over")!=0)
				{
					strcat(command,input_words[j]);
					strcat(command," ");
					j++;
				}
				//printf("command is %s\n",command);
				piping(command);
				continue;
			}
			//printf("%d\n",count);
			background=(strcmp(input_words[number_of_words-2],"&")==0);
			j = 0;
			while(strcmp(input_words[j],"over")!=0)
			{
				temp[j]=input_words[j];
				j++;
			}
			temp[j] = NULL;
			if(((strcmp(input_words[0],"cd")==0) ||strcmp(input_words[0],"setenv") == 0 || strcmp(input_words[0],"unsetenv") == 0 || strcmp(input_words[0],"overkill") == 0 || strcmp(input_words[0],"fg") == 0 || strcmp(input_words[0],"kjob") == 0 || strcmp(input_words[0],"pinfo") == 0 || strcmp(input_words[0],"jobs") == 0 || strcmp(input_words[0],"pwd") == 0 || strcmp(input_words[0],"echo")==0 || strcmp(input_words[0],"ls")==0 || strcmp(input_words[0],"quit")==0) && (background==0))
			{
				if(strcmp(input_words[0],"cd") == 0)
				{
					implementcd(input_words,curpath,home,number_of_words);
				}
				if(strcmp(input_words[0],"overkill") == 0)
				{
					overkill(no);
				}
				if(strcmp(input_words[0],"setenv") == 0)
				{
					setenv(input_words[1],input_words[2],1);
					//system("/bin/sh");
				}
				if(strcmp(input_words[0],"unsetenv") == 0)
				{
					unsetenv(input_words[1]);
				}
				if(strcmp(input_words[0],"fg") == 0)
				{
					fg(input_words,no);
				}
				if(strcmp(input_words[0],"kjob") == 0)
				{
				//	printf("yo\n");
					kjob(input_words,no);
				}
				if(strcmp(input_words[0],"jobs") == 0)
				{
					jobs(no);
				}
				else if (strcmp(input_words[0],"pwd") == 0)
				{
					implementpwd();
				}
				else if (strcmp(input_words[0],"echo") == 0)
				{
					implementEcho(input_words);
				}
				else if (strcmp(input_words[0],"quit") == 0)
				{
					exit(0);
				}
				else if(strcmp(input_words[0],"pinfo") == 0)
				{
					implementpinfo(input_words, home);
				}
				else if (strcmp(input_words[0],"ls") == 0)
				{
					implementLS(input_words);
				}
			}
			else
			{
				if(strcmp(input_words[0],"cd") == 0)
				{
					implementcd(input_words,curpath,home,number_of_words);
				}
				else
				{
					pid_t pid;
					int status;
					int j = 0;
					int x;
					signal (SIGCHLD, proc_exit);
					pid=fork();
					if(background == 1 && pid!=0)
					{
						printf("yes\n");
						no++;
						a[no] = (process *) malloc(sizeof(process));
						(a[no])->num = pid;
						strcpy((a[no])->arr, input_words[0]);
						printf("%d\n",a[no]->num);
					}
					if(pid<0)
					{
						fprintf(stderr, "Fork failed");
					}
					else if(pid == 0)
					{
						redirect(input_words, number_of_words - 2);
						while(strcmp(input_words[j],"over")!=0)
						{
							if(strcmp(input_words[j],"redirection") == 0)
								temp[j] = NULL;
							else
								temp[j]=input_words[j];
							j++;
						}
						if(background == 1)
						{

							temp[j-1] = NULL;
						}
						else
							temp[j] = NULL;
						if(execvp(input_words[0],temp)<0)
						{
							perror("Error");
							exit(0);
						}
					}
					else
					{
						if(background == 0)
						{
							wait(&status);
						}
						else if(background == 1)
						{
							printf("Process Started: %s [%d]\n",input_words[0],pid);
						}
					}
				}
			}
		}
	}
	return 0;
}
