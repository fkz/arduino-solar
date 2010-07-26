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
: lcd (LCD_RS, LCD_ENABLE, LCD_D0, LCD_D1, LCD_D2, LCD_D3), isInitializing (true), lcdCodePage (0)
{
  addMethod(this, &Fernbedienung::readPackages, 0);
  addMethod(this, &Fernbedienung::sendData, 1000);
  addMethod(this, &Fernbedienung::checkBatteryState, 60000);
  addMethod (this, &Fernbedienung::writeLcd, 1000);
  lcd.begin(16, 2);
  lcd.print ("Initialisiere");
  pinMode (5, OUTPUT);
  digitalWrite (5, LOW);
  lcd_various_data[0][0] = 'H';
  lcd_various_data[0][1] = 'a';
  lcd_various_data[0][2] = 'l';
  lcd_various_data[0][3] = 'l';
  lcd_various_data[0][4] = 'o';
  lcd_various_data[0][5] = ' ';
  lcd_various_data[0][6] = 'd';
  lcd_various_data[0][7] = 'u';
  lcd_various_data[1][0] = 'E';
  lcd_various_data[1][1] = 's';
  lcd_various_data[1][2] = ' ';
  lcd_various_data[1][3] = 'i';
  lcd_various_data[1][4] = 's';
  lcd_various_data[1][5] = 't';
  lcd_various_data[1][6] = ' ';
  lcd_various_data[1][7] = ' ';
  
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
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print (spannung, DEC);
    lcd.print ("mV");
    lcd.setCursor (8, 0);
    lcd.print(strom, DEC);
    lcd.print("mA");
    
    long power = strom * spannung;
    power /= 1000;
    lcd.setCursor (0, 1);
    lcd.print (power);
    lcd.print ("mW");
    
    writeLcd ();
  }
}

void Fernbedienung::connectionInterrupted()
{
  lcd.clear();
  lcd.setCursor (0,0);
  lcd.print ("Verbindung");
  lcd.setCursor (0, 1);
  lcd.print ("unterbrochen");
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
  if (value < 700)
  {
    const char *message = "Batterie";
    const char *message2 = "leer    ";
    for (int i = 0; i != 8; ++i)
    {
      lcd_various_data[0][i] = message[i];
      lcd_various_data[1][i] = message2[i];
    }
  }
}

void Fernbedienung::writeLcd(bool inInterval)
{
  if (inInterval)
    lcdCodePage = (lcdCodePage + 1) % 2;
  lcd.setCursor(8, 1);
  for (int i = 0; i != 8; ++i)
  {
    lcd.write (lcd_various_data[lcdCodePage][i]);
  }
}
