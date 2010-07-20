#pragma once
#include "../wireless.h"
#include <LiquidCrystal/LiquidCrystal.h>

class Fernbedienung: public MyXBee, public NDispatcher
{
  public:
    
    static const uint8_t LCD_RS = 12;
    static const uint8_t LCD_ENABLE = 11;
    static const uint8_t LCD_D0 = 5;
    static const uint8_t LCD_D1 = 4;
    static const uint8_t LCD_D2 = 3;
    static const uint8_t LCD_D3 = 2;
    
    static const uint32_t receiver1 = 0;
    static const uint32_t receiver2 = 0;

    Fernbedienung();
  
  protected:
    virtual void error(uint8_t arg1);
    virtual void readData(uint8_t* data, uint8_t length);
    virtual void connectionInterrupted();
    
  private:
    LiquidCrystal lcd;
    void sendData ();
};