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
    CUSTOM_TRIM,
    CUSTOM_TRIM2,
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
  
  void interval();
  
  void writeStromAndSpannung(unsigned long spannung, signed long strom);
  void writeDrehzahl (unsigned long drehzahl);
  void writeMPPTData (char mpptData);
};