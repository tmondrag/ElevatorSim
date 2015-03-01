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
int qcount,scount,loops;
PersonQueue * floor;
PersonStack * elevator;

std::string first = "First Name";
std::string last = "person";

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

void * populator(void * arg)
{
  int i,rc;
  Person * tmp;
  for(i = 0; i < loops; i++)
  {
    tmp = new Person(first,last+NumberToString(i),1,2);
    floor->enqueue(tmp);
    rc = pthread_mutex_lock(&qmutex);assert(rc == 0);
    //in the multifloor version, the queue's own mutex and condition should be used?
    //no waiting because infinite buffer
    qcount++;
    rc = pthread_cond_signal(&qfill);assert(rc == 0);
    int rc = pthread_mutex_unlock(&qmutex);assert(rc == 0);
  }
  return NULL;
}

void * elevatorLoader(void * arg)
{
  int i,rc,l;
  Person * tmp;
  for (i = 0; i < loops; i++)
  {
    rc = pthread_mutex_lock(&qmutex);assert(rc == 0); // c1
    while (qcount == 0) // c2
    {
      rc = pthread_cond_wait(&qfill, &qmutex);assert(rc == 0); // c3
    }//huh... maybe I should incorporate this into dequeue instead?
    tmp = floor->dequeue(); // c4
    qcount--; // c5
    if(qcount > 0)
    {
      rc = pthread_cond_signal(&qfill);assert(rc == 0);
    }
    rc = pthread_mutex_unlock(&qmutex);assert(rc == 0); // c6

    elevator->push(tmp);
    rc = pthread_mutex_lock(&smutex);assert(rc == 0);
    //in the multifloor version, the stack's own mutex and condition should be used?
    //no waiting because infinite buffer
    //std::cout<<tmp->firstName<<" "<<tmp->lastName<<" gets on the elevator"<<std::endl;
    //for(l=0;l<1000000;l++)
    //    rc=rc+l;
    scount++;
    rc = pthread_cond_signal(&sfill);assert(rc == 0);
    rc = pthread_mutex_unlock(&smutex);assert(rc == 0);
  }

  return NULL;
}

void * elevatorUnloader(void * arg)
{
    int i,rc;
    Person * tmp;
    for (i = 0; i < loops; i++)
    {
      rc = pthread_mutex_lock(&smutex);assert(rc == 0); // c1
      while (scount == 0) // c2
      {
        rc = pthread_cond_wait(&sfill, &smutex);assert(rc == 0); // c3
      }//huh... maybe I should incorporate this into pop instead?
      tmp = elevator->pop(); // c4
      scount--; // c5
      if(scount > 0)
      {
        rc = pthread_cond_signal(&sfill);assert(rc == 0);
      }
      tmp->currfloor=tmp->destination;
      delete tmp;
      rc = pthread_mutex_unlock(&smutex);assert(rc == 0); // c6
    }
  return NULL;
}


int main(int argc, char *argv[])
{
  assert(argc > 0);
  loops = StringToInt(argv[1]);
  int i = 0;
  int rc;
  Person * tempPerson;

  elevator = new PersonStack();
  floor = new PersonQueue();

  // -- single thread test -- //
  /*for(i = 0; i < loops; i++)
  {
    tempPerson = new Person(first,last+NumberToString(i),1,2);
    floor->enqueue(tempPerson);

  }
  for(i = 0; i < loops; i++)
  {
    tempPerson = floor->dequeue();
    elevator->push(tempPerson);
    tempPerson = elevator->pop();
    tempPerson->currfloor = 2;
    delete tempPerson;
  }*/

  // -- single stack and queue, multiple threads -- //
  pthread_t threads[3];
  rc = pthread_mutex_init(&smutex,NULL);assert(rc == 0);
  rc = pthread_mutex_init(&qmutex,NULL);assert(rc == 0);
  scount = qcount = 0;
  rc = pthread_cond_init(&sfill,NULL);assert(rc == 0);
  rc = pthread_cond_init(&qfill,NULL);assert(rc == 0);

  rc = pthread_create(&threads[0],NULL,populator,NULL);assert(rc == 0);
  rc = pthread_create(&threads[1],NULL,elevatorLoader,NULL);assert(rc == 0);
  rc = pthread_create(&threads[2],NULL,elevatorUnloader,NULL);assert(rc == 0);

  /*for(i = 0; i < loops; i++)
  {
    tempPerson = new Person(first+NumberToString(10-i),last+NumberToString(i),1,1);
    delete tempPerson;
  }*/

  rc = pthread_join(threads[0],NULL);assert(rc == 0);
  rc = pthread_join(threads[1],NULL);assert(rc == 0);
  rc = pthread_join(threads[2],NULL);assert(rc == 0);

  delete floor;
  delete elevator;
  return 0;
}

