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
  init();
  pinMode(ledPin, OUTPUT);
  pinMode(ledPin, INPUT);
  
  while (true)
  {
    digitalWrite(ledPin, HIGH);
    delay(1000);
    digitalWrite(ledPin, LOW);
    delay (1000);
    
    EEPROM.read(10);
    Serial.println ("Hallo Welt\n");
  }
}

