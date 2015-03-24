#ifndef PERSON_H
#define PERSON_H

#include <iostream>
#include <ctime>
#include <string>
#include <pthread.h>

class Person
{
private:
  time_t startTime;
  std::string _firstName;
  std::string _lastName;
  int _startFloor;
  int _currfloor;
  int _destination;
  pthread_mutex_t* printlock;
public:
  Person(std::string first, std::string last, int start, int destiny);
  Person(std::string first, std::string last, int start, int destiny, pthread_mutex_t * print);
  ~Person();
  std::string firstName();
  std::string lastName();
  double elapsedTime();
  int startfloor();
  int destination();
  void currfloor(int floor);
};

#endif // PERSON_H
