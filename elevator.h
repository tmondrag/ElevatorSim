#ifndef ELEVATOR_H
#define ELEVATOR_H

#include <vector>
#include "personqueue.h"

class Elevator
{
private:
  std::vector<PersonStack> passengers;
  int passengerCount;
  int currFloor;
public:
  Elevator();
  ~Elevator();
  void openDoors();
  void closeDoors();
  void loadPassengers();
  void unloadPassengers();
  void move();
};

#endif // ELEVATOR_H
