#include "person.h"


Person::Person(std::string first,std::string last,int start,int destiny)
    :firstName(first),lastName(last),startFloor(start),currfloor(start),
      destination(destiny)
{
  startTime = time(NULL);
  std::cout << firstName << ' ' << lastName << " has arrived on floor " << startFloor;
  std::cout << std::endl;
}

Person::~Person()
{
  if(currfloor == destination)
  {
    std::cout << firstName << ' ' << lastName << " ended up on floor " << currfloor;
    std::cout << " after " << elapsedTime() << " seconds." << std::endl;
  }
  else
  {
    std::cout << firstName << ' ' << lastName << " ended up on floor " << currfloor;
    std::cout << " after " << elapsedTime() << " seconds." << std::endl;
    std::cout << firstName << ' ' << lastName << " wanted to be on floor ";
    std::cout << destination << "." << std::endl;
  }
}

double Person::elapsedTime()
{
  return difftime(startTime,time(NULL));
}
