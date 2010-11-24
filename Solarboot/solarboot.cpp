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
#include <Dispatcher.h>
#include <Servo/Servo.h>
#include "mppt.h"
#include <MessageTypes.h>
#include <FreqCounter/counter.h>

NoMPPT noMPPT;
PerturbAndObserve perturbAndObserve;
PerturbEstimate perturbEstimate;
PerturbEstimateEstimate perturbEstimateEstimate;
ConstMPPT constMppt;

Solarboot::Solarboot()
: mpptInterval (5)
{
  static NoMPPT noMPPT;
  mppt = &noMPPT;
  mpptType = Message::MPPT::NOMPPT;
  
  servoMotor.attach(motorId);
  servoTurn.attach(turnId);
  
  //FIXME: initialize motor
  servoMotor.write (90);
  delay (1000);

  //bleibe zunächst stehen
  servoMotor.write (90);
  
  
  addMethod(this, &Solarboot::readPackages, 0);
  addMethod(this, &Solarboot::sendData, 500);
  addMethod(this, &Solarboot::iterateMPPT, 20);
  addMethod(this, &Solarboot::checkBattery, 60000);
  
  Counter::initialize();
}

void Solarboot::error(uint8_t arg1)
{
  // simply do nothing for now
}

void Solarboot::sendData()
{
  long int strom = 0;
  for (int i = 0; i != readStromCount; ++i)
    strom += analogRead (stromId);
  strom /= readStromCount;
  int spannung = analogRead (spannungId);
  
  uint8_t data[10];
  data[0] = Message::FromSolarboat::DATA_FROM_SOLARBOAT;
  data[1] = strom & 0xFF;
  data[2] = strom >> 8;
  data[3] = spannung & 0xFF;
  data[4] = spannung >> 8;
  data[5] = mpptType;
  unsigned long int freqCount = Counter::getCount();
  data[6] = freqCount & 0xFF;
  data[7] = (freqCount & 0xFF00) >> 8;
  data[8] = (freqCount & 0xFF0000) >> 16;
  data[9] = freqCount >> 24;
  
  writeData(data, sizeof(data));
}

void Solarboot::connectionInterrupted()
{
  servoMotor.write(90);
  servoTurn.write (90);
  mppt->updateSpeed(128);
}


void Solarboot::readData(const uint8_t* data, uint8_t length)
{  
  switch (data[0])
  {
    case Message::ToSolarboat::POTI_DATA:
    {
      mppt->updateSpeed (data[1]);
      servoTurn.write (data[2]*45/64);
      break;
    }
    case Message::ToSolarboat::CHANGE_MPPT_TYPE:
    {
      mpptType = data[1];
      changeMPPT ();
      break;
    }
    case Message::ToSolarboat::REQUEST_BATTERY:
    {
      int value = analogRead (BATTERY);
      uint8_t d_data[3];
      d_data[0] = Message::FromSolarboat::BATTERY;
      d_data[1] = value & 0xFF;
      d_data[2] = value >> 8;
      writeData (d_data, 3);
      break;
    }
    case Message::ToSolarboat::REQUEST_MPPT:
    {
      mppt->receiveData(*this, const_cast< uint8_t * > (data+1), length-1); //TODO: remove const_cast, add const to receiveData method signature
      break;
    }
    case Message::ToSolarboat::REQUEST_MPPT_INTERVAL:
    {
      uint8_t d_data[3];
      d_data[0] = Message::FromSolarboat::RESPONSE_MPPT_INTERVAL;
      d_data[1] = mpptInterval;
      d_data[2] = 0;
      writeData (d_data, 3);
      break;
    }
    case Message::ToSolarboat::SET_MPPT_INTERVAL:
    {
      mpptInterval = data[1];
      break;
    }
  }
}

void Solarboot::changeMPPT()
{
  switch (mpptType)
  {
    case Message::MPPT::NOMPPT:
      mppt = &noMPPT;
      break;
    case Message::MPPT::PERTURBEANDOBSERVE:
      mppt = &perturbAndObserve;
      break;
    case Message::MPPT::ESTIMATEPERTURB:
      mppt = &perturbEstimate;
      break;
    case Message::MPPT::ESTIMATEESTIMATEPERTURB:
      mppt = &perturbEstimateEstimate;
      break;
    case Message::MPPT::CONSTMPPT:
      mppt = &constMppt;
      break;
    default:
      mpptType = Message::MPPT::NOMPPT;
      mppt = &noMPPT;
      break;
  }
}


void Solarboot::iterateMPPT()
{
  if ((mpptIntervalIndex = (mpptIntervalIndex + 1) % mpptInterval))
    return;
  
  long int strom = 0;
  for (int i = 0; i != readStromCount; ++i)
    strom += analogRead (stromId);
  strom /= readStromCount;
  int spannung = analogRead (spannungId);
  int motor = mppt->loop(strom, spannung);
  servoMotor.write (motor);
}

void Solarboot::checkBattery()
{
  int value = analogRead (BATTERY);
  if (value < 500)
  {
    uint8_t data[3];
    data[0] = Message::FromSolarboat::BATTERY;
    data[1] = 0;
    data[2] = 0;
    writeData(data, 3);
  }
}


