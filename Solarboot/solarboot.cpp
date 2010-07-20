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
#include "solarboot.h"
#include "../Dispatcher.h"
#include <Servo/Servo.h>

Solarboot::Solarboot(): MyXBee(XBeeAddress64 (addressOfRemoteControl1, addressOfRemoteControl2))
{
  static NoMPPT noMPPT;
  mppt = &noMPPT;
  
  servoMotor.attach(motorId);
  servoTurn.attach(turnId);
  
  //FIXME: initialize motor
  servoMotor.write (90);
  delay (1000);
  servoMotor.write (0);
  delay (1000);
  servoMotor.write (180);
  delay (1000);
  
  //bleibe zunächst stehen
  servoMotor.write (90);
  
  
  addMethod(this, &Solarboot::readPackages, 0);
  addMethod(this, &Solarboot::sendData, 500);
  addMethod(this, &Solarboot::iterateMPPT, 2);
}

void Solarboot::error(uint8_t arg1)
{
  // simply do nothing for now
}

void Solarboot::sendData()
{
  int strom = analogRead (stromId);
  int spannung = analogRead (spannungId);
  
  uint8_t data[5];
  data[0] = 'S';
  data[1] = strom & 0xFF;
  data[2] = strom << 8;
  data[3] = spannung & 0xFF;
  data[4] = spannung << 8;
  
  writeData(data, sizeof(data));
}

void Solarboot::connectionInterrupted()
{
  analogWrite (motorId, 128);
  analogWrite (turnId, 128);
}


void Solarboot::readData(uint8_t* data, uint8_t length)
{
  if (length < 3)
  {
    error (100);
    return;
  }
  
  switch (data[0])
  {
    case 'F':
    {
      mppt->updateSpeed (data[1]);
      servoTurn.write (data[2]);
      break;
    }
  }
}

void Solarboot::iterateMPPT()
{
  int strom = analogRead (stromId);
  int spannung = analogRead (spannungId);
  int motor = mppt->loop(strom, spannung);
  servoMotor.write (motor);
}

int NoMPPT::loop(int strom, int spannung)
{
  return speed * 180 / 256;
}

PerturbAndObserve::PerturbAndObserve()
: lastServo(110), lastSpannung(0), lastStrom(0)
{
}

int PerturbAndObserve::loop(int strom, int spannung)
{
  if (speed > 150)
  {
    int power = strom * spannung;
    int lastPower = lastStrom * lastSpannung;
    if (power > lastPower)
    {
      if (spannung > lastSpannung)
	lastServo += diff;
      else
	lastServo -= diff;
    }
    else
    {
      if (spannung > lastSpannung)
	lastServo -= diff;
      else
	lastServo += diff;
    }
    
    if (lastServo < 95)
      lastServo += 10;
    else if (lastServo > 180)
      lastServo -= 10;
  }
  else
  {
    lastServo = speed * 180 / 256;
  }
  lastStrom = strom;
  lastSpannung = spannung;
  return lastServo;
}




