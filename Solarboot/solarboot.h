#pragma once
#include "../wireless.h"
#include <LiquidCrystal/LiquidCrystal.h>
#include "../Dispatcher.h"
#include <Servo/Servo.h>

class MPPT: public Dispatcheable
{
  public:
    /**
     loops 
     @param strom gemessene aktuelle Stromstärke
     @param spannung gemessene aktuelle Spannung
     @return zum Motor-Servo gebender Wert
    */
    virtual int loop (int strom, int spannung) = 0;
    void updateSpeed(uint8_t arg1)
    {
      speed = arg1;
    }
  protected:
    uint8_t speed;
};

class NoMPPT: public MPPT
{
  public:
    virtual int loop(int strom, int spannung);
};

class PerturbAndObserve: public MPPT
{
  public:
    static const int diff = 5;
    
    PerturbAndObserve ();
    
    virtual int loop(int strom, int spannung);
  private:
    int lastSpannung;
    int lastStrom;
    int lastServo;
};

class Solarboot: public MyXBee, public NDispatcher
{
  public:
    static const int motorId = 9;
    static const int turnId = 10;
    
    static const int stromId = 0;
    static const int spannungId = 1;
    
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
    void iterateMPPT ();
    MPPT *mppt;
    Servo servoMotor, servoTurn;
};