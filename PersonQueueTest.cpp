//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Test for classes in personqueue.h
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#include "personqueue.h"
#include <pthread.h>
#include <assert.h>
#include <string>
#include <sstream>
#include <cstdlib>

//buffers are infinite, producer won't have to wait for open spot
//main thread will produce persons and stick them in queue
//elevator thread consume from queue and produce to stack
//cleanup thread will consume from stack

pthread_cond_t qfill, sfill;
pthread_mutex_t qmutex, smutex;
int qcount,scount;
PersonQueue * floor;
PersonStack * elevator;

template <typename T>
  std::string NumberToString ( T Number )
  {
    std::ostringstream ss;
    ss << Number;
    return ss.str();
  }

int StringToInt(std::string s)
{
  int x;
  std::stringstream str(s);
  str >> x;
  return x;
}

int StringToInt(char* s)
{
  int x;
  std::stringstream str(s);
  str >> x;
  return x;
}

/*void * populatr(void * arg)
{
  int i, rc;
  for (i = 0; i < loops; i++)
  {
    rc = pthread_mutex_lock(&qmutex); // p1
    assert(rc == 0);



  put(i); // p4
  Pthread_cond_signal(&fill); // p5
  Pthread_mutex_unlock(&mutex); // p6
  }
}
*/
void * elevatorLoader(void * arg)
{
  /*int i;
  for (i = 0; i < loops; i++)
  {
    Pthread_mutex_lock(&mutex); // c1
    while (count == 0) // c2
      Pthread_cond_wait(&fill, &mutex); // c3
    int tmp = get(); // c4
    Pthread_cond_signal(&empty); // c5
    Pthread_mutex_unlock(&mutex); // c6
    printf("%d\n", tmp);
  }*/
  return NULL;
}

void * elevatorUnloader(void * arg)
{
  return NULL;
}


int main(int argc, char *argv[])
{
  assert(argc > 0);
  int loops = StringToInt(argv[1]);
  int i = 0;
  int rc;
  Person * people[1];
  std::string first = "First Name";
  std::string last = "person";

  elevator = new PersonStack;
  floor = new PersonQueue;

  // -- single thread test -- //
  for(i = 0; i < loops; i++)
  {
    people[0] = new Person(first,last+NumberToString(i),1,1);
    floor->enqueue(people[0]);
    elevator->push(floor->dequeue());
    elevator->pop();
    delete people[0];
  }

  // -- single stack and queue, multiple threads -- //
  pthread_t threads[3];
  rc = pthread_mutex_init(&smutex,NULL);assert(rc == 0);
  rc = pthread_mutex_init(&qmutex,NULL);assert(rc == 0);
  scount = qcount = 0;
  rc = pthread_cond_init(&sfill,NULL);assert(rc == 0);
  rc = pthread_cond_init(&qfill,NULL);assert(rc == 0);

  rc = pthread_create(&threads[0],NULL,elevatorLoader,NULL);assert(rc == 0);
  rc = pthread_create(&threads[1],NULL,elevatorUnloader,NULL);assert(rc == 0);

  for(i = 0; i < loops; i++)
  {
    people[0] = new Person(first+NumberToString(10-i),last+NumberToString(i),1,1);
    delete people[0];
  }

  rc = pthread_join(threads[0],NULL);assert(rc == 0);
  rc = pthread_join(threads[1],NULL);assert(rc == 0);

  delete floor;
  delete elevator;
  return 0;
}

