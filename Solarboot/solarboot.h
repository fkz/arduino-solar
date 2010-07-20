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