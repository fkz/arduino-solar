
#include <LiquidCrystal/LiquidCrystal.h>
namespace Fernbedienung{
namespace LcdHelper{

extern LiquidCrystal lcd;
  
void writeCommaNumber(unsigned long int arg1, const char *str);
inline void writeSpannung15(int arg1)
{
  unsigned long spannung = (unsigned long)arg1 * 15271 / 1000;
  writeCommaNumber(spannung, "V");
}


  
};
};