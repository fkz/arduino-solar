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

class Fernbedienung: public MyXBee, public NDispatcher
{
  public:
    
    static const uint8_t LCD_RS = 2;
    static const uint8_t LCD_ENABLE = 3;
    static const uint8_t LCD_D0 = 8;
    static const uint8_t LCD_D1 = 9;
    static const uint8_t LCD_D2 = 10;
    static const uint8_t LCD_D3 = 11;
    
    static const uint8_t POT_SPEED = 0;
    static const uint8_t POT_TURN = 1;
    static const uint8_t BATTERY = 3;
    
    static const uint8_t POT_STEUERUNG = 2;
    static const int POT_STEUERUNG_DOWN = 450;
    static const int POT_STEUERUNG_SWITCH = 512;
    static const int POT_STEUERUNG_UP = 550;
    
    static const int PUSH_BUTTON_EXECUTE = 13;
    
    Fernbedienung();
    
  protected:
    virtual void error(uint8_t arg1);
    virtual void readData(uint8_t* data, uint8_t length);
    virtual void connectionInterrupted();
    
  private:
    LiquidCrystal lcd;
    Menu menu;
    void sendData ();
    void checkBatteryState();
    
    // STATE SECTION
    bool push_button_state;
    int8_t  pot_steuerung_state;
    void controlButtons();
    
    int TURN_MIN;
    int TURN_MAX;
    int SPEED_MIN;
    int SPEED_MAX;
};
