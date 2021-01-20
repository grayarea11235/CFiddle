#include <stdio.h>
#include <pthread.h>


void *thread_func(void *ptr)
{
  printf("Hi from the thread"\n);

  return NULL;
}


int main(int argc, char *argv[])
{
  if (pthread_create(&thread_func, NULL, thread_func, NULL))
  {
    printf("Thread created\n");
  }


  return 0;
}
