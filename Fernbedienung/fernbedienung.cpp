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
#include "fernbedienung.h"

Fernbedienung::Fernbedienung()
: MyXBee(XBeeAddress64 (receiver1, receiver2)), lcd (LCD_RS, LCD_ENABLE, LCD_D0, LCD_D1, LCD_D2, LCD_D3)
{
  addMethod(this, &Fernbedienung::readPackages, 0);
}

void Fernbedienung::error(uint8_t arg1)
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print ("Fehler XBee Nr.");
  lcd.setCursor(0,1);
  lcd.print (arg1, DEC);
}

void Fernbedienung::readData(uint8_t* data, uint8_t length)
{
  if (data[0] == 'd')
  {
    // show data
    int spannung = data[1] + data[2] << 8;
    int strom = data[3] + data[4] << 8;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print ("Spannung: ");
    lcd.print (spannung, DEC);
    lcd.setCursor (0, 1);
    lcd.print ("Strom: ");
    lcd.print(strom, DEC);
  }
}

void Fernbedienung::connectionInterrupted()
{
  lcd.clear();
  lcd.setCursor (0,0);
  lcd.print ("Connection with");
  lcd.setCursor (0, 1);
  lcd.print ("Solarboat lost");
}

