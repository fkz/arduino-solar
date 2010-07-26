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
#include "../Dispatcher.h"
#include <LiquidCrystal/LiquidCrystal.h>

class Menu: public Dispatcheable
{
  public:
    Menu (LiquidCrystal &lcd) : lcd (lcd), mode (RUNNING), flags (NONE), mppt (UNKNOWN) { }
    
    void setAction (int8_t richtung);
    void setExecute ();
    void writeData (int spannung, int strom);
    
    void setSolarBattery ()
    {
      flags = (Flags)(flags | BATTERY_SOLARBOOT);
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
    
  private:
    enum Mode 
    {
      RUNNING = 255,
      MAINMENU = 0,
      AKKU,
      TRIM,
      MPPT,
      MENU_COUNT,
      CUSTOM_TRIM
    };
    
    
    LiquidCrystal &lcd;
    Mode mode;
    uint8_t actual;
    
    void highlight(char value);
    void activate (Mode m);
    
    void chooseMPPT();
    
    enum Flags
    {
      NONE = 0,
      BATTERY_SOLARBOOT = 1,
      BATTERY_FERNBEDIENUNG = 2,
      CONNECTION = 4
    };
    
    enum MPPTType
    {
      UNKNOWN,
      NO_MPPT,
      PANDP,
      PE,
      PEE
    };
    
    MPPTType mppt;
    Flags flags;
};