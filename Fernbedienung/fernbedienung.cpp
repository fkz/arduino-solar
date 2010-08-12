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
#include <MessageTypes.h>

Fernbedienung::Fernbedienung()
: lcd (LCD_RS, LCD_ENABLE, LCD_D0, LCD_D1, LCD_D2, LCD_D3), menu(lcd, *this), pot_steuerung_state(0xFF)
{
  addMethod(this, &Fernbedienung::readPackages, 0);
  addMethod(this, &Fernbedienung::sendData, 1000);
  addMethod(this, &Fernbedienung::checkBatteryState, 60000);
  addMethod(&menu, &Menu::interval, 2000);
  addMethod(this, &Fernbedienung::controlButtons, 0);
  lcd.begin(16, 2);
  lcd.print ("Initialisiere");
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
  if (data[0] == Message::DATA_FROM_SOLARBOAT && length == 6)
  {
    // show data
    long strom = data[1] | (data[2] << 8);
    //FIXME: set correct factor
    strom *= 5;
    
    long spannung = data[3] | (data[4] << 8);
    // 0 = 0V, 1024=(5V*(14,7)/4,7)=3.127*5=15,635
    // ==> 0,015267V pro Stelle
    spannung *= 15267; 
    spannung /= 1000; // spannung in mV
    menu.setActualMPPTType (data[5]);
    menu.writeStromAndSpannung(spannung, strom);
  }
  else if (data[0] == Message::BATTERY)
  {
    int value = data[1] | (data[2] << 8);
    menu.setSolarBattery(value);
  }
  else if (data[0] == Message::REQUEST_MPPT)
  {
    if (data[1] == 'r')
      menu.setMPPTDiff (data[2]);
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
  
  data[0] = Message::POTI_DATA;
  data[1] = menu.getPotiValue(Menu::SPEED);
  data[2] = menu.getPotiValue(Menu::TURN);
  writeData(data, sizeof(data));
}

void Fernbedienung::checkBatteryState()
{
  int value = analogRead (BATTERY);
  if (value < MIN_BATTERY_VALUE)
    menu.setFernBattery();
}

void Fernbedienung::setMPPT(char mpptType)
{
  uint8_t data[2];
  data[0] = Message::CHANGE_MPPT_TYPE;
  data[1] = mpptType;
  writeData (data, 2);
}


void Fernbedienung::controlButtons()
{
  int value = analogRead (POT_STEUERUNG);
  uint8_t command;
  if (value < POT_STEUERUNG_1)
    command = 0;
  else if (value < POT_STEUERUNG_2)
    command = 1;
  else if (value < POT_STEUERUNG_3)
    command = 2;
  else
    command = 3;
  
  if (command != pot_steuerung_state)
  {
    pot_steuerung_state = command;
    
    if (command == 4)
    {
      menu.start();
    }
    else
    {
      menu.end();
      char mpptType;
      switch (command)
      {
	case 0:
	  mpptType = Message::MPPT_NOMPPT;
	  break;
	case 1:
	  mpptType = Message::MPPT_PERTURBEANDOBSERVE;
	  break;
	case 2:
	  mpptType = Message::MPPT_ESTIMATEPERTURB;
	  break;
	case 3:
	  mpptType = Message::MPPT_ESTIMATEESTIMATEPERTURB;
	  break;
      }
      setMPPT (mpptType);
    }
  }
  else if (menu.isStarted ())
  {
    int speed = analogRead (POT_SPEED);
    int8_t speed_c;
    if (speed < POT_SPEED_LOW)
      speed_c = -1;
    else if (speed < POT_SPEED_HIGH)
      speed_c = 0;
    else
      speed_c = 1;
    
    if (speed_c != pot_speed_state)
    {
      pot_speed_state = speed_c;
      if (speed_c == -1)
	menu.goUp();
      else if (speed_c == 1)
	menu.setExecute();
    }
    
    int turn = analogRead (POT_TURN);
    int8_t turn_c;
    if (turn < POT_TURN_LOW)
      turn_c = -1;
    else if (turn < POT_TURN_HIGH)
      turn_c = 0;
    else
      turn_c = 1;
    
    if (turn_c != pot_turn_state)
    {
      pot_turn_state = turn_c;
      if (turn_c != 0)
	menu.setAction(turn_c);
    }
    
  }
}
