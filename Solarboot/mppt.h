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

// die MPPT Algorithmen folgen http://www.solarbuildings.ca/c/sbn/file_db/Advanced%20algorithm%20for%20MPPT.pdf

class MyXBee;
class MPPT
{
  public:
    /**
     loops 
     @param strom gemessene aktuelle Stromstärke
     @param spannung gemessene aktuelle Spannung
     @return zum Motor-Servo gebender Wert
    */
    virtual int loop (int strom, int spannung) = 0;
    static void updateSpeed(uint8_t arg1)
    {
      speed = arg1;
    }
    
    virtual char getDisplayData () { return ' '; };
    virtual void setData (char data) { }
    
    static int diff;
    static int interval;
  protected:
    static uint8_t speed;
};

/// @short kein MPPT-Tracking
/// die Steuerung findet ganz normal statt
class NoMPPT: public MPPT
{
  public:
    virtual int loop(int strom, int spannung);
};

class ConstMPPT: public MPPT
{
public:
  ConstMPPT () : MPPT(), actualValue(128), refStrom(0), lastSpeed(0)  {}
  virtual int loop (int strom, int spannung);
  virtual char getDisplayData();
  virtual void setData(char data);
private:
  uint8_t refStromN;
  int actualValue;
  int refStrom;
  int lastSpeed;
};

class OptimizingMPPT: public MPPT
{
protected:
  bool isMPPT ();
  
private:
  bool isInMPPTMode;
};

class PerturbAndObserve: public OptimizingMPPT
{
  public:
    PerturbAndObserve ();
    
    virtual int loop(int strom, int spannung);
  private:
    int lastSpannung;
    int lastStrom;
    int lastServo;
};

class PerturbEstimate: public OptimizingMPPT
{
  public:
    PerturbEstimate();
    virtual int loop(int strom, int spannung);
  private:
    int8_t mode;
    
    // last 3 power p[0] = now, p[1]= now-1 p[2]=now-2
    long power[3];
    long spannung[3];
    int vRef;
};

class PerturbEstimateEstimate: public OptimizingMPPT
{
  public:
    PerturbEstimateEstimate();
    virtual int loop(int strom, int spannung);
    
  private:
    int8_t mode;
    
    // last 3 power p[0] = now, p[1]= now-1 p[2]=now-2
    long power[3];
    long spannung[3];
    int vRef;
    long int dP;

};
