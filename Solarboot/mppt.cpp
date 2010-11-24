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
#include "mppt.h"
#include <MessageTypes.h>
#include <wireless.h>

int MPPT::diff = 5;
uint8_t MPPT::speed = 128;

void MPPT::receiveData(MyXBee& xbee, uint8_t* data, uint8_t size)
{
  if (data[0] == 'r')
  {
    uint8_t r_data[3];
    r_data[0] = Message::FromSolarboat::SEND_MPPT;
    r_data[1] = 'r';
    r_data[2] = diff;
    xbee.writeData(r_data, 3);
  }
  else if (data[0] == 's')
  {
    diff = data[1];
  }
}


int NoMPPT::loop(int strom, int spannung)
{
  return speed * 45 / 64;
}

bool OptimizingMPPT::isMPPT()
{
  bool mpptMode = speed < 100;
  if (mpptMode != isInMPPTMode)
  {
    isInMPPTMode = mpptMode;
    //TODO: sendData ()
  }
  return isInMPPTMode;
}


PerturbAndObserve::PerturbAndObserve()
:  lastSpannung(0), lastStrom(0), lastServo(110)
{
}

int PerturbAndObserve::loop(int strom, int spannung)
{
  if (isMPPT())
  {
    int power = strom * spannung;
    int lastPower = lastStrom * lastSpannung;
    if (power > lastPower)
    {
      if (spannung > lastSpannung)
	lastServo -= diff;
      else
	lastServo += diff;
    }
    else
    {
      if (spannung > lastSpannung)
	lastServo += diff;
      else
	lastServo -= diff;
    }
    
    if (lastServo < 100)
      lastServo += 2*diff;
    else if (lastServo > 180)
      lastServo -= 2*diff;
  }
  else
  {
    lastServo = speed * 45 / 64;
  }
  lastStrom = strom;
  lastSpannung = spannung;
  return lastServo;
}

PerturbEstimate::PerturbEstimate()
: mode (0), vRef (110)
{
  power[0] = 0;
  power[1] = 0;
  power[2] = 0;
  spannung[0] = 0;
  spannung[1] = 0;
  spannung[2] = 0;
}

PerturbEstimateEstimate::PerturbEstimateEstimate()
: mode (0), vRef (110)
{
  power[0] = 0;
  power[1] = 0;
  power[2] = 0;
  spannung[0] = 0;
  spannung[1] = 0;
  spannung[2] = 0;
}


int PerturbEstimate::loop(int strom, int sp)
{ 
  power[2] = power[1];
  power[1] = power[0];
  power[0] = strom * (long)spannung;
  
  spannung[2] = spannung[1];
  spannung[1] = spannung[0];
  spannung[0] = sp;
  
  if (!isMPPT ())
  {
    vRef = speed * 45 / 64;
  }
  else if (mode == 0)
  {
    mode = 1;
  }
  else
  {
    mode = 0;
    long int dP = power[0] - power[1];
    if (power[1] > power[2] - dP)
    {
      if (spannung[1] > spannung[2])
      {
	vRef += diff;
      }
      else
      {
	vRef -= diff;
      }
    }
    else
    {
      if (spannung[1] > spannung[2])
      {
	vRef -= diff;
      }
      else
      {
	vRef += diff;
      }
    }
  }
  
  if (vRef < 100)
    vRef += 2*diff;
  else if (vRef > 180)
    vRef -= 2*diff;
  
  return vRef;
}

int PerturbEstimateEstimate::loop(int strom, int sp)
{
  power[2] = power[1];
  power[1] = power[0];
  power[0] = strom * (long)spannung;
  
  spannung[2] = spannung[1];
  spannung[1] = spannung[0];
  spannung[0] = sp;
  
  if (!isMPPT ())
    vRef = speed * 45 / 64;
  else
  {
    switch (mode)
    {
      case 0:
	mode = 1;
	break;
      case 1:
	mode = 2;
	dP = power[0] - power[1];
	if (power[1] > power[2] - dP)
	{
	  if (spannung[1] > spannung[2])
	    vRef += diff;
	  else
	    vRef -= diff;
	}
	else
	{
	  if (spannung[1] > spannung[2])
	    vRef -= diff;
	  else
	    vRef += diff;
	}
	break;
      case 2:
	mode = 0;
	if (power[0] > power[1] - dP)
	{
	  if (spannung[0] > spannung[1])
	    vRef += diff;
	  else
	    vRef -= diff;
	}
	else
	{
	  if (spannung[0] > spannung[1])
	    vRef -= diff;
	  else
	    vRef += diff;
	}
    }
  }
  
  if (vRef < 100)
    vRef += 2*diff;
  else if (vRef > 180)
    vRef -= 2*diff;
  
  return vRef;
}

int ConstMPPT::loop(int strom, int spannung)
{
  if (lastSpeed != speed)
  {
    int8_t lastSpeedV = lastSpeed < 100 ? -1 : lastSpeed > 200 ? +1 : 0;
    int8_t speedV = speed < 100 ? -1 : speed > 200 ? +1 : 0;
    if (lastSpeedV != speedV && speedV != 0 && ( refSpannungN != 0 || speedV == 1 ) && (refSpannungN != 7 || speedV == -1))
    {
      refSpannungN += speedV;
      refSpannung = 511 - (long)refSpannungN * 1000000 / 26394 ;
    }
    lastSpeed = speed;
  }
  
  if (strom < refSpannung - 20)
    ++actualValue;
  else if (strom > refSpannung + 20)
    --actualValue;
  
  if (strom < refSpannung - 40)
    ++actualValue;
  else if (strom > refSpannung + 40)
    --actualValue;
  
  if (strom < refSpannung)
    ++actualValue;
  else
    --actualValue;
  return actualValue;
}

