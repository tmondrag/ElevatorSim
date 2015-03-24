// -- Test for the person class -- //

#include "person.h"

int main(int argc,char* argv[])
{
  long int i = 0;
  Person p1("Spple","mazzy",3,9);

  while(i < 10000000)
      i++;
  p1.currfloor(6);
}
