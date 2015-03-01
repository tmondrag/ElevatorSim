#include "personqueue.h"
#include <iostream>
#include <assert.h>

PersonQueue::PersonQueue()
{
  int rc;
  pnode_t * tmp = new pnode_t;
  assert(tmp != NULL);
  tmp->next = NULL;
  head = tail = tmp;
  rc = pthread_mutex_init(&headLock,NULL);
  assert(rc == 0);
  rc = pthread_mutex_init(&tailLock,NULL);
  assert(rc == 0);
}

void PersonQueue::enqueue(Person * p)
{
  int rc;
  pnode_t * tmp = new pnode_t;
  assert(tmp != NULL);
  tmp->value = p;
  tmp->next = NULL;

  rc = pthread_mutex_lock(&tailLock);
  assert(rc == 0);
  tail->next = tmp;
  tail = tmp;
  rc = pthread_mutex_unlock(&tailLock);
  assert(rc == 0);
}

Person * PersonQueue::dequeue()
{
  int rc;
  rc = pthread_mutex_lock(&headLock);
  assert(rc == 0);
  pnode_t * temp = head;
  pnode_t * newHead = temp->next;
  if(newHead == NULL)
  {
    rc = pthread_mutex_unlock(&headLock);
    assert(rc == 0);
    return NULL; // Queue was empty
  }
  Person * tempP = newHead->value;
  head = newHead;
  rc = pthread_mutex_unlock(&headLock);
  assert(rc == 0);
  delete temp;
  return tempP;
}

bool PersonQueue::isEmpty()
{
  int rc;
  rc = pthread_mutex_lock(&headLock);
  assert(rc == 0);
  pnode_t * temp = head;
  pnode_t * newHead = temp->next;
  if(newHead == NULL)
  {
    rc = pthread_mutex_unlock(&headLock);
    assert(rc == 0);
    return true; // Queue is empty
  }
  else
  {
    rc = pthread_mutex_unlock(&headLock);
    assert(rc == 0);
    return false;
  }
}

PersonQueue::~PersonQueue()
{
  while(!isEmpty())
    delete dequeue();
}

PersonStack::PersonStack()
{
  int rc;
  pnode_t * tmp = new pnode_t;
  tmp->next = NULL;
  head = /*tail =*/ tmp;
  rc = pthread_mutex_init(&headLock,NULL);
  assert(rc == 0);
  //pthread_mutex_init(&tailLock,NULL);
}

void PersonStack::push(Person * p)
{
  int rc;
  pnode_t * tmp = new pnode_t;
  tmp->value = p;

  rc = pthread_mutex_lock(&headLock);
  assert(rc == 0);
  tmp->next = head;
  head = tmp;
  rc = pthread_mutex_unlock(&headLock);
  assert(rc == 0);
}

Person * PersonStack::pop()
{
  int rc = pthread_mutex_lock(&headLock);
  assert(rc == 0);
  pnode_t * temp = head;
  pnode_t * newHead = temp->next;
  if(newHead == NULL)
  {
    rc = pthread_mutex_unlock(&headLock);
    assert(rc == 0);
    return NULL; // Stack was empty
  }
  Person * tempP = newHead->value;
  head = newHead;
  rc = pthread_mutex_unlock(&headLock);
  assert(rc == 0);
  delete temp;
  return tempP;
}

bool PersonStack::isEmpty()
{
  int rc = pthread_mutex_lock(&headLock);
  assert(rc == 0);
  pnode_t * temp = head;
  pnode_t * newHead = temp->next;
  if(newHead == NULL)
  {
    rc = pthread_mutex_unlock(&headLock);
    assert(rc == 0);
    return true; // Stack is empty
  }
  else
  {
    rc = pthread_mutex_unlock(&headLock);
    assert(rc == 0);
    return false;
  }
}

PersonStack::~PersonStack()
{
  while(!isEmpty())
    delete pop();
}
