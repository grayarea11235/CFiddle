// Testing of new C++ features take 1
//
// An example simple object with vsarious constructors and how they work
//

#include <new>
#include <iostream>

class Calc
{
private:
  int count;
public:
  Calc() { };
  
  int add(int x, int y);
};

int Calc::add(int x, int y)
{
  return x + y;
}


int main(int argc, char *argv[])
{
  auto calc = new Calc();

  
  //  std::cou
  
  return 0;
}
