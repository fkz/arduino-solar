/*
    Copyright (C) 2010 Fabian Schmitthenner

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/
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
