#include "personqueue.h"
#include <iostream>
#include <assert.h>

PersonQueue::PersonQueue()
{
  qcount = 0;
  int rc;
  pnode_t * tmp = new pnode_t;
  assert(tmp != NULL);
  tmp->next = NULL;
  head = tail = tmp;
  rc = pthread_mutex_init(&headLock,NULL);
  assert(rc == 0);
  rc = pthread_mutex_init(&tailLock,NULL);
  assert(rc == 0);
  rc = pthread_cond_init(&qfill,NULL);
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
  rc = pthread_mutex_lock(&headLock);
  assert(rc == 0);
  qcount++;
  rc = pthread_cond_signal(&qfill);
  assert(rc == 0);
  rc = pthread_mutex_unlock(&headLock);
  assert(rc == 0);
}

Person * PersonQueue::dequeue()
{
  int rc;
  rc = pthread_mutex_lock(&headLock);
  assert(rc == 0);
  while (qcount == 0) // c2
  {
    rc = pthread_cond_wait(&qfill, &headLock);
    assert(rc == 0); // c3
  }
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
  qcount--; // c5
  if(qcount > 0)
  {
    rc = pthread_cond_signal(&qfill);assert(rc == 0);
  }
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
  delete tail;
}

PersonStack::PersonStack()
{
  scount = 0;
  int rc;
  pnode_t * tmp = new pnode_t;
  assert(tmp != NULL);
  tmp->next = NULL;
  head = tmp;
  rc = pthread_mutex_init(&headLock,NULL);
  assert(rc == 0);
  rc = pthread_cond_init(&sfill,NULL);
  assert(rc == 0);
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
  scount++;
  rc = pthread_cond_signal(&sfill);
  assert(rc == 0);
  rc = pthread_mutex_unlock(&headLock);
  assert(rc == 0);
}

Person * PersonStack::pop()
{
  int rc = pthread_mutex_lock(&headLock);
  assert(rc == 0);
  while (scount == 0) // c2
  {
    rc = pthread_cond_wait(&sfill, &headLock);
    assert(rc == 0); // c3
  }
  pnode_t * temp = head;
  pnode_t * newHead = temp->next;
  if(newHead == NULL)
  {
    rc = pthread_mutex_unlock(&headLock);
    assert(rc == 0);
    return NULL; // Stack was empty
  }
  Person * tempP = temp->value;
  head = newHead;
  scount--; // c5
  if(scount > 0)
  {
    rc = pthread_cond_signal(&sfill);
    assert(rc == 0);
  }
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
  delete head;
}
