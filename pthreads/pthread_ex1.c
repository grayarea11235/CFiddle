#include <stdio.h>
#include <pthread.h>

void *thread_func(void *ptr)
{
  printf("Chickens\n");

  return NULL;
}

int main(int argc, char *argv[])
{
  pthread_t the_thread;

  if (pthread_create(&the_thread, NULL, thread_func, NULL))
  {
    printf("Thread creation failed!\n");
  }

  if(pthread_join(the_thread, NULL)) 
  {
    fprintf(stderr, "Error joining thread\n");
    return 2;
  }
  else
  {
    printf("Thread joined\n");
  }

  return 0;
}
