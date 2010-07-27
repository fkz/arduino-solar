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
#include <MessageTypes.h>
#include "fernbedienung.h"

const uint8_t commandData[Menu::MENU_COUNT][5][2] = {
  { { 16, 21}, {21, 26}, {26, 31}, {12, 15}, {32, 32} },
  { { 16, 19}, {32, 32}, {32, 32}, {32, 32}, {32, 32} },
  { { 16, 21}, {22, 27}, {28, 31}, {32, 32}, {32, 32} },
  { {  0,  3}, {16, 19}, {19, 23}, {23, 26}, {26, 30} },
  { { 21, 24}, {32, 32}, {32, 32}, {32, 32}, {32, 32} }
};

const char *commandStrings[Menu::MENU_COUNT][2] = {
  {"Hauptmen\xF5    up ", " MPPT Akku Trim "},
  {"Akku Sol.       "   , " up Fern.       "},
  {"   ---Trim---   "   , " Pot1  Pot2  up "},
  {" up akt.:       "   , " No P&O PE PEE  "},
  {" MPPT ge\xE1ndert  ", "      ok        "}
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
  if (m == CUSTOM_TRIM)
  {
    
  }
  else if (m != RUNNING)
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
      {
	activate (AKKU);
	battery_solarboot = 0;
      }
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
    case MPPT_DATA_SEND:
      activate (MAINMENU);
    default:;
  }
}

void Menu::chooseMPPT()
{
  uint8_t data[2];
  data[0] = Message::CHANGE_MPPT_TYPE;
  data[1] = getMPPTChar (actual);
  xbee.writeData(data, 2);
  activate (MPPT_DATA_SEND);
}

char Menu::getMPPTChar(uint8_t arg1)
{
  switch (arg1)
  {
    case 1:
      return Message::MPPT_NOMPPT;
    case 2:
      return Message::MPPT_PERTURBEANDOBSERVE;
    case 3:
      return Message::MPPT_ESTIMATEPERTURB;
    case 4:
      return Message::MPPT_ESTIMATEESTIMATEPERTURB;
    default:
      return 0;
  }
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
  if (mode == RUNNING)
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
	lcd.print ("P&O ");
	break;
      case PE:
	lcd.print ("PE  ");
	break;
      case PEE:
	lcd.print ("PEE ");
	break;
      default:
	lcd.print ("ERR ");
	break;
    }
    
    lcd.write (flags & CONNECTION ? ' ' : 'x');
    lcd.write (flags & BATTERY_FERNBEDIENUNG ? '!' : ' ');
    lcd.write (flags & BATTERY_SOLARBOOT ? '!' : ' ');
  }
  else if (mode == AKKU)
  {
    int value = analogRead (Fernbedienung::BATTERY);
    lcd.setCursor(10, 0);
    lcd.print (value);
    lcd.print ("  ");
    uint8_t data[1];
    data[0] = Message::REQUEST_BATTERY;
    xbee.writeData(data, 1);
    
    lcd.setCursor (10, 1);
    if (battery_solarboot == 0)
      lcd.print ("----");
    else
    {
      lcd.print (battery_solarboot);
      lcd.print ("  ");
    }
  }
  else if (mode == MPPT)
  {
    lcd.setCursor (10, 0);
    switch (mppt)
    {
      case UNKNOWN:
	lcd.print ("------");
	break;
      case NO_MPPT:
	lcd.print ("NoMPPT");
	break;
      case PANDP:
	lcd.print ("P&O   ");
	break;
      case PE:
	lcd.print ("PE    ");
	break;
      case PEE:
	lcd.print ("PEE   ");
	break;
      default:
	lcd.print ("ERROR ");
	break;
    }
  }
}

void Menu::setActualMPPTType(uint8_t arg1)
{
  mppt = (MPPTType)arg1;
}
