/*
 * (c) 2013 Dan C. Cosma
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h> 
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define BUFSIZE 4096

void usage(char *name)
{
  printf("Usage: %s <source> <destination>\n", name);
}

int main(int argc, char *argv[])
{
  int fd1, fd2;
  int n;
  char buf[BUFSIZE];
  
  /*** Verificarea argumentelor din linia de comanda */
  if(argc!=3)
    {
      usage(argv[0]);
      exit(1);
    }

  /*** Deschiderea fisierelor */
  if((fd1=open(argv[1], O_RDONLY))<0)
    {
      printf("Error opening input file\n");
      exit(2);
    }
  if((fd2=open(argv[2], O_WRONLY | O_CREAT | O_EXCL, S_IRWXU)) < 0)
    {
      printf("Error creating destination file\n");
      exit(3);
    }
  
  /*** Copierea propriu-zisa */
  while((n = read(fd1, buf, BUFSIZE)) > 0)
    {
      if(write(fd2, buf, n) < 0)
	{
	  printf("Error writing to file\n");
	  exit(4);
	}
    }

  if(n < 0)
    {
      printf("Error reading from file\n");
      exit(5);
    }
  
  /*** Inchiderea fisierelor */
  close(fd1);
  close(fd2);
  
  return 0;
}