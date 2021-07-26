//#include <iostream>
#include <octave/octave.h>

int main(int argc,char* argv)
{
  int embedded;
  octave_main(argc,argv,embedded=0);  
  return embedded;
}

