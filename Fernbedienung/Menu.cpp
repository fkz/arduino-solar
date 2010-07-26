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
#include "Menu.h"

const uint8_t commandData[4][5][2] = {
  { { 16, 21}, {21, 26}, {26, 31}, {12, 15}, {32, 32} },
  { { 16, 19}, {32, 32}, {32, 32}, {32, 32}, {32, 32} },
  { { 16, 21}, {22, 27}, {28, 31}, {32, 32}, {32, 32} },
  { {  0,  3}, {16, 19}, {19, 23}, {23, 26}, {26, 30} }
};

const char *commandStrings[4][2] = {
  {"Hauptmen\xF5    up ", " MPPT Akku Trim "},
  {"Akku Sol.       "   , " up Fern.       "},
  {"   ---Trim---   "   , " Pot1  Pot2  up "},
  {" up akt.:       "   , " No P&P PE PEE  "}
};

void Menu::setAction(int8_t richtung)
{
  if (mode != RUNNING)
  {
    highlight(' ');    
    if (actual == 0 && richtung == -1)
    {
      for (actual = 1; actual != 5; ++actual)
	if (commandData[mode][actual][0] == 32)
	  break;
      --actual;
    }
    else
    {
      actual += richtung;
      if (actual == 5 || commandData[mode][actual][0] == 32)
	actual = 0;
    }
    highlight('_');
  }
}

void Menu::highlight(char value)
{
  lcd.setCursor (commandData[mode][actual][0] & 0xF, (commandData[mode][actual][0] & 0x10) >> 4);
  lcd.write (value);
  lcd.setCursor (commandData[mode][actual][1] & 0xF, (commandData[mode][actual][1] & 0x10) >> 4);
  lcd.write (value);
}

void Menu::activate(Menu::Mode m)
{
  mode = m;
  actual = 0;
  if (m != RUNNING)
  {
    lcd.setCursor (0,0);
    lcd.print (commandStrings[m][0]);
    lcd.setCursor(0,1);
    lcd.print (commandStrings[m][1]);
    highlight('_');
  }
  else
    lcd.clear();
}

void Menu::setExecute()
{
  switch (mode)
  {
    case RUNNING:
      activate (MAINMENU);
      break;
    case MAINMENU:
      if (actual == 0)
	activate (MPPT);
      else if (actual == 1)
	activate (AKKU);
      else if (actual == 2)
	activate (TRIM);
      else if (actual == 3)
	activate (RUNNING);
      break;
    case AKKU:
      activate (MAINMENU);
      break;
    case TRIM:
      if (actual == 0 || actual == 1)
	activate (CUSTOM_TRIM);
      else
	activate (MAINMENU);
      break;
    case MPPT:
      if (actual == 0)
	activate (MAINMENU);
      else
	chooseMPPT ();
      break;
  }
}

void Menu::chooseMPPT()
{
  
}


void Menu::writeData(int spannung, int strom)
{
  if (mode == RUNNING)
  {
    lcd.clear();
    lcd.setCursor (0, 0);
    lcd.print (spannung);
    lcd.print ("U");
    lcd.setCursor (8, 0);
    lcd.print (strom);
    lcd.print ("A");
    lcd.setCursor (0, 1);
    lcd.print (spannung * (long)strom);
    lcd.print ("W");
    interval();
  }
}

void Menu::interval()
{
  lcd.setCursor (8,1);
  switch (mppt)
  {
    case UNKNOWN:
      lcd.print ("??  ");
      break;
    case NO_MPPT:
      lcd.print ("No  ");
      break;
    case PANDP:
      lcd.print ("P&P ");
      break;
    case PE:
      lcd.print ("PE  ");
      break;
    case PEE:
      lcd.print ("PEE ");
      break;
  }
  
  lcd.write (flags & CONNECTION ? ' ' : 'x');
  lcd.write (flags & BATTERY_FERNBEDIENUNG ? '!' : ' ');
  lcd.write (flags & BATTERY_SOLARBOOT ? '!' : ' ');
}
