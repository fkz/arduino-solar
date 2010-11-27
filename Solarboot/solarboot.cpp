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

MyXBee Solarboot::xbee;
NDispatcher Solarboot::dispatcher;

namespace Solarboot{
// number of 20ms-states until MPPT is used
int mpptInterval, mpptIntervalIndex;

void sendData ();
void iterateMPPT ();
MPPT *mppt;
char mpptType;
Servo servoMotor, servoTurn;
UltraSound left (3), right (4);

void messureDistance ();

namespace Callback{
  
using Message::MessageData;
using namespace Message::ToSolarboat;

void potiData (const MessageData< POTI_DATA > *data, uint8_t length);
void changeMPPTType (const MessageData< CHANGE_MPPT_TYPE > *data, uint8_t length);
void requestBattery (const MessageData< REQUEST_BATTERY > *data, uint8_t length);
void dataToMPPT(const MessageData< DATA_TO_MPPT > *data, uint8_t length);
void error (const MessageData< MyXBee::ERROR > *data, uint8_t length);
void connectionInterrupted (const MessageData< MyXBee::CONNECTION_INTERRUPTED > *data, uint8_t length);

};
};


void Solarboot::initialize()
{
  mpptInterval = 5;
  
  mppt = &noMPPT;
  mpptType = Message::MPPT::NOMPPT;
  
  servoMotor.attach(motorId);
  servoTurn.attach(turnId);
  
  servoMotor.write (90);
  
  dispatcher.addMethod(&Solarboot::sendData, 500);
  dispatcher.addMethod(&Solarboot::iterateMPPT, 300);
  dispatcher.addMethod(&Solarboot::checkBattery, 60000);
  dispatcher.addMethod(&Solarboot::messureDistance, 1000);

  using namespace Message::ToSolarboat;
  using namespace Callback;
  
  xbee.registerMethod< POTI_DATA >(&potiData);
  xbee.registerMethod< CHANGE_MPPT_TYPE >(&changeMPPTType);
  xbee.registerMethod< REQUEST_BATTERY > (&requestBattery);
  xbee.registerMethod< DATA_TO_MPPT >(&dataToMPPT);
  xbee.registerMethod< MyXBee::ERROR >(&error);
  xbee.registerMethod< MyXBee::CONNECTION_INTERRUPTED > (&connectionInterrupted);
  
  Counter::initialize();
}

void Solarboot::Callback::error(const Message::MessageData< MyXBee::ERROR >* data, uint8_t length)
{
  // simply do nothing for now
}

void Solarboot::Callback::connectionInterrupted(const Message::MessageData< MyXBee::CONNECTION_INTERRUPTED >* data, uint8_t length)
{
  servoMotor.write(90);
  servoTurn.write (90);
  mppt->updateSpeed(128);
}

void Solarboot::sendData()
{
  long int strom = 0;
  for (int i = 0; i != readStromCount; ++i)
    strom += analogRead (stromId);
  strom /= readStromCount;
  int spannung = analogRead (spannungId);
  xbee.writePackage< Message::FromSolarboat::DATA_FROM_SOLARBOAT > (strom, spannung, mpptType, Counter::getCount(), mppt->getDisplayData ());
}

void Solarboot::Callback::potiData(const Message::MessageData< Message::ToSolarboat::POTI_DATA >* data, uint8_t length)
{
  mppt->updateSpeed (data->speed);
  servoTurn.write (data->turn);
}

void Solarboot::Callback::changeMPPTType(const Message::MessageData< Message::ToSolarboat::CHANGE_MPPT_TYPE >* data, uint8_t length)
{
  mpptType = data->mpptType;
  changeMPPT ();
}

void Solarboot::Callback::requestBattery(const Message::MessageData< Message::ToSolarboat::REQUEST_BATTERY >* data, uint8_t length)
{
  int value = analogRead (BATTERY);
  xbee.writePackage< Message::FromSolarboat::BATTERY > (value);
}


void Solarboot::Callback::dataToMPPT(const Message::MessageData< Message::ToSolarboat::DATA_TO_MPPT >* data, uint8_t length)
{
  mppt->setData (data->data);
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
    xbee.writePackage< Message::FromSolarboat::BATTERY > (0);
  }
}


void Solarboot::messureDistance()
{
  UltraSound::messure();
  if (left.getDistanceMm() < 1000 || right.getDistanceMm() < 1000)
  {
    xbee.writePackage< Message::FromSolarboat::DISTANCE > (left.getDistanceMm(), right.getDistanceMm());
  }
}
