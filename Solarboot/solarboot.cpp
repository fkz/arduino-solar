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

NoMPPT noMPPT;
PerturbAndObserve perturbAndObserve;
PerturbEstimate perturbEstimate;
PerturbEstimateEstimate perturbEstimateEstimate;

Solarboot::Solarboot()
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
  mpptRythm = addMethod(this, &Solarboot::iterateMPPT, 2);
  addMethod(this, &Solarboot::checkBattery, 60000);
}

void Solarboot::error(uint8_t arg1)
{
  // simply do nothing for now
}

void Solarboot::sendData()
{
  int strom = analogRead (stromId);
  int spannung = analogRead (spannungId);
  
  uint8_t data[6];
  data[0] = Message::FromSolarboat::DATA_FROM_SOLARBOAT;
  data[1] = strom & 0xFF;
  data[2] = strom >> 8;
  data[3] = spannung & 0xFF;
  data[4] = spannung >> 8;
  data[5] = mpptType;
  
  writeData(data, sizeof(data));
}

void Solarboot::connectionInterrupted()
{
  servoMotor.write(90);
  servoTurn.write (90);
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
    case Message::ToSolarboat::POTI_DATA:
    {
      mppt->updateSpeed (data[1]);
      servoTurn.write (data[2]);
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
      mppt->receiveData(*this, data+1, length-1);
      break;
    }
    case Message::ToSolarboat::REQUEST_MPPT_INTERVAL:
    {
      uint8_t d_data[3];
      d_data[0] = Message::FromSolarboat::RESPONSE_MPPT_INTERVAL;
      long unsigned int mpptInterval = getInterval(mpptRythm);
      d_data[1] = mpptInterval & 0xFF;
      d_data[2] = mpptInterval >> 8;
      writeData (d_data, 3);
      break;
    }
    case Message::ToSolarboat::SET_MPPT_INTERVAL:
    {
      setInterval(mpptRythm, data[1] | (data[2] << 8));
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
    default:
      mpptType = Message::MPPT::NOMPPT;
      mppt = &noMPPT;
      break;
  }
}


void Solarboot::iterateMPPT()
{
  int strom = analogRead (stromId);
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


