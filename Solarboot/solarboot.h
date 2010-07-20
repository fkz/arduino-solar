#pragma once
#include "../wireless.h"
#include <LiquidCrystal/LiquidCrystal.h>
#include "../Dispatcher.h"

class Solarboot: public MyXBee, public NDispatcher
{
  public:
    /// @defgroup Pins Pins im Solarboot Arduino
    // @{
    static const int motorId = 5;
    static const int turnId = 6;
    
    static const int stromId = 0;
    static const int spannungId = 1;
    // @}
    
    static const uint32_t addressOfRemoteControl1 = 0x2;
    static const uint32_t addressOfRemoteControl2 = 0x1;

     /// initialize pin modes and XBee connection
    Solarboot();
    
    
    
  protected:
    virtual void error(uint8_t arg1);
    virtual void readData(uint8_t* data, uint8_t length);
    virtual void connectionInterrupted();
    
    
  private:
    void sendData ();
};