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
  { {  13,  15}, {26, 29}, {32, 32}, {32, 32}, {32, 32} },
  { { 16, 19}, {19, 31}, {32, 32}, {32, 32}, {32, 32} },
  { { 19, 26}, {27, 30}, {32, 32}, {32, 32}, {32, 32} },
  { { 25, 28}, {28, 31}, {32, 32}, {32, 32}, {32, 32} },
  { { 16, 19}, {19, 22}, {29, 31}, {32, 32}, {32, 32} }
};

const char *commandStrings[Menu::MENU_COUNT][2] = {
  {"Hauptmen\xF5    up ", " MPPT Akku Trim "},
  {"Akku Sol.       "   , " up Fern.       "},
  {"   ---Trim---   "   , " Pot1  Pot2  up "},
  {"--MPPT--diff:   "   , "Intervall:      "},
  {" MPPT ge\xE1ndert  ", " ok diff einst. "},
  {"Pot        -    "   , "    einst.  up  "},
  {"einst.     -    "   , "Wert:     ok up "},
  {"-diff-    akt.: "   , " up ok einst.   "}
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
    else if (mode == MPPT_SET_DIFF && actual == 2)
    {
      mppt_diff_act += richtung;
      interval();
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
      {
	mppt_diff = 255;
	activate (MPPT);
      }
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
      {
	trim_poti = actual;
	activate (CUSTOM_TRIM);
      }
      else
	activate (MAINMENU);
      break;
    case CUSTOM_TRIM:
      if (actual == 0)
      {
	activate (CUSTOM_TRIM2);
	max_pot_backup = max_pot (trim_poti);
	min_pot_backup = min_pot (trim_poti);
	max_pot (trim_poti) = min_pot (trim_poti) = pot_value (trim_poti);
	++max_pot (trim_poti);
      }
      else
	activate (MAINMENU);
      break;
    case CUSTOM_TRIM2:
      if (actual == 0)
	activate (CUSTOM_TRIM);
      else
      {
	max_pot(trim_poti) = max_pot_backup;
	min_pot(trim_poti) = min_pot_backup;
	activate (CUSTOM_TRIM);
      }
      break;
    case MPPT:
      if (actual == 0)
      {
	mppt_diff_act = mppt_diff;
	activate (MPPT_SET_DIFF);
      }
      else
      {
	//TODO
	c
      }
      break;
    case MPPT_DATA_SEND:
      if (actual == 1)
      {
	mppt_diff_act = mppt_diff;
	activate (MPPT_SET_DIFF);
      }
      else
	activate (MAINMENU);
      break;
    case MPPT_SET_DIFF:
      if (actual == 0)
	activate (MAINMENU);
      else if (actual == 1)
      {
	uint8_t data[3];
	data[0] = Message::SEND_MPPT;
	data[1] = 's';
	data[2] = mppt_diff_act;
	xbee.writeData(data, 3);
	mppt_diff = 255;
	activate (MPPT);
      }
      else if (actual == 2)
      {
	highlight(' ');
	actual = 0;
	highlight ('_');
      }
      break;
    default:;
  }
}

void Menu::goUp()
{
  switch (mode)
  {
    case RUNNING:
      break;
    case MAINMENU:
      activate (RUNNING);
      break;
    case AKKU:
    case TRIM:
    case CUSTOM_TRIM:
    case MPPT:
    case MPPT_DATA_SEND:
    case MPPT_SET_DIFF:
      activate (MAINMENU);
      break;
    case CUSTOM_TRIM2:
      activate (CUSTOM_TRIM);
      break;
    case MENU_COUNT:;
  }
}




void Menu::writeData(long spannung, long strom)
{
  if (mode == RUNNING)
  {
    lcd.clear();
    lcd.setCursor (0, 0);
    writeCommaNumber (spannung, "V");
    lcd.setCursor (8, 0);
    writeCommaNumber (strom, "A");
    lcd.setCursor (0, 1);
    writeCommaNumber (spannung * (long)strom / 1000, "W");
    interval();
  }
}

void Menu::writeCommaNumber(long int arg1, const char *str)
{
  lcd.print (arg1 / 1000);
  lcd.write ('.');
  int nc = arg1 % 1000;
  lcd.write ((nc / 100) + '0');
  lcd.write (((nc % 100)/10) + '0');
  lcd.print (str);
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
    uint8_t data[2];
    data[0] = Message::REQUEST_MPPT;
    data[1] = 'r';
    xbee.writeData(data, 2);
    lcd.setCursor (14, 0);
    if (mppt_diff == 255)
      lcd.write ('-');
    else
      lcd.write (mppt_diff + '0');
    //TODO: write interval
    in
  }
  else if (mode == CUSTOM_TRIM || mode == CUSTOM_TRIM2)
  {
    int max = max_pot (trim_poti);
    int min = min_pot (trim_poti);
    
    if (mode == CUSTOM_TRIM)
      lcd.setCursor (0, 1);
    else
      lcd.setCursor (6, 1);
    int value = pot_value (trim_poti);
    if (value > max)
    {
      if (mode == CUSTOM_TRIM2)
	max_pot (trim_poti) = value;
      value = 255;
    }
    else if (value < min)
    {
      if (mode == CUSTOM_TRIM2)
	min_pot(trim_poti) = value;
      value = 0;
    }
    else
    {
      value -= min;
      value = (long)value * 256 / (max - min);
    }
    lcd.print (value);
    lcd.write (' ');
    
    lcd.setCursor (7, 0);
    lcd.print (min);
    lcd.setCursor (12, 0);
    lcd.print (max);
    
  }
  else if (mode == MPPT_SET_DIFF)
  {
    lcd.setCursor (15, 0);
    if (mppt_diff == 255)
      lcd.write ('-');
    else
      lcd.write (mppt_diff + '0');
    lcd.setCursor (14, 1);
    lcd.write (mppt_diff_act + '0');
  }
}

void Menu::setActualMPPTType(uint8_t arg1)
{
  mppt = (MPPTType)arg1;
}

int& Menu::max_pot(uint8_t poti)
{
  return POT_MAX[poti];
}

int& Menu::min_pot(uint8_t poti)
{
  return POT_MIN[poti];
}

int Menu::pot_value(uint8_t poti)
{
  return analogRead (poti == SPEED ? Fernbedienung::POT_SPEED : Fernbedienung::POT_TURN);
}

uint8_t Menu::getPotiValue(Menu::Poti poti)
{
  if (mode != RUNNING)
    return 128;
  else
  {
    int value = pot_value (poti);
    if (value > max_pot (poti))
      return 255;
    else if (value < min_pot(poti))
      return 0;
    else
      return (value-min_pot(poti))*256/(max_pot(poti)-min_pot(poti));
  }
}
