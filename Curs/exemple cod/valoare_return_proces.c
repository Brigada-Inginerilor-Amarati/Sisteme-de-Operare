#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
void process(char chr, int n){
   int i;
   for(i=0; i<=n-1; i++)
    printf("%c", chr);
}

int main(){
   pid_t pid; int status;
   
   if((pid=fork())<0){
     printf("Error creating child process\n"); exit(1);
  }
   if(pid==0) /* procesul fiu */ {
     process('c', 2000);
     exit(0);
  }
   /* procesul parinte*/
   process('p', 3000);
   wait(&status);
  if(WIFEXITED(status))
     printf("\nChild ended with code %d\n", WEXITSTATUS(status));
  else
     printf("\nChild ended abnormally\n");
}