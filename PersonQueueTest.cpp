//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Test for classes in personqueue.h
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#include "personqueue.h"
#include <pthread.h>
#include <assert.h>
#include <string>
#include <sstream>
#include <cstdlib>
#include <iostream>

//buffers are infinite, producer won't have to wait for open spot
//main thread will produce persons and stick them in queue
//elevator thread consume from queue and produce to stack
//cleanup thread will consume from stack

int mult,loops;
PersonQueue * floor;
PersonStack * elevator;

std::string first = "Timmy ";
std::string last = "Lounds ";

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
  int i;
  long int tid = (long int)arg;
  Person * tmp;
  for(i = 0; i < loops/mult; i++)
  {
    tmp = new Person(first+NumberToString(tid),last+NumberToString(i),1,2);
    floor->enqueue(tmp);
  }
  return NULL;
}

void * elevatorLoader(void * arg)
{
  int i;
  Person * tmp;
  for (i = 0; i < loops/mult; i++)
  {
    tmp = floor->dequeue(); // c4
    elevator->push(tmp);
  }

  return NULL;
}

void * elevatorUnloader(void * arg)
{
    int i;
    Person * tmp;
    for (i = 0; i < loops/mult; i++)
    {
      tmp = elevator->pop();
      tmp->currfloor(tmp->destination());
      delete tmp;
    }
  return NULL;
}


int main(int argc, char *argv[])
{
  long int i = 0;
  int rc;
  Person * tempPerson;

  if(argc < 2)
  {
    std::cout << "Usage: PersonQueueTest [num people] [thread multiple]" << std::endl;
    return 1;
  }
  loops = StringToInt(argv[1]);
  if(argc > 2)
  {
    mult = StringToInt(argv[2]);
    if(mult < 1)
      mult = 1;
  }
  else
    mult = 1;

  elevator = new PersonStack();
  floor = new PersonQueue();

  std::cout << "\nsingle thread test\n" << std::endl;
  for(i = 0; i < loops; i++)
  {
    tempPerson = new Person(first,last+NumberToString(i),1,2);
    floor->enqueue(tempPerson);
  }
  for(i = 0; i < loops; i++)
  {
    tempPerson = floor->dequeue();
    elevator->push(tempPerson);
    tempPerson = elevator->pop();
    tempPerson->currfloor(2);
    delete tempPerson;
  }

  std::cout << "\nsingle stack and queue, multiple threads\n" << std::endl;
  pthread_t threads[3*mult];
  for(i = 0; i < mult; i++)
  {
  rc = pthread_create(&threads[0+3*i],NULL,populator,(void*) i);assert(rc == 0);
  rc = pthread_create(&threads[1+3*i],NULL,elevatorLoader,NULL);assert(rc == 0);
  rc = pthread_create(&threads[2+3*i],NULL,elevatorUnloader,NULL);assert(rc == 0);
  }

  /*for(i = 0; i < loops; i++)
  {
    tempPerson = new Person(first+NumberToString(10-i),last+NumberToString(i),1,1);
    delete tempPerson;
  }*/

  for(i = 0; i < mult; i++)
  {
  rc = pthread_join(threads[0+3*i],NULL);assert(rc == 0);
  rc = pthread_join(threads[1+3*i],NULL);assert(rc == 0);
  rc = pthread_join(threads[2+3*i],NULL);assert(rc == 0);
  }

  delete floor;
  delete elevator;
  return 0;
}

