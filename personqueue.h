#ifndef PERSONQUEUE_H
#define PERSONQUEUE_H

#include "person.h"
#include <pthread.h>

typedef struct pnode_t
{
  Person * value;
  struct pnode_t * next;
} __pnode_t;

class PersonQueue
{
private:
  pnode_t * head;
  pnode_t * tail;
  pthread_mutex_t headLock;
  pthread_mutex_t tailLock;
public:
  PersonQueue();
  ~PersonQueue();
  void enqueue(Person * p);
  Person * dequeue();
  bool isEmpty();
};

class PersonStack
{
private:
  pnode_t * head;
  pthread_mutex_t headLock;
public:
  PersonStack();
  ~PersonStack();
  void push(Person * p);
  Person * pop();
  bool isEmpty();
};

#endif // PERSONQUEUE_H
