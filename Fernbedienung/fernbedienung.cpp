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
#include "fernbedienung.h"
#include "flags.h"
#include "pot_value.h"
#include "solarboot_data.h"

MyXBee Fernbedienung::xbee;
NDispatcher Fernbedienung::dispatcher;
LiquidCrystal lcd (Fernbedienung::LCD_RS, Fernbedienung::LCD_ENABLE, Fernbedienung::LCD_D0, Fernbedienung::LCD_D1, Fernbedienung::LCD_D2, Fernbedienung::LCD_D3);
//Menu Fernbedienung::menu (lcd);

Poti< Fernbedienung::POT_STEUERUNG_X, 400, 640 > Fernbedienung::Buttons::steuerungX;
Poti< Fernbedienung::POT_STEUERUNG_Y, 400, 640 > Fernbedienung::Buttons::steuerungY;
PushButton< Fernbedienung::STEUERUNG_PRESS > Fernbedienung::Buttons::steuerungPress;
FileManagement Fernbedienung::files; 
int Fernbedienung::recordId;


namespace Fernbedienung{
namespace Callback{
  
using Message::MessageData;
using namespace Message::FromSolarboat;
  
void dataFromSolarboat (const MessageData< DATA_FROM_SOLARBOAT > *data, uint8_t length);
void battery (const MessageData< Message::FromSolarboat::BATTERY > *data, uint8_t length);
void sendMPPT (const MessageData< SEND_MPPT > *data, uint8_t length);
void responseMPPTInterval (const MessageData< RESPONSE_MPPT_INTERVAL > *data, uint8_t length);
void connection (const MessageData< MyXBee::CONNECTION_INTERRUPTED > *data, uint8_t length);
void error (const MessageData< MyXBee::ERROR > *data, uint8_t length);

};
};

void Fernbedienung::initialize()
{
  dispatcher.addMethod(&sendData, 20);
  dispatcher.addMethod(&checkBatteryState, 60000);
  dispatcher.addMethod(menuInterval, 2000);
  dispatcher.addMethod(&Buttons::controlButtons, 0);
  
  lcd.begin(16,2);
  lcd.print (":)");
  
  using namespace Fernbedienung::Callback;
  using Message::MessageData;
  using namespace Message::FromSolarboat;
  
  xbee.registerMethod< DATA_FROM_SOLARBOAT >(dataFromSolarboat);
  xbee.registerMethod< Message::FromSolarboat::BATTERY >(battery);
  xbee.registerMethod< SEND_MPPT >(sendMPPT);
  xbee.registerMethod< RESPONSE_MPPT_INTERVAL >(responseMPPTInterval);
  xbee.registerMethod< MyXBee::CONNECTION_INTERRUPTED > (connection);
  xbee.registerMethod< MyXBee::ERROR > (error);
  
  Pot::initialize();
  pinMode(LED_PIN, OUTPUT);
}

void Fernbedienung::menuInterval()
{
  Menu::interval();
}

void Fernbedienung::Callback::error(const Message::MessageData< MyXBee::ERROR >* data, uint8_t length)
{
  lcd.setCursor(0,0);
  lcd.print ("ERR");
  lcd.print (data->errorNumber, 10);
}


void Fernbedienung::Callback::connection(const Message::MessageData< MyXBee::CONNECTION_INTERRUPTED >* data, uint8_t length)
{
  Flags::setFlag (Flags::CONNECTION, data->connected);
}

void Fernbedienung::Callback::dataFromSolarboat(const Message::MessageData< Message::FromSolarboat::DATA_FROM_SOLARBOAT >* data, uint8_t length)
{
  signed long strom = data->strom;
  unsigned long spannung = data->spannung;

  if (Flags::getFlag(Flags::RECORDING))
    files.newData(spannung, strom, data->freqCount);
  
  strom = 511 - strom;
  strom *= 26394;
  strom /= 1000;

  // 0 = 0V, 1024=(5V*(14,7)/4,7)=3.127*5=15,635
  // ==> 0,015267V pro Stelle
  spannung *= 15271;
  spannung /= 1000; // spannung in mV
  SolData::setActualMPPTType(data->mpptType);
  Menu::writeStromAndSpannung(spannung, strom);
  Menu::writeDrehzahl (data->freqCount);
  Menu::writeMPPTData (data->mpptData);
}

void Fernbedienung::Callback::battery(const Message::MessageData< Message::FromSolarboat::BATTERY >* data, uint8_t length)
{
  if (data->data == 0)
    Flags::setFlag (Flags::BATTERY_SOLARBOOT, true);
  else
  {
    SolData::setSolarBattery (data->data);
  }
}

void Fernbedienung::Callback::sendMPPT(const Message::MessageData< Message::FromSolarboat::SEND_MPPT >* data, uint8_t length)
{
  if (data->subtype == 'r')
    SolData::setMPPTDiff (data->data);
}

void Fernbedienung::Callback::responseMPPTInterval(const Message::MessageData< Message::FromSolarboat::RESPONSE_MPPT_INTERVAL >* data, uint8_t length)
{
  SolData::setMPPTInterval(data->interval);
}

void Fernbedienung::sendData ()
{
  xbee.writePackage< Message::ToSolarboat::POTI_DATA > 
    (Pot::getMappedPotValue (Pot::SPEED), Pot::getMappedPotValue(Pot::TURN) );
}


void Fernbedienung::Buttons::controlButtons()
{
  if (Menu::isStarted())
  {
    steuerungX.control();
    if (steuerungX.isUp())
      Menu::setAction(-1);
    else if (steuerungX.isDown())
      Menu::setAction(+1);
    
    steuerungY.control();
    if (steuerungY.isUp())
      Menu::goUp();
    
    if (steuerungPress.isPressed())
      Menu::setExecute();
  }
  else
  {
    if (steuerungPress.isPressed())
      Menu::start();
    else
    {
      steuerungX.control();
      if (steuerungX.isUp())
      {
	xbee.writePackage< Message::ToSolarboat::CHANGE_MPPT_TYPE > ((SolData::getActualMPPTType() + 1) % Message::MPPT::COUNT);
	SolData::setActualMPPTType(Message::MPPT::UNKNOWN);
      }
      else if (steuerungX.isDown())
      {
	xbee.writePackage< Message::ToSolarboat::DATA_TO_MPPT > (1);
      }
      steuerungY.control();
      if (steuerungY.isUp())
      {
      }
      else if (steuerungY.isDown())
      {
	if (Flags::getFlag (Flags::RECORDING))
	{
	  files.endRecord();
	  Flags::setFlag (Flags::RECORDING, false);
	}
	else
	{
	  recordId = files.startRecord();
	  Flags::setFlag (Flags::RECORDING, true);
	}
	
      }
    }
  }
}

void Fernbedienung::checkBatteryState ()
{
  int value = analogRead (BATTERY);
  if (value < MIN_BATTERY_VALUE)
    Flags::setFlag (Flags::BATTERY_FERNBEDIENUNG, true);
}