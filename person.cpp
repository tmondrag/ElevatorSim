#include "person.h"
#include <assert.h>

static pthread_mutex_t printlock_default = PTHREAD_MUTEX_INITIALIZER;
//printlock is nescessary to keep printed outputs looking pretty
//this only keeps people from shouting over eachothers' head, but later elevators will talk too
//a building wide printlock might be necessary

Person::Person(std::string first,std::string last,int start,int destiny)
    :_firstName(first),_lastName(last),_startFloor(start),_currfloor(start),
      _destination(destiny)
{
  printlock = &printlock_default;
  startTime = time(NULL);
  int rc;rc = pthread_mutex_lock(printlock);assert(rc==0);
  std::cout << _firstName << ' ' << _lastName << " has arrived on floor " << _startFloor;
  std::cout << std::endl;
  rc = pthread_mutex_unlock(printlock);assert(rc==0);
}

Person::Person(std::string first,std::string last,int start,int destiny,pthread_mutex_t * print)
    :_firstName(first),_lastName(last),_startFloor(start),_currfloor(start),
      _destination(destiny),printlock(print)
{
  startTime = time(NULL);
  int rc;rc = pthread_mutex_lock(printlock);assert(rc==0);
  std::cout << _firstName << ' ' << _lastName << " has arrived on floor " << _startFloor;
  std::cout << std::endl;
  rc = pthread_mutex_unlock(printlock);assert(rc==0);
}

Person::~Person()
{
  int rc;
  if(_currfloor == _destination)
  {
    rc = pthread_mutex_lock(printlock);assert(rc==0);
    std::cout << _firstName << ' ' << _lastName << " ended up on floor " << _currfloor;
    std::cout << " after " << elapsedTime() << " seconds." << std::endl;
    rc = pthread_mutex_unlock(printlock);assert(rc==0);
  }
  else
  {
    rc = pthread_mutex_lock(printlock);assert(rc==0);
    std::cout << _firstName << ' ' << _lastName << " ended up on floor " << _currfloor;
    std::cout << " after " << elapsedTime() << " seconds." << std::endl;
    std::cout << _firstName << ' ' << _lastName << " wanted to be on floor ";
    std::cout << _destination << "." << std::endl;
    rc = pthread_mutex_unlock(printlock);assert(rc==0);
  }
}


std::string Person::firstName()
{
  return _firstName;
}

std::string Person::lastName()
{
  return _lastName;
}

double Person::elapsedTime()
{
  return 0-difftime(startTime,time(NULL));
}

int Person::destination()
{
  return _destination;
}

int Person::startfloor()
{
  return _startFloor;
}

void Person::currfloor(int floor)
{
  _currfloor = floor;
}
