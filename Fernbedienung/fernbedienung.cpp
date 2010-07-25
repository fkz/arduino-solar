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
#include <pins_arduino.h>

Fernbedienung::Fernbedienung()
: lcd (LCD_RS, LCD_ENABLE, LCD_D0, LCD_D1, LCD_D2, LCD_D3), isInitializing (true)
{
  addMethod(this, &Fernbedienung::readPackages, 0);
  addMethod(this, &Fernbedienung::sendData, 1000);
  lcd.begin(16, 2);
  lcd.print ("Initialisiere");
  pinMode (5, OUTPUT);
  digitalWrite (5, LOW);  
  writeData ((uint8_t*)"Creating object \"Fernbedienung\"", 30);
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
  if (data[0] == 'S')
  {
    // show data
    int strom = data[1] + data[2] >> 8;
    int spannung = data[3] + data[4] >> 8;
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

void Fernbedienung::sendData()
{
  uint8_t data[3];
  int speed = analogRead (POT_SPEED);
  int turn = analogRead (POT_TURN);
  data[0] = 'F';
  data[1] = speed << 2;
  data[2] = turn << 2;
  writeData(data, sizeof(data));
  
  //ASCII
  static int nr = 0;
  lcd.clear();
  lcd.print ("Nr. :");
  lcd.print (nr);
  lcd.setCursor(0,1);
  lcd.print("ASCII. :");
  lcd.print ((char)nr++);
  lcd.setCursor(8, 0);
  lcd.print (getReadCount());
}