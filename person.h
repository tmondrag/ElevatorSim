#ifndef PERSON_H
#define PERSON_H

#include <iostream>
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
  int currfloor;
  int destination;
  Person(std::string first, std::string last, int start, int destiny);
  ~Person();
  double elapsedTime();
};

#endif // PERSON_H
