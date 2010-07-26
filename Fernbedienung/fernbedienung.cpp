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
#include "../MessageTypes.h"

Fernbedienung::Fernbedienung()
: lcd (LCD_RS, LCD_ENABLE, LCD_D0, LCD_D1, LCD_D2, LCD_D3), menu(lcd), isInitializing (true)
{
  addMethod(this, &Fernbedienung::readPackages, 0);
  addMethod(this, &Fernbedienung::sendData, 1000);
  addMethod(this, &Fernbedienung::checkBatteryState, 60000);
  addMethod(&menu, &Menu::interval, 2000);
  lcd.begin(16, 2);
  lcd.print ("Initialisiere");
  pinMode (5, OUTPUT);
  digitalWrite (5, LOW);
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
  menu.setConnection(true);
  if (data[0] == Message::DATA_FROM_SOLARBOAT)
  {
    // show data
    long strom = data[1] | (data[2] << 8);
    strom *= 5;
    
    
    long spannung = data[3] | (data[4] << 8);
    // 0 = 0V, 1024=(5V*(14,7)/4,7)=3.127*5=15,635
    // ==> 0,015267V pro Stelle
    spannung *= 15267; 
    spannung /= 1000; // spannung in mV
    menu.writeData(spannung, strom);
  }
  else if (data[0] == '-')
    menu.setAction(-1);
  else if (data[0] == '+')
    menu.setAction(+1);
  else if (data[0] == 'R')
    menu.setExecute();
}

void Fernbedienung::connectionInterrupted()
{
  menu.setConnection(false);
}

void Fernbedienung::sendData()
{
  uint8_t data[3];
  int speed = analogRead (POT_SPEED);
  int turn = analogRead (POT_TURN);
  data[0] = Message::POTI_DATA;
  data[1] = speed >> 2;
  data[2] = turn >> 2;
  writeData(data, sizeof(data));
}

void Fernbedienung::checkBatteryState()
{
  int value = analogRead (BATTERY);
  if (value < 600)
    menu.setFernBattery();
}