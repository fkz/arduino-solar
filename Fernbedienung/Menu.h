#pragma once
#include "../Dispatcher.h"
#include <LiquidCrystal/LiquidCrystal.h>

class Menu: public Dispatcheable
{
  public:
    Menu (LiquidCrystal &lcd) : lcd (lcd), mode (RUNNING) { }
    
    void setAction (int8_t richtung);
    void setExecute ();
    void writeData (int spannung, int strom);
    
  private:
    enum Mode 
    {
      RUNNING = 255,
      MAINMENU = 0,
      AKKU,
      TRIM,
      MPPT,
      MENU_COUNT,
      CUSTOM_TRIM
    };
    
    
    LiquidCrystal &lcd;
    Mode mode;
    uint8_t actual;
    
    void highlight(char value);
    void activate (Mode m);
    
    void chooseMPPT();
};