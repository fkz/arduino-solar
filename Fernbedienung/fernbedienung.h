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
#include "../wireless.h"
#include <LiquidCrystal/LiquidCrystal.h>

class Fernbedienung: public MyXBee, public NDispatcher
{
  public:
    
    static const uint8_t LCD_RS = 2;
    static const uint8_t LCD_ENABLE = 3;
    static const uint8_t LCD_D0 = 8;
    static const uint8_t LCD_D1 = 9;
    static const uint8_t LCD_D2 = 10;
    static const uint8_t LCD_D3 = 11;
    
    static const uint8_t POT_SPEED = 1;
    static const uint8_t POT_TURN = 2;
    
    Fernbedienung();
  
  protected:
    virtual void error(uint8_t arg1);
    virtual void readData(uint8_t* data, uint8_t length);
    virtual void connectionInterrupted();
    
  private:
    LiquidCrystal lcd;
    bool isInitializing;
    void sendData ();
    
};
