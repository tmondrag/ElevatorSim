#include "personqueue.h"
#include <iostream>

PersonQueue::PersonQueue()
{
  pnode_t * tmp = new pnode_t;
  tmp->next = NULL;
  head = tail = tmp;
  pthread_mutex_init(&headLock,NULL);
  pthread_mutex_init(&tailLock,NULL);
}

void PersonQueue::enqueue(Person * p)
{
  pnode_t * tmp = new pnode_t;
  tmp->value = p;
  tmp->next = NULL;

  pthread_mutex_lock(&tailLock);
  tail->next = tmp;
  tail = tmp;
  pthread_mutex_unlock(&tailLock);
}

Person * PersonQueue::dequeue()
{
  pthread_mutex_lock(&headLock);
  pnode_t * temp = head;
  pnode_t * newHead = temp->next;
  if(newHead == NULL)
  {
    pthread_mutex_unlock(&headLock);
    return NULL; // Queue was empty
  }
  Person * tempP = newHead->value;
  head = newHead;
  pthread_mutex_unlock(&headLock);
  delete temp;
  return tempP;
}

bool PersonQueue::isEmpty()
{
  pthread_mutex_lock(&headLock);
  pnode_t * temp = head;
  pnode_t * newHead = temp->next;
  if(newHead == NULL)
  {
    pthread_mutex_unlock(&headLock);
    return true; // Queue is empty
  }
  else
  {
    pthread_mutex_unlock(&headLock);
    return false;
  }
}

PersonQueue::~PersonQueue()
{
  pthread_mutex_lock(&headLock);
  Person * temp = head->next->value;
  pthread_mutex_unlock(&headLock);
  while(temp != NULL)
    temp = dequeue();
}

PersonStack::PersonStack()
{
  pnode_t * tmp = new pnode_t;
  tmp->next = NULL;
  head = /*tail =*/ tmp;
  pthread_mutex_init(&headLock,NULL);
  //pthread_mutex_init(&tailLock,NULL);
}

void PersonStack::push(Person * p)
{
  pnode_t * tmp = new pnode_t;
  tmp->value = p;

  pthread_mutex_lock(&headLock);
  tmp->next = head->next->next;
  head->next = tmp;
  pthread_mutex_unlock(&headLock);
}

Person * PersonStack::pop()
{
  pthread_mutex_lock(&headLock);
  pnode_t * temp = head;
  pnode_t * newHead = temp->next;
  if(newHead == NULL)
  {
    pthread_mutex_unlock(&headLock);
    return NULL; // Stack was empty
  }
  Person * tempP = newHead->value;
  head = newHead;
  pthread_mutex_unlock(&headLock);
  delete temp;
  return tempP;
}

bool PersonStack::isEmpty()
{
  pthread_mutex_lock(&headLock);
  pnode_t * temp = head;
  pnode_t * newHead = temp->next;
  if(newHead == NULL)
  {
    pthread_mutex_unlock(&headLock);
    return true; // Stack is empty
  }
  else
  {
    pthread_mutex_unlock(&headLock);
    return false;
  }
}

PersonStack::~PersonStack()
{
  pthread_mutex_lock(&headLock);
  Person * temp = head->next->value;
  pthread_mutex_unlock(&headLock);
  while(temp != NULL)
    temp = pop();
}
