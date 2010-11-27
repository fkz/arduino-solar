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
#include <avr/pgmspace.h>


const prog_uint8_t commandData[Menu::MENU_COUNT*5*2] PROGMEM = {
    21, 26, 26, 31, 10, 15, 0, 9 , 32, 32,
      32, 32 ,  32, 32 ,  32, 32 ,  32, 32 ,  32, 32   ,
      16, 21 ,  22, 27 ,  28, 31 ,  32, 32 ,  32, 32   ,
      19, 26 ,  27, 30 ,  32, 32 ,  32, 32 ,  32, 32   ,
      25, 28 ,  28, 31 ,  32, 32 ,  32, 32 ,  32, 32   ,
      16, 25 ,  25, 31 ,  32, 32 ,  32, 32 ,  32, 32   ,
      32, 32 ,  32, 32 ,  32, 32 ,  32, 32 ,  32, 32   
};

const prog_char commandStrings[Menu::MENU_COUNT*2*16+1] PROGMEM = 
  " Drehzahl  Data "    "       Akku Trim" 
  "Akku Fer.       "    "Solarboot       "
  "   ---Trim---   "    " Pot1  Pot2  up "
  "Pot        -    "    "    einst.  up  "
  "einst.     -    "    "Wert:     ok up "
  " Daten          "    " auslesen forma "
  "                "    "                "
;

namespace Menu{
Mode mode = RUNNING;
uint8_t actual;

//BEGIN TODO: REARRANGE!!

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
	if (pgm_read_byte (commandData + mode*10 + actual*5) == 32)
	  break;
      --actual;
    }
    else
    {
      actual += richtung;
      if (actual == 5 || pgm_read_byte (commandData+10*mode+2*actual) == 32)
	actual = 0;
    }
    highlight('_');
  }
}

void Menu::highlight(char value)
{
  using Fernbedienung::LcdHelper::lcd;
  uint8_t command1 = pgm_read_byte (commandData + 10*mode + 2*actual);
  uint8_t command2 = pgm_read_byte (commandData + 10*mode + 2*actual + 1);
  lcd.setCursor (command1 & 0xF, (command1 & 0x10) >> 4);
  lcd.write (value);
  lcd.setCursor (command2 & 0xF, (command2 & 0x10) >> 4);
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
    for (int i = 0; i != 16; ++i)
    {
      lcd.write (pgm_read_byte (commandStrings + 32*m + i));
    }
    lcd.setCursor(0,1);
    for (int i = 0; i != 16; ++i)
    {
      lcd.write (pgm_read_byte (commandStrings + 32*m + 16 + i));
    }
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
	activate (AKKU);
	Fernbedienung::SolData::requestSolarBattery ();
      }
      else if (actual == 1)
	activate (TRIM);
      else if (actual == 2)
	activate (SAVE_DATA);
      else if (actual == 3)
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
    case SAVE_DATA:
      if (actual == 0)
      {
	Fernbedienung::files.readAllCvs();
	activate (MAINMENU);
      }
      else if (actual == 1)
      {
	Fernbedienung::files.clear();
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
  
  if (Fernbedienung::Flags::getFlag (Fernbedienung::Flags::CONNECTION))
  {
    digitalWrite (Fernbedienung::LED_PIN, HIGH);
  }
  else
  {
    digitalWrite (Fernbedienung::LED_PIN, LOW);
  }
  
  if (mode == RUNNING)
  {
    if (haltUntil < millis())
    {
    
    lcd.setCursor (8,1);
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
    }
  }
  else if (mode == AKKU)
  {
    int value = analogRead (Fernbedienung::BATTERY);
    lcd.setCursor(10, 0);
    Fernbedienung::LcdHelper::writeSpannung15 (value);
    lcd.print ("  ");
    xbee.writePackage< Message::ToSolarboat::REQUEST_BATTERY > ();
    
    lcd.setCursor (10, 1);
    if (Fernbedienung::SolData::getSolarBattery() == 0)
      lcd.print ("----");
    else
    {
      Fernbedienung::LcdHelper::writeSpannung2 (Fernbedienung::SolData::getSolarBattery());
      lcd.print ("  ");
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
    }
    else if (value < min)
    {
      if (mode == CUSTOM_TRIM2)
	Fernbedienung::Pot::min_pot(trim_poti) = value;
    }
    
    lcd.print (Fernbedienung::Pot::getMappedPotValueCalculated(trim_poti, value));
    lcd.write (' ');
    
    lcd.setCursor (7, 0);
    lcd.print (min);
    lcd.setCursor (12, 0);
    lcd.print (max);
    
  }
  else if (mode == SAVE_DATA)
  {
    lcd.setCursor (7, 0);
    lcd.print (Fernbedienung::files.diskSpace());
    lcd.print ("/992");
  }
}

void Menu::writeStromAndSpannung(long unsigned int spannung, long signed int strom)
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
    //lcd.setCursor (0, 1);
    //writeCommaNumber (spannung * strom / 1000, "W");
    interval();
  }
}

void Menu::writeDrehzahl(long unsigned int drehzahl)
{
  static long unsigned int lastDrehzahl;
  static long unsigned int lastDrehzahlTime;
  using Fernbedienung::LcdHelper::lcd;
  if (mode == DREHZAHL)
  {
    lcd.clear();
    lcd.setCursor (0,0);
    lcd.print ("Dreh:");
    lcd.print (drehzahl);
    lcd.setCursor (0,1);
    lcd.print((drehzahl-lastDrehzahl)*1000ul/(millis()-lastDrehzahlTime));
    lcd.print("/s");
  }
  else if (mode == RUNNING)
  {
    lcd.setCursor (0, 1);
    lcd.print((drehzahl-lastDrehzahl)*1000ul/(millis()-lastDrehzahlTime));
    lcd.print("/s");
  }
  lastDrehzahl = drehzahl;
  lastDrehzahlTime = millis();
}

void Menu::writeMPPTData(char mpptData)
{
  if (mode == RUNNING)
  {
    using Fernbedienung::LcdHelper::lcd;
    lcd.setCursor (15, 0);
    lcd.write (mpptData);
  }
}

