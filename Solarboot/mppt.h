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
    virtual void receiveData (MyXBee &xbee, uint8_t *data, uint8_t size);
    static void updateSpeed(uint8_t arg1)
    {
      speed = arg1;
    }
    
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

class PerturbAndObserve: public MPPT
{
  public:
    PerturbAndObserve ();
    
    virtual int loop(int strom, int spannung);
  private:
    int lastSpannung;
    int lastStrom;
    int lastServo;
};

class PerturbEstimate: public MPPT
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

class PerturbEstimateEstimate: public MPPT
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
