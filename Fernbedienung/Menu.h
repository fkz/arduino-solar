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
#include <EEPROM/EEPROM.h>
#include <wireless.h>
#include <MessageTypes.h>
#include <Memory/serialize.h>

namespace Menu
{       
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
    SAVE_DATA,
    DREHZAHL,
    MENU_COUNT
  };
  
  void highlight(char value);
  void activate (Mode m);
  
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
  
  inline void start()
  {
    activate (MAINMENU);
  }
  inline void end ()
  {
    activate (RUNNING);
  }
  bool isStarted();
  
  bool halt (unsigned long int until);
  
  void interval();
  
  void writeStromAndSpannung(unsigned long spannung, unsigned long strom);
  void writeDrehzahl (unsigned long drehzahl);
};
    

#if false
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
    
    
    void flipPage ()
    {
      ++interval_page;
      interval_page %= 2;
      interval();
    }
    
    void setPage(int arg1)
    {
      interval_page = arg1;
      interval();
    }

    
    /**
     this method is regularily called and updates the menu
    */
    void interval ();
    
   
    
    
    /**
     writes a number on lcd with two digits after comma
     @param arg1 number*1000
     @param str Einheit (which is appended)
    */
    void writeCommaNumber(long unsigned int arg1, const char* str);
    
    
    LiquidCrystal &lcd;
    uint8_t actual;
    FileManagement files;    

    
   void writePotToEEPROM();
    
    void startRecord ();
    void endRecord ();
    void writeRawStromAndSpannung(int spannung, int strom);
    void startStopRecord();
    
        
    char mppt;
    int battery_solarboot;
    
    uint8_t mppt_diff;
    unsigned int mppt_act;
    unsigned int mppt_interval;
    
    int POT_MIN[2];
    int POT_MAX[2];
    
    uint8_t trim_poti;
    int min_pot_backup;
    int max_pot_backup;
    int pot_value(uint8_t poti);
    int &max_pot(uint8_t poti);
    int &min_pot(uint8_t poti);
    
    int interval_page;
};

#endif