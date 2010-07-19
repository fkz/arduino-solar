#include <WProgram.h>
#include <EEPROM/EEPROM.h>


const int ledPin = 14;

// this line is for pure virtual functions in the vtable
//extern "C" { void __cxa_pure_virtual () { } };

inline Print &operator<< (Print &print, const char *str)
{
  print.print(str);
  return print;
}

int main ()
{

}

