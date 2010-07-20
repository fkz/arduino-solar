#include <WProgram.h>
#include "solarboot.h"


extern "C" void __cxa_pure_virtual () { for (;;); }

//verify, that sizeof(int)=2. If this is not the case
// this will lead to a compile time error
int COMPILE_TIME_ASSERT_INT[(sizeof(int)==2)*2-1];

int main ()
{
  init ();
  
  Solarboot solarboot;
  
  while (true)
    solarboot.call();
  
}