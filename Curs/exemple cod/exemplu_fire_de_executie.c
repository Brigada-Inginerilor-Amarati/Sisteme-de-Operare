#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *thread_code(void *arg)
{
    int i;
   
    for(i=0; i<1000; i++)
     printf("%s", (char *)arg);
    printf("\n");

    return (void *)( *((char *)arg) - 'A' + 1);
}

int main(int argc, char *argv[])
{
   pthread_t th1, th2;
   void *ret1, *ret2; 
   
   pthread_create( &th1, NULL, thread_code, (void*) "A");
   pthread_create( &th2, NULL, thread_code, (void*) "B");
   
   printf("Threads created.\n");
   
   pthread_join(th1, &ret1);
   pthread_join(th2, &ret2); 
   
   printf("Thread 1 ends returning: %d.\n", (int)ret1);
   printf("Thread 2 ends returning: %d.\n", (int)ret2);
   exit(0);
   
   return 0;
}
