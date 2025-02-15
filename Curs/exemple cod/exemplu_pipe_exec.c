#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>


int pfd[2];


int main(int argc, char *argv[])
{
  int pid_a, pid_b;
    
  if(argc != 3)
  {
    printf("Utilizare: %s prog1 prog2\n", argv[0]);
    exit(1);
  }

  if(pipe(pfd)<0)
  {
    printf("Eroare la crearea pipe-ului\n");
    exit(1);
  }

  if((pid_a=fork())<0)
  {
    printf("Eroare la fork\n");
    exit(1);
  }
  if(pid_a==0) /* a */
  {
    close(pfd[0]);
    
    dup2(pfd[1],1);

    execlp(argv[1], argv[1], NULL);
    printf("Eroare la exec\n");
    exit(1);
  }
  
  /* parinte */
  
  if((pid_b=fork())<0)
  {
    printf("Eroare la fork\n");
    exit(1);
  }
  
  if(pid_b==0) /* b */
  {
    close(pfd[1]);
    
    dup2(pfd[0],0);

    execlp(argv[2], argv[2], NULL);
    printf("Eroare la exec\n");
    exit(1);
  }
  
  /* b */
  close(pfd[0]); 
  close(pfd[1]); 
     
  /* Procesul parinte preia rezultatele */
  
  int status;
  
  waitpid(pid_a, &status, 0);
  
  waitpid(pid_b, &status, 0);
  
  /*varianta simplificata de tratare a valorii de return*/
  if(WIFEXITED(status))
    return WEXITSTATUS(status);
  else
    return 1;
  
  return 0;
}