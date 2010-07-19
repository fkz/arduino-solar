#include <WProgram.h>
#include <XBee/XBee.h>
#include <LiquidCrystal/LiquidCrystal.h>

int main ()
{
  LiquidCrystal lcd (12, 11, 5, 4, 3, 2);
  XBee xbee;
  xbee.begin();
  
  
}