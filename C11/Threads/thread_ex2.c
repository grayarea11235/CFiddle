#include <stdio.h>
#include <threads.h>

#define NUM_THREADS 7

static int threadData[NUM_THREADS];

int threadFunction(void * data) {
		    printf("%d-th thread up\n", *(int*)data);
			    return 0;
}

int main(void) {
		    thrd_t threadId[NUM_THREADS];

			    // init thread data
				for (int i=0; i < NUM_THREADS; ++i)
				threadData[i] = i;
				
				// start NUM_THREADS amount of threads
				                     for (int i=0; i < NUM_THREADS; ++i) {
				                             if (thrd_create(threadId+i, threadFunction, threadData+i) != thrd_success) {
				                                         printf("%d-th thread create error\n", i);
				                                                     return 0;
				                                                            }
				                                                                 }
				
				                                                                     // wait until all threads terminates
				                                                                         for (int i=0; i < NUM_THREADS; ++i)
				                                                                                 thrd_join(threadId[i], NULL);
				
				                                                                                     return 0;
				                                                                                     }
