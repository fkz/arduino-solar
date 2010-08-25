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
#include "Poti.h"


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
    
    static const uint8_t POT_STEUERUNG_X = 2;
    static const uint8_t POT_STEUERUNG_Y = 4;
    static const uint8_t STEUERUNG_PRESS = 10;
    
    Fernbedienung();
    void sendMPPTDiff(uint8_t arg1);
    void sendMPPTInterval(unsigned int arg1);
    
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
    
    // Potis
    Poti< POT_STEUERUNG_X, 500, 524 > steuerungX;
    Poti< POT_STEUERUNG_Y, 500, 524 > steuerungY;
    PushButton< STEUERUNG_PRESS > steuerungPress;
    
    void controlButtons();    
};
