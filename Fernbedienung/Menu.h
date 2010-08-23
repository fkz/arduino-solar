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
    Menu (LiquidCrystal &lcd, MyXBee &xbee) : lcd (lcd), xbee (xbee), mode (RUNNING), flags (NONE), mppt (Message::MPPT::UNKNOWN) 
    { 
      POT_MIN[SPEED] = 379;
      POT_MAX[SPEED] = 648;
      POT_MIN[TURN] = 450;
      POT_MAX[TURN] = 649;
      //TODO: read values from EEPROM
    }
    
    /** 
     navigation in the menu, go left or right
     @param richtung should be -1 or 1 to go left/right
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
     @param spannung spannung in mV
     @param strom strom in mA
    */
    void writeStromAndSpannung (long unsigned int spannung, long unsigned int strom);
    
    /**
     writes the spannung with writeCommaNumber()
     this is for a value with a voltage divider 10+4.7 as 14.7:4.7 
     @param arg1 should be a value between 0 and 1023, as read by readAnalog()
    */
    void writeSpannung15(int arg1);
    
    /**
     sets the solar battery intensity. (only used if one is build in)
     @param value either 0, stands for "LOW BATTERY" or between 1 and 1023 which
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
    void setActualMPPTType(char arg1);
    char getActualMPPTType ()
    {
      return mppt;
    }
    
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
     @param arg1 number*1000
     @param str Einheit (which is appended)
    */
    void writeCommaNumber(long unsigned int arg1, const char* str);
    
    
  private:    
    LiquidCrystal &lcd;
    MyXBee &xbee;
    Mode mode;
    uint8_t actual;
    
    void highlight(char value);
    void activate (Mode m);
    
  public:

    
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
    
    Flags flags;
    char mppt;
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