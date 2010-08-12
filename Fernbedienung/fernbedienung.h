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
#include <wireless.h>
#include <LiquidCrystal/LiquidCrystal.h>
#include "Menu.h"

/**
@short Basisklasse für die Fernbedienung
*/
class Fernbedienung: public MyXBee, public NDispatcher
{
  public:
    
    static const uint8_t LCD_RS = 12;
    static const uint8_t LCD_ENABLE = 11;
    static const uint8_t LCD_D0 = 5;
    static const uint8_t LCD_D1 = 4;
    static const uint8_t LCD_D2 = 3;
    static const uint8_t LCD_D3 = 2;
    
    static const uint8_t POT_SPEED = 0;
    static const uint8_t POT_TURN = 1;
    static const uint8_t BATTERY = 3;
    static const int MIN_BATTERY_VALUE = 570;
    
    static const int POT_TURN_LOW = 500;
    static const int POT_TURN_HIGH = 600;
    static const int POT_SPEED_LOW = 400;
    static const int POT_SPEED_HIGH = 600;
    
    static const uint8_t POT_STEUERUNG = 2;
    static const int POT_STEUERUNG_1 = 400;
    static const int POT_STEUERUNG_2 = 500;
    static const int POT_STEUERUNG_3 = 600;
    static const int POT_STEUERUNG_4 = 900;
    
    
    Fernbedienung();
    
  protected:
    virtual void error(uint8_t arg1);
    virtual void readData(uint8_t* data, uint8_t length);
    virtual void connectionInterrupted();
    
  private:
    /**
     sends MPPT type to solar boat
     @p mpptType type as listed in MessageTypes.h
    */
    void setMPPT (char mpptType);
    
    
    LiquidCrystal lcd;
    Menu menu;
    
    /**
     sends data of speed and turn to solar boat 
    */
    void sendData ();
    void checkBatteryState();
    
    // STATE SECTION
    int8_t  pot_steuerung_state;
    int8_t  pot_speed_state;
    int8_t  pot_turn_state;
    void controlButtons();
};
