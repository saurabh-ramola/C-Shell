#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<sys/types.h>
#include <sys/wait.h>
#include<signal.h>
#include<fcntl.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
typedef struct process
{
	int num;
	char arr[60];
}process;
process * a[200];
void lsDetails(struct dirent *dirp,int a)
{ if(a == 1)
  {	  if(dirp->d_name[0] != '.')
	  {	struct stat fileStat;
		  stat(dirp->d_name,&fileStat);
		  printf( (S_ISDIR(fileStat.st_mode)) ? "d" : "-");
		  printf( (fileStat.st_mode & S_IRUSR) ? "r" : "-");
		  printf( (fileStat.st_mode & S_IWUSR) ? "w" : "-");
		  printf( (fileStat.st_mode & S_IXUSR) ? "x" : "-");
		  printf( (fileStat.st_mode & S_IRGRP) ? "r" : "-");
		  printf( (fileStat.st_mode & S_IWGRP) ? "w" : "-");
		  printf( (fileStat.st_mode & S_IXGRP) ? "x" : "-");
		  printf( (fileStat.st_mode & S_IROTH) ? "r" : "-");
		  printf( (fileStat.st_mode & S_IWOTH) ? "w" : "-");
		  printf( (fileStat.st_mode & S_IXOTH) ? "x" : "-");
		  printf(" %ld ",fileStat.st_size);
		  printf(" %ld ",fileStat.st_nlink);
		  printf(" %ld ",fileStat.st_ino);
		  printf(" %s ", dirp->d_name);
	  	  printf("\n");
	  }
   }
   else
   {
	  struct stat fileStat;
	  stat(dirp->d_name,&fileStat);
	  printf( (S_ISDIR(fileStat.st_mode)) ? "d" : "-");
	  printf( (fileStat.st_mode & S_IRUSR) ? "r" : "-");
	  printf( (fileStat.st_mode & S_IWUSR) ? "w" : "-");
	  printf( (fileStat.st_mode & S_IXUSR) ? "x" : "-");
	  printf( (fileStat.st_mode & S_IRGRP) ? "r" : "-");
	  printf( (fileStat.st_mode & S_IWGRP) ? "w" : "-");
	  printf( (fileStat.st_mode & S_IXGRP) ? "x" : "-");
	  printf( (fileStat.st_mode & S_IROTH) ? "r" : "-");
	  printf( (fileStat.st_mode & S_IWOTH) ? "w" : "-");
	  printf( (fileStat.st_mode & S_IXOTH) ? "x" : "-");
	  printf(" %ld ",fileStat.st_size);
	  printf(" %ld ",fileStat.st_nlink);
	  printf(" %ld ",fileStat.st_ino);
	  printf(" %s ", dirp->d_name);
	  printf("\n");
   }
}
int implementcd(char input_words[][20],char curpath[], char home[], int number_of_words)
{
	if(strcmp(input_words[number_of_words - 2],"&")!=0)
	{
		if (strcmp(input_words[1],"over") == 0)
		{
	    	strcpy(input_words[1],home);
	  	}
	  	else
	  	{
			if (strncmp(input_words[1],"~", 1)==0)
	        	strcpy(input_words[1],home);
	    }
	    int out = chdir(input_words[1]);
	    if(out!=0)
	    {
	    	perror("Error: ");
	    }
	}
  	return 1;
}
void implementpwd()
{
	char my_cwd[1024];
	getcwd(my_cwd, 1024);
	printf("%s\n", my_cwd);
}
int implementEcho(char input_words[][20])
{
  if(strcmp(input_words[1],"over") == 0)
  {
    printf("\n" );
    return 1;
  }
  else
  { int i = 1;
    do{
      printf("%s ",input_words[i]);
      i++;
    }while(strcmp(input_words[i],"over")!=0);
    printf("\n");
  }
}
void implementpinfo(char input_words[][20], char home[])
{
  char filename[1000];
  int flag = 0;
  char s[100];
  strcpy(filename,"/proc/");
  if(strcmp(input_words[1],"over")!=0)
  {
    strcat(filename,input_words[1]);
    flag = 1;
  }
  else
  {
    int i=0;
    int pid = getpid();
    int n = pid;
    do
    {
      s[i++] = n % 10 + '0';
    }while ((n /= 10) > 0);
    s[i] = '\0';
    int j = strlen(s)-1;
    i = 0;
    while(i<j)
    {
      char c = s[i];
      s[i] = s[j];
      s[j] = c;
      i++;
      j--;
    }
    strcat(filename,s);
  }
  char status[1000];
  strcpy(status, filename);
  strcat(status, "/status");
  FILE *fp1 = fopen(status, "r");
  if(!fp1)
  {
      fprintf(stderr,"Invalid Process\n");
      return;
  }
  char l[1000];
  while ( fgets ( l, sizeof l, fp1 ) != NULL )
  {
    char* field = strtok(l,":");
    if(!strcmp(field,"Name"))
    {
      field=strtok(NULL,":");
      printf("Name - %s",field);

    }
    if(!strcmp(field,"State"))
    {
      field=strtok(NULL,":");
      printf("State - %s",field);

    }
    if(!strcmp(field,"Pid"))
    {
      field=strtok(NULL,":");
      printf("Pid - %s",field);

    }
    if(!strcmp(field,"VmSize"))
    {
      field=strtok(NULL,":");
      printf("Virtual Memory - %s",field);

    }

  }
  fclose(fp1);
  if(flag == 0)
    printf("Executable Path:%s/main",home);
  printf("\n");
}
void implementpinfojobs(char arr[20],int x)
{
  char filename[1000];
  int flag = 0;
  char s[100];
  strcpy(filename,"/proc/");
  int n = x;
  int i=0;
  do
  {
    s[i++] = n % 10 + '0';
  }while ((n /= 10) > 0);
  s[i] = '\0';
  int j = strlen(s)-1;
  i = 0;
  while(i<j)
  {
    char c = s[i];
    s[i] = s[j];
    s[j] = c;
    i++;
    j--;
  }
  strcat(filename,s);
  flag = 1;
  char status[1000];
  strcpy(status, filename);
  strcat(status, "/status");
  FILE *fp1 = fopen(status, "r");
  if(!fp1)
  {
      fprintf(stderr,"Invalid Process\n");
      return;
  }
  char l[1000];
  while ( fgets ( l, sizeof l, fp1 ) != NULL )
  {
    char* field = strtok(l,":");
    if(!strcmp(field,"State"))
    {
      field=strtok(NULL,":");
      printf("State - %s ",field);

    }
    fclose(fp1);
    }
}
int implementLS(char input_words[][20])
{ char my_cwd[1024];
  getcwd(my_cwd, 1024);
  DIR *dir;
  struct dirent *dirp;
  int i = 0;
  int flag = 0;

  while(strcmp(input_words[i],"over")!=0)
  {
    if ((dir = opendir (my_cwd)) != NULL) {
     if(strcmp(input_words[i],"-l") == 0){
     while ((dirp = readdir(dir)) != NULL){
          lsDetails(dirp,1);
          flag = 1;
      }
      break;
     }

    else if(strcmp(input_words[i],"-al") == 0 || strcmp(input_words[i],"-la") == 0 || (strcmp(input_words[i],"-a") == 0 && strcmp(input_words[i+1],"-l") == 0) || (strcmp(input_words[i],"-l") == 0 && strcmp(input_words[i+1],"-a") == 0))
    {
    	while ((dirp = readdir(dir)) != NULL){
          lsDetails(dirp,0);
          flag = 1;
      }
      break;
    }
     else if(strcmp(input_words[i],"-a") == 0)
    {
    	while ((dirp = readdir(dir)) != NULL){
          lsDetails(dirp,0);
          flag = 1;
      }
     break;
    }

    else if(strcmp(input_words[i+1],"over") == 0)
    {

      while ((dirp = readdir(dir)) != NULL)
      { if((strcmp(dirp->d_name,".")!=0) || (strcmp(dirp->d_name,"..")!=0))
        if(dirp->d_name[0] != '.')
        {
	        printf("%s ", dirp->d_name);
	        printf("\n");
	    }
      }
      break;

    }
    else if(input_words[i+1][0] != '-')
    {
      printf("No such file or directory\n");
      break;
    }
    }

    else {
    perror ("");
    return EXIT_FAILURE;
    }
	i++;

    closedir (dir);
    }

}

void jobs(int no)
{
    int loop=1;
    while(loop<=no)
    {
        if(a[loop]!=NULL)
        {
            printf("[%d] ", loop );
            printf("%s ", (a[loop])->arr );
            printf("[%d]\n", (a[loop])->num );
        }
        loop++;
    }
}
void kjob(char input_words[][20], int no)
{
    if(input_words[1] == "over" || input_words[2] == "over")
    {
        perror("Usage: kjob [process no.] [signal no.]");
    }
    int signo = atoi(input_words[2]);
    int procno =atoi(input_words[1]);
    int i;
    for(i=1;i<=no;i++)
    {
        if(a[i]!= NULL && a[i]->num == procno)
        {
            break;
        }
    }
    //printf("hi\n" );
    if(i==no+1)
    {
        perror("process does not exist");
    }
    int id = (a[i])->num;
    a[i]=NULL;
    int ret = kill(id, signo);

}
void fg(char input_words[][20], int no)
{
    if(input_words[1] == "over")
    {
        perror("Wrong usage of fg command");
    }
    int procno = atoi(input_words[1]);
    int i;
    for(i=1;i<=no;i++)
    {
        if(a[i]!=NULL && a[i]->num == procno)
        {
            break;
        }
    }
    if(i == no+1)
    {
        perror("No such process exists");
        return;
    }
    int id = a[i]->num;
    a[i] = NULL;
    kill(id, SIGCONT);

}
void overkill(int no)
{
    for(int i=1;i<=no;i++)
    {
        if(a[i]!=NULL)
        {
            int id = a[i]->num;
            a[i] = NULL;
            kill(id, 9);

        }
    }
}
