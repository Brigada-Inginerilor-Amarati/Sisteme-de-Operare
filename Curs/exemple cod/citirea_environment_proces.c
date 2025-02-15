#include <stdio.h>
extern char **environ;

int main(int argc, char *argv[])
{   
   char **p;
   p = environ;
   while(*p)
   {	
        printf("%s\n", *p);
        p++;
   }
   
   printf("\n\n----------------\n\n");
   /**
    * acelasi cod se poate scrie, echivalent, astfel:
    */
   for(p=environ; *p; p++)
     printf("%s\n", *p);
}