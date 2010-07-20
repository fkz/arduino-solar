#include "fernbedienung.h"

extern "C" void __cxa_pure_virtual () { for (;;); }

int main ()
{
  Fernbedienung fernbedienung;
  
  while (true)
    fernbedienung.call();
  
}