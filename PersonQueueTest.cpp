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

//cond_t qfill, sfill;
//mutex_t qmutex, smutex;
PersonQueue floor;
PersonStack elevator;

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

/*void * queueProducer(void * arg)
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
}*/

int main(int argc, char *argv[])
{
  assert(argc > 0);
  int loops = StringToInt(argv[1]);
  int i = 0;
  Person * people[loops];
  std::string first = "First Name";
  std::string last = "person";
  for(i = 0; i < loops; i++)
  {

    people[i] = new Person(first,last+NumberToString(i),1,1);
    floor.enqueue(people[i]);
    elevator.push(floor.dequeue());
    elevator.pop();
    delete people[i];
  }
    return 0;
}

