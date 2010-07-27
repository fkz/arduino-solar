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
#pragma once
#include <Dispatcher.h>
#include <LiquidCrystal/LiquidCrystal.h>
#include <wireless.h>
#include <MessageTypes.h>

class Menu: public Dispatcheable
{
  public:
    Menu (LiquidCrystal &lcd, MyXBee &xbee) : lcd (lcd), xbee (xbee), mode (RUNNING), flags (NONE), mppt (UNKNOWN) { }
    
    void setAction (int8_t richtung);
    void setExecute ();
    void writeData (int spannung, int strom);
    
    void setSolarBattery (int value = 0)
    {
      if (value == 0)
	flags = (Flags)(flags | BATTERY_SOLARBOOT);
      else
	battery_solarboot = value;
    }
    
    void setFernBattery ()
    {
      flags = (Flags)(flags | BATTERY_FERNBEDIENUNG);
    }
    
    void setConnection (bool connection)
    {
      flags = (Flags)((flags & 3) | (connection << 2));      
    }
    
    void interval ();
    void setActualMPPTType(uint8_t arg1);
    
    enum Mode 
    {
      RUNNING = 255,
      MAINMENU = 0,
      AKKU,
      TRIM,
      MPPT,
      MPPT_DATA_SEND,
      MENU_COUNT,
      CUSTOM_TRIM
    };
    
  private:    
    LiquidCrystal &lcd;
    MyXBee &xbee;
    Mode mode;
    uint8_t actual;
    
    void highlight(char value);
    void activate (Mode m);
    
    void chooseMPPT();
    char getMPPTChar(uint8_t arg1);
    
    enum Flags
    {
      NONE = 0,
      BATTERY_SOLARBOOT = 1,
      BATTERY_FERNBEDIENUNG = 2,
      CONNECTION = 4
    };
    
    enum MPPTType
    {
      UNKNOWN = Message::MPPT_UNKNOWN,
      NO_MPPT = Message::MPPT_NOMPPT,
      PANDP = Message::MPPT_PERTURBEANDOBSERVE,
      PE = Message::MPPT_ESTIMATEPERTURB,
      PEE = Message::MPPT_ESTIMATEESTIMATEPERTURB
    };
    
    Flags flags;
    MPPTType mppt;
    int battery_solarboot;
};