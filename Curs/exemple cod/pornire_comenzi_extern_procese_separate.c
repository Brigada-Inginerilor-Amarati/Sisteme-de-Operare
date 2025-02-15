#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
  pid_t pid[2], wpid;
 
  char *arg1[]={ "echo", "a", "b", "c", NULL };
  char *arg2[]={ "ls", "-l", ".", NULL };
  int i, status;
   
  char ** param[2];
   
  param[0] = arg1; 
  param[1] = arg2;
  
  for(i=0; i<2; i++)
  {	
     if((pid[i]=fork())<0)
     {
	printf("Eroare la fork\n");
	exit(1);
     }
     if(pid[i]==0) /* procesul fiu i */
     {
	execvp(param[i][0], param[i]);
	printf("Eroare la exec\n");
	exit(2);
     }
  }
  
  printf("Processes started:\n");
  for(i=0; i<2; i++)
    printf("%d ", pid[i]);
  printf("\n");
   
  /* procesul parinte */
  for (i=1; i<=2; i++)
  {	
    wpid = wait(&status);
    if(WIFEXITED(status))
      printf("\nChild %d ended with code %d\n", wpid, WEXITSTATUS(status));
    else
      printf("\nChild %d ended abnormally\n", wpid);
  }  
}