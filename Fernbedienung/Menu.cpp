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
#include "solarboot_data.h"
#include "pot_value.h"
#include "flags.h"
#include "lcd_helper.h"


const uint8_t commandData[Menu::MENU_COUNT][5][2] = {
  { { 16, 21}, {21, 26}, {26, 31}, {10, 15}, {0, 9} },
  { { 32, 32}, {32, 32}, {32, 32}, {32, 32}, {32, 32} },
  { { 16, 21}, {22, 27}, {28, 31}, {32, 32}, {32, 32} },
  { {  13,  15}, {26, 29}, {32, 32}, {32, 32}, {32, 32} },
  { { 16, 19}, {19, 22}, {29, 31}, {32, 32}, {32, 32} },
  { { 19, 26}, {27, 30}, {32, 32}, {32, 32}, {32, 32} },
  { { 25, 28}, {28, 31}, {32, 32}, {32, 32}, {32, 32} },
  { { 16, 19}, {19, 22}, {29, 31}, {32, 32}, {32, 32} },
  { { 16, 25}, {25, 31}, {32, 32}, {32, 32}, {32, 32} },
  { { 32, 32}, {32, 32}, {32, 32}, {32, 32}, {32, 32} }
};

const char *commandStrings[Menu::MENU_COUNT][2] = {
  {" Drehzahl  Data ",     " MPPT Akku Trim "},
  {"Akku Fer.       "   , "Solarboot       "},
  {"   ---Trim---   "   , " Pot1  Pot2  up "},
  {"--MPPT--diff:   "   , "Intervall:      "},
  {"-inter.-akt.:   "   , " up ok einst.   "},
  {"Pot        -    "   , "    einst.  up  "},
  {"einst.     -    "   , "Wert:     ok up "},
  {"-diff-    akt.: "   , " up ok einst.   "},
  {" Daten          "   , " auslesen forma "},
  {"                ",    "                "}
};

namespace Menu{
Mode mode = RUNNING;
uint8_t actual;

//BEGIN TODO: REARRANGE!!

uint8_t mppt_act;
Fernbedienung::Pot::Poti trim_poti;
int max_pot_backup;
int min_pot_backup;
unsigned long int haltUntil = 0;

//END TODO
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
    else if ((mode == MPPT_SET_DIFF || mode == MPPT_SET_INTERVAL) && actual == 2)
    {
      mppt_act += richtung;
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
  using Fernbedienung::LcdHelper::lcd;
  lcd.setCursor (commandData[mode][actual][0] & 0xF, (commandData[mode][actual][0] & 0x10) >> 4);
  lcd.write (value);
  lcd.setCursor (commandData[mode][actual][1] & 0xF, (commandData[mode][actual][1] & 0x10) >> 4);
  lcd.write (value);
}

bool Menu::isStarted()
{
  return mode != RUNNING;
}

bool Menu::halt(long unsigned int until)
{
  if (mode != RUNNING)
    return false;
  
  haltUntil = until;
  return true;
}



void Menu::activate(Menu::Mode m)
{
  using Fernbedienung::LcdHelper::lcd;
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
  using Fernbedienung::xbee;
  switch (mode)
  {
    case RUNNING:
      activate (MAINMENU);
      break;
    case MAINMENU:
      if (actual == 0)
      {
	Fernbedienung::SolData::requestMPPTDiff ();
	Fernbedienung::SolData::requestMPPTInterval ();
	activate (MPPT);
      }
      else if (actual == 1)
      {
	activate (AKKU);
	Fernbedienung::SolData::requestSolarBattery ();
      }
      else if (actual == 2)
	activate (TRIM);
      else if (actual == 3)
	activate (SAVE_DATA);
      else if (actual == 4)
	activate (DREHZAHL);
      break;
    case AKKU:
      activate (MAINMENU);
      break;
    case TRIM:
      if (actual == 0 || actual == 1)
      {
	trim_poti = (Fernbedienung::Pot::Poti)actual;
	activate (CUSTOM_TRIM);
      }
      else
	activate (MAINMENU);
      break;
    case CUSTOM_TRIM:
      if (actual == 0)
      {
	activate (CUSTOM_TRIM2);
	max_pot_backup = Fernbedienung::Pot::max_pot (trim_poti);
	min_pot_backup = Fernbedienung::Pot::min_pot (trim_poti);
	Fernbedienung::Pot::max_pot (trim_poti) = Fernbedienung::Pot::min_pot (trim_poti) = Fernbedienung::Pot::pot_value (trim_poti);
	++Fernbedienung::Pot::max_pot (trim_poti);
      }
      else
	activate (MAINMENU);
      break;
    case CUSTOM_TRIM2:
      if (actual == 0)
      {
	activate (CUSTOM_TRIM);
      }
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
	mppt_act = Fernbedienung::SolData::getMPPTDiff();
	activate (MPPT_SET_DIFF);
      }
      else
      {
	mppt_act = Fernbedienung::SolData::getMPPTInterval();
	activate (MPPT_SET_INTERVAL);
      }
      break;
    case MPPT_SET_DIFF:
    case MPPT_SET_INTERVAL:
      if (actual == 0)
	activate (MAINMENU);
      else if (actual == 1)
      {
	if (mode == MPPT_SET_DIFF)
	{
	  uint8_t data[3];
	  data[0] = Message::ToSolarboat::REQUEST_MPPT;
	  data[1] = 's';
	  data[2] = mppt_act;
	  xbee.writeData(data, 3);
	}
	else
	{
	  uint8_t data[3];
	  data[0] = Message::ToSolarboat::SET_MPPT_INTERVAL;
	  data[1] = mppt_act & 0xFF;
	  data[2] = mppt_act >> 8;
	  xbee.writeData(data, 3);
	}
	Fernbedienung::SolData::requestMPPTDiff();
	Fernbedienung::SolData::requestMPPTInterval();
	activate (MPPT);
      }
      else if (actual == 2)
      {
	highlight(' ');
	actual = 0;
	highlight ('_');
      }
      break;
    case SAVE_DATA:
      if (actual == 0)
      {
	Fernbedienung::files.readAllCvs();
	activate (MAINMENU);
      }
      else if (actual == 1)
      {
	EEPROM.write (0, 0);
	activate (MAINMENU);
      }
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
    case MPPT_SET_INTERVAL:
    case MPPT_SET_DIFF:
      activate (MAINMENU);
      break;
    case CUSTOM_TRIM2:
      activate (CUSTOM_TRIM);
      break;
    case SAVE_DATA:
      activate (MAINMENU);
      break;
    case DREHZAHL:
      activate (MAINMENU);
      break;
    case MENU_COUNT:;
  }
}

//TODO: rework interval function
void Menu::interval()
{
  using Fernbedienung::xbee;
  using Fernbedienung::LcdHelper::lcd;
  
  if (mode == RUNNING)
  {
    lcd.setCursor (8,1);
    //if (interval_page)
    //{   
      switch (Fernbedienung::SolData::getActualMPPTType())
      {
	case Message::MPPT::UNKNOWN:
	  lcd.print ("??  ");
	  break;
	case Message::MPPT::NOMPPT:
	  lcd.print ("No  ");
	  break;
	case Message::MPPT::PERTURBEANDOBSERVE:
	  lcd.print ("P&O ");
	  break;
	case Message::MPPT::ESTIMATEPERTURB:
	  lcd.print ("PE  ");
	  break;
	case Message::MPPT::ESTIMATEESTIMATEPERTURB:
	  lcd.print ("PEE ");
	  break;
	case Message::MPPT::CONSTMPPT:
	  lcd.print ("coV ");
	  break;
	default:
	  lcd.print ("ERR ");
	  break;
      }
      
      if (Fernbedienung::Flags::getFlag (Fernbedienung::Flags::RECORDING))
      {
	if (Fernbedienung::Flags::getFlag (Fernbedienung::Flags::CONNECTION))
	  lcd.print ("REC");
	else
	  lcd.print ("_R_");
	if (Fernbedienung::recordId >= 10)
	  lcd.setCursor (14,1);
	lcd.print (Fernbedienung::recordId, 10);
      }
      else
      {
	lcd.write (Fernbedienung::Flags::getFlag (Fernbedienung::Flags::CONNECTION) ? ' ' : 'x');
	lcd.write (Fernbedienung::Flags::getFlag (Fernbedienung::Flags::BATTERY_FERNBEDIENUNG) ? '!' : ' ');
	lcd.write (Fernbedienung::Flags::getFlag (Fernbedienung::Flags::BATTERY_SOLARBOOT) ? '!' : ' ');
	lcd.write (' ');
      }
    //}
    //else
    /*{
      if (mppt_interval == 65535)
      {
	lcd.print ("????");
      }
      else
	lcd.print (mppt_interval);
      lcd.setCursor (14, 1);
      if (mppt_diff == 255)
      {
	lcd.print ("??");
      }
      else
	lcd.print (mppt_diff);
      {
	uint8_t data[1];
	data[0] = Message::ToSolarboat::REQUEST_MPPT_INTERVAL;
	xbee.writeData(data, 1);
      }
      {
	uint8_t data[2];
	data[0] = Message::ToSolarboat::REQUEST_MPPT;
	data[1] = 'r';
	xbee.writeData(data, 2);
      }
    }*/
  }
  else if (mode == AKKU)
  {
    int value = analogRead (Fernbedienung::BATTERY);
    lcd.setCursor(10, 0);
    Fernbedienung::LcdHelper::writeSpannung15 (value);
    lcd.print ("  ");
    uint8_t data[1];
    data[0] = Message::ToSolarboat::REQUEST_BATTERY;
    xbee.writeData(data, 1);
    
    lcd.setCursor (10, 1);
    if (Fernbedienung::SolData::getSolarBattery() == 0)
      lcd.print ("----");
    else
    {
      Fernbedienung::LcdHelper::writeSpannung15 (Fernbedienung::SolData::getSolarBattery());
      lcd.print ("  ");
    }
  }
  else if (mode == MPPT)
  {
    uint8_t data[2];
    data[0] = Message::ToSolarboat::REQUEST_MPPT;
    data[1] = 'r';
    xbee.writeData(data, 2);
    lcd.setCursor (14, 0);
    if (Fernbedienung::SolData::getMPPTDiff() == 255)
    {
      lcd.write ('-');
    }
    else
      lcd.write (Fernbedienung::SolData::getMPPTDiff() + '0');
    lcd.setCursor (11, 1);
    if (Fernbedienung::SolData::getMPPTInterval() == 65535)
    {
      lcd.print ("--");
    }
    else
      lcd.print (Fernbedienung::SolData::getMPPTInterval());
    
    {
      uint8_t data[2];
      data[0] = Message::ToSolarboat::REQUEST_MPPT;
      data[1] = 'r';
      xbee.writeData(data, 2);
    }
    
    { 
      uint8_t data[1];
      data[0] = Message::ToSolarboat::REQUEST_MPPT_INTERVAL;
      xbee.writeData(data, 1);
    }
  }
  else if (mode == CUSTOM_TRIM || mode == CUSTOM_TRIM2)
  {
    int max = Fernbedienung::Pot::max_pot (trim_poti);
    int min = Fernbedienung::Pot::min_pot (trim_poti);
    
    if (mode == CUSTOM_TRIM)
      lcd.setCursor (0, 1);
    else
      lcd.setCursor (6, 1);
    int value = Fernbedienung::Pot::pot_value (trim_poti);
    if (value > max)
    {
      if (mode == CUSTOM_TRIM2)
	Fernbedienung::Pot::max_pot (trim_poti) = value;
      value = 255;
    }
    else if (value < min)
    {
      if (mode == CUSTOM_TRIM2)
	Fernbedienung::Pot::min_pot(trim_poti) = value;
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
    if (Fernbedienung::SolData::getMPPTDiff() == 255)
      lcd.write ('-');
    else
      lcd.write (Fernbedienung::SolData::getMPPTDiff() + '0');
    lcd.setCursor (14, 1);
    lcd.write (mppt_act + '0');
  }
  else if (mode == MPPT_SET_INTERVAL)
  {
    lcd.setCursor (13, 0);
    if (Fernbedienung::SolData::getMPPTInterval() == 65535)
      lcd.write ('-');
    else
      lcd.print (Fernbedienung::SolData::getMPPTInterval());
    lcd.setCursor (14, 1);
    lcd.print (mppt_act);
  }
  else if (mode == SAVE_DATA)
  {
    lcd.setCursor (7, 0);
    lcd.print (Fernbedienung::files.diskSpace());
    lcd.print ("/992");
  }
}

void Menu::writeStromAndSpannung(unsigned long spannung, unsigned long strom)
{
  using Fernbedienung::LcdHelper::lcd;
  using Fernbedienung::LcdHelper::writeCommaNumber;
  if (mode == RUNNING)
  {
    lcd.clear();
    lcd.setCursor (0, 0);
    writeCommaNumber (spannung, "V");
    lcd.setCursor (8, 0);
    writeCommaNumber (strom, "A");
    lcd.setCursor (0, 1);
    writeCommaNumber (spannung * strom / 1000, "W");
    interval();
  }
}

void Menu::writeDrehzahl(long unsigned int drehzahl)
{
  static long unsigned int lastDrehzahl;
  static long unsigned int lastDrehzahlTime;
  if (mode == DREHZAHL)
  {
    using Fernbedienung::LcdHelper::lcd;
    lcd.clear();
    lcd.setCursor (0,0);
    lcd.print ("Dreh:");
    lcd.print (drehzahl);
    lcd.setCursor (0,1);
    lcd.print((drehzahl-lastDrehzahl)*1000ul/(millis()-lastDrehzahlTime));
    lcd.print("/s");
  }
  lastDrehzahl = drehzahl;
  lastDrehzahlTime = millis();
}




////////////////////////////////////////

#if false



void Menu::writeStromAndSpannung(unsigned long spannung, unsigned long strom)
{
  if (mode == RUNNING)
  {
    lcd.clear();
    lcd.setCursor (0, 0);
    writeCommaNumber (spannung, "V");
    lcd.setCursor (8, 0);
    writeCommaNumber (strom, "A");
    lcd.setCursor (0, 1);
    writeCommaNumber (spannung * strom / 1000, "W");
    interval();
  }
}

void Menu::writeRawStromAndSpannung(int spannung, int strom)
{
  if (flags & RECORDING)
    files.newData(spannung, strom);
}


void Menu::writeCommaNumber(unsigned long int arg1, const char *str)
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

void Menu::writeSpannung15(int arg1)
{
  unsigned long spannung = (unsigned long)arg1 * 15271 / 1000;
  writeCommaNumber(spannung, "V");
}



void Menu::interval()
{
  using Fernbedienung::xbee;
  
  if (mode == RUNNING)
  {
    lcd.setCursor (8,1);
    if (interval_page)
    {   
      switch (mppt)
      {
	case Message::MPPT::UNKNOWN:
	  lcd.print ("??  ");
	  break;
	case Message::MPPT::NOMPPT:
	  lcd.print ("No  ");
	  break;
	case Message::MPPT::PERTURBEANDOBSERVE:
	  lcd.print ("P&O ");
	  break;
	case Message::MPPT::ESTIMATEPERTURB:
	  lcd.print ("PE  ");
	  break;
	case Message::MPPT::ESTIMATEESTIMATEPERTURB:
	  lcd.print ("PEE ");
	  break;
	default:
	  lcd.print ("ERR ");
	  break;
      }
      
      lcd.write (flags & CONNECTION ? flags & RECORDING ? 'R' : ' ' : 'x');
      lcd.write (flags & BATTERY_FERNBEDIENUNG ? '!' : ' ');
      lcd.write (flags & BATTERY_SOLARBOOT ? '!' : ' ');
      
    }
    else
    {
      if (mppt_interval == 65535)
      {
	lcd.print ("????");
      }
      else
	lcd.print (mppt_interval);
      lcd.setCursor (14, 1);
      if (mppt_diff == 255)
      {
	lcd.print ("??");
      }
      else
	lcd.print (mppt_diff);
      {
	uint8_t data[1];
	data[0] = Message::ToSolarboat::REQUEST_MPPT_INTERVAL;
	xbee.writeData(data, 1);
      }
      {
	uint8_t data[2];
	data[0] = Message::ToSolarboat::REQUEST_MPPT;
	data[1] = 'r';
	xbee.writeData(data, 2);
      }
    }
  }
  else if (mode == AKKU)
  {
    int value = analogRead (Fernbedienung::BATTERY);
    lcd.setCursor(10, 0);
    writeSpannung15 (value);
    lcd.print ("  ");
    uint8_t data[1];
    data[0] = Message::ToSolarboat::REQUEST_BATTERY;
    xbee.writeData(data, 1);
    
    lcd.setCursor (10, 1);
    if (battery_solarboot == 0)
      lcd.print ("----");
    else
    {
      writeSpannung15 (battery_solarboot);
      lcd.print ("  ");
    }
  }
  else if (mode == MPPT)
  {
    uint8_t data[2];
    data[0] = Message::ToSolarboat::REQUEST_MPPT;
    data[1] = 'r';
    xbee.writeData(data, 2);
    lcd.setCursor (14, 0);
    if (mppt_diff == 255)
    {
      lcd.write ('-');
    }
    else
      lcd.write (mppt_diff + '0');
    lcd.setCursor (11, 1);
    if (mppt_interval == 65535)
    {
      lcd.print ("--");
    }
    else
      lcd.print (mppt_interval);
    
    {
      uint8_t data[2];
      data[0] = Message::ToSolarboat::REQUEST_MPPT;
      data[1] = 'r';
      xbee.writeData(data, 2);
    }
    
    { 
      uint8_t data[1];
      data[0] = Message::ToSolarboat::REQUEST_MPPT_INTERVAL;
      xbee.writeData(data, 1);
    }
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
    lcd.write (mppt_act + '0');
  }
  else if (mode == MPPT_SET_INTERVAL)
  {
    lcd.setCursor (13, 0);
    if (mppt_interval == 65535)
      lcd.write ('-');
    else
      lcd.print (mppt_interval);
    lcd.setCursor (14, 1);
    lcd.print (mppt_act);
  }
}

void Menu::setActualMPPTType(char arg1)
{
  mppt = arg1;
}


uint8_t Menu::getPotiValue(Menu::Poti poti)
{
  if (mode == TRIM || mode == CUSTOM_TRIM || mode == CUSTOM_TRIM2)
    return 128;
  else
  {
    int value = pot_value (poti);
    if (value > max_pot (poti))
      return 255;
    else if (value < min_pot(poti))
      return 0;
    else
      return (long)(value-min_pot(poti))*256/(max_pot(poti)-min_pot(poti));
  }
}

void Menu::endRecord()
{
  files.endRecord();
  flags = (Flags)(flags & ~RECORDING);
}

void Menu::startRecord()
{
  files.startRecord();
  flags = (Flags)(flags | RECORDING);
}

void Menu::startStopRecord()
{
  if (flags | RECORDING)
    endRecord();
  else
    startRecord();
  interval();
}

#endif
