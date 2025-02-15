#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

int pipe_ac[2], pipe_bc[2], results_pipe[2];

enum processes { COUNTER, MAXRANDOM };
struct result_data {
  enum processes type;
  long data;
};

void process_c()
{
   int count = 0, n1, n2;
   long num;
   long max = LONG_MIN;
   int end1=0, end2=0;
   char c;
   struct result_data res;
   
   close(pipe_ac[1]);
   close(pipe_bc[1]);
   close(results_pipe[0]);
   
   
   while( !(end1 && end2) )
    {
      if(!end1)
      {
         if((n1=read(pipe_ac[0], &c, sizeof(char)))<0)
         {
           printf("Eroare la citire din pipe a-c\n");
           exit(1);
         }
      
         if(n1 >0)
         {
           if(islower(c)) 
             count++;
         }
         else
	 {	  
	    res.type = COUNTER;
	    res.data = count;

	    if(write(results_pipe[1], &res, sizeof(struct result_data))<0)
	      {
		 printf("Eroare la scriere in pipe rezultate\n");
		 exit(1);
	      }    
	    end1=1;
	 }
      }
       
      
      if(! end2) 
      {
        if((n2=read(pipe_bc[0], &num, sizeof(long)))<0)
        {
          printf("Eroare la citire din pipe b-c\n");
          exit(1);
        }
        if(n2>0)
        {
          if(num%2 == 0)
          {
            if(num >= max) 
              max = num;
          }
        }
        else
	{
	   res.type = MAXRANDOM;
	   res.data=max;
	   
	   if(write(results_pipe[1], &res, sizeof(struct result_data))<0)
	   {
	      printf("Eroare la scriere in pipe rezultate\n");
	      exit(1);
	   }	      
	   end2=1;
	}
	 
      }  
    } /* while */

    close(pipe_ac[0]);
    close(pipe_bc[0]); 
    close(results_pipe[1]);       
}


int main(int argc, char *argv[])
{
  int pid_a, pid_c;
  
  
  if(argc != 3)
  {
    printf("Utilizare: %s nr_linii fisier\n", argv[0]);
    exit(1);
  }

  if(pipe(results_pipe)<0)
  {
    printf("Eroare la crearea pipe-ului\n");
    exit(1);
  }

  if(pipe(pipe_ac)<0)
  {
    printf("Eroare la crearea pipe-ului a-c\n");
    exit(1);
  }
  
  if((pid_a=fork())<0)
  {
    printf("Eroare la fork\n");
    exit(1);
  }
  if(pid_a==0) /* a */
  {
    close(pipe_ac[0]);
    close(results_pipe[0]); close(results_pipe[1]);
    
    dup2(pipe_ac[1],1);

    execlp("tail", "tail", "-n", argv[1], argv[2], NULL);
    printf("Eroare la exec\n");
    exit(1);
  }
  
  /* b */
  if(pipe(pipe_bc)<0)
  {
    printf("Eroare la crearea pipe-ului b-c\n");
    exit(1);
  }
  
  if((pid_c=fork())<0)
  {
    printf("Eroare la fork\n");
    exit(1);
  }
  
  if(pid_c==0) /* c */
  {
    process_c();
    exit(0);
  }
  
  /* b */
  close(pipe_bc[0]); 
  close(pipe_ac[0]); close(pipe_ac[1]);
  close(results_pipe[1]);
  
  unsigned int i;
  long num;
  srand((int)time(NULL));
  for(i=0; i<100; i++)
  {
    num = 3*rand();
    if(write(pipe_bc[1], &num, sizeof(long))<0)
    {
      printf("Eroare la scriere in pipe b-c\n");
      exit(1);
    }
  }
  
  close(pipe_bc[1]); /* important sa fie aici */
   
  /* Procesul parinte (b) preia rezultatele */
  struct result_data res;
  
  for(i=0; i<2; i++)
  {
    if(read(results_pipe[0], &res, sizeof(struct result_data))<0)
     {
      printf("Eroare la citire din pipe rezultate\n");
      exit(1);
     }
    printf("Rezultat primit: %s = %ld\n", res.type==COUNTER ? "Nr. litere mici" : "Numar generat maxim", res.data);
  }
  
  
  close(results_pipe[0]); 

  int s;
  
  wait(&s); wait(&s); /* preluarea starii fiilor - in mod normal ar trebui facuta aici si verificarea starilor de return */
  
  return 0;
}