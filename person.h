#ifndef PERSON_H
#define PERSON_H

#include <ctime>
#include <string>

class Person
{
private:
  time_t startTime;
public:
  std::string firstName;
  std::string lastName;
  int startFloor;
  int destination;
  Person();
  ~Person();
  time_t elapsedTime();
};

#endif // PERSON_H
