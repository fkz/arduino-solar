#include "lcd_helper.h"
#include "fernbedienung.h"

LiquidCrystal Fernbedienung::LcdHelper::lcd (LCD_RS, LCD_ENABLE, LCD_D0, LCD_D1, LCD_D2, LCD_D3);

void Fernbedienung::LcdHelper::writeCommaNumber(long unsigned int arg1, const char* str)
{
  //Runden
  if (arg1 % 10 >= 5)
    arg1 += 10;
  lcd.print (arg1 / 1000);
  lcd.write ('.');
  int nc = arg1 % 1000;
  lcd.write ((nc / 100) + '0');
  lcd.write (((nc % 100)/10) + '0');
  lcd.print (str);
}
