//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Test for classes in personqueue.h
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#include "personqueue.h"
#include <pthread.h>

cond_t empty, fill;
mutex_t mutex;

void * producer(void * arg)
{
  int i;
  for (i = 0; i < loops; i++)
  {
  Pthread_mutex_lock(&mutex); // p1
  while (count == MAX) // p2
  Pthread_cond_wait(&empty, &mutex); // p3
  put(i); // p4
  Pthread_cond_signal(&fill); // p5
  Pthread_mutex_unlock(&mutex); // p6
  }
}

void * consumer(void * arg)
{
  int i;
  for (i = 0; i < loops; i++)
  {
    Pthread_mutex_lock(&mutex); // c1
    while (count == 0) // c2
      Pthread_cond_wait(&fill, &mutex); // c3
    int tmp = get(); // c4
    Pthread_cond_signal(&empty); // c5
    Pthread_mutex_unlock(&mutex); // c6
    printf("%d\n", tmp);
  }
}
