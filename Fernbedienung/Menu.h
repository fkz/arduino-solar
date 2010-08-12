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
    Menu (LiquidCrystal &lcd, MyXBee &xbee) : lcd (lcd), xbee (xbee), mode (RUNNING), flags (NONE), mppt (UNKNOWN) 
    { 
      POT_MIN[SPEED] = 379;
      POT_MAX[SPEED] = 648;
      POT_MIN[TURN] = 450;
      POT_MAX[TURN] = 649;
      //TODO: read values from EEPROM
    }
    
    /** 
     navigation in the menu, go left or right
     @p richtung should be -1 or 1 to go left/right
    */
    void setAction (int8_t richtung);
    /**
     "ok" clicked
    */
    void setExecute ();
    /**
     go down
    */
    void goUp ();
    /**
     updates strom, spannung and power
     @p spannung spannung in mV
     @p strom strom in mA
    */
    void writeStromAndSpannung (long int spannung, long int strom);
    
    /**
     writes the spannung with writeCommaNumber()
    */
    void writeSpannung15(int arg1);
    
    /**
     sets the solar battery intensity. (only used if one is build in)
     @p value either 0, stands for "LOW BATTERY" or between 1 and 1023 which
        is the value returned by analogRead().
    */
    void setSolarBattery (int value = 0)
    {
      if (value == 0)
	flags = (Flags)(flags | BATTERY_SOLARBOOT);
      else
	battery_solarboot = value;
    }
    
    /**
     sets the flag of the battery. Should be called, if the battery is low
    */
    void setFernBattery ()
    {
      flags = (Flags)(flags | BATTERY_FERNBEDIENUNG);
    }
    
    /**
     sets the connection flag
    */
    void setConnection (bool connection)
    {
      flags = (Flags)((flags & 3) | (connection << 2));      
    }
    
    /**
     this method is regularily called and updates the menu
    */
    void interval ();
    
    /**
     sets the actual mppt type as returned by the boat
    */
    void setActualMPPTType(uint8_t arg1);
    
    enum Mode 
    {
      RUNNING = 255,
      MAINMENU = 0,
      AKKU,
      TRIM,
      MPPT,
      MPPT_SET_INTERVAL,
      CUSTOM_TRIM,
      CUSTOM_TRIM2,
      MPPT_SET_DIFF,
      MENU_COUNT
    };
    
    enum Poti
    {
      SPEED,
      TURN
    };
    
    uint8_t getPotiValue (Poti poti);
    void setMPPTDiff(uint8_t arg1)
    {
      mppt_diff = arg1;
    }
    
    /**
     writes a number on lcd with two digits after comma
     @p arg1 number*1000
     @p str Einheit (which is appended)
    */
    void writeCommaNumber(long int arg1, const char* str);
    
    
  private:    
    LiquidCrystal &lcd;
    MyXBee &xbee;
    Mode mode;
    uint8_t actual;
    
    void highlight(char value);
    void activate (Mode m);
    
  public:
    //void chooseMPPT();
    void start()
    {
      activate (MAINMENU);
    }
    void end ()
    {
      activate (RUNNING);
    }
    bool isStarted()
    {
      return mode != RUNNING;
    }
    
  private:
    
    enum Flags
    {
      NONE = 0,
      BATTERY_SOLARBOOT = 1,
      BATTERY_FERNBEDIENUNG = 2,
      CONNECTION = 4,
      FLAGS_ALL = 7
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
    
    uint8_t mppt_diff;
    uint8_t mppt_act;
    uint8_t mppt_interval;
    
    int POT_MIN[2];
    int POT_MAX[2];
    
    uint8_t trim_poti;
    int min_pot_backup;
    int max_pot_backup;
    int pot_value(uint8_t poti);
    int &max_pot(uint8_t poti);
    int &min_pot(uint8_t poti);
};