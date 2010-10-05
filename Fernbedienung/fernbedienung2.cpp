#include "fernbedienung2.h"

MyXBee Fernbedienung::xbee;
NDispatcher Fernbedienung::dispatcher;
LiquidCrystal lcd (Fernbedienung::LCD_RS, Fernbedienung::LCD_ENABLE, Fernbedienung::LCD_D0, Fernbedienung::LCD_D1, Fernbedienung::LCD_D2, Fernbedienung::LCD_D3);
Menu Fernbedienung::menu (lcd);

Poti< Fernbedienung::POT_STEUERUNG_X, 400, 640 > Fernbedienung::Buttons::steuerungX;
Poti< Fernbedienung::POT_STEUERUNG_Y, 400, 640 > Fernbedienung::Buttons::steuerungY;
PushButton< Fernbedienung::STEUERUNG_PRESS > Fernbedienung::Buttons::steuerungPress;

namespace Fernbedienung{
namespace Callback{
  
using Message::MessageData;
using namespace Message::FromSolarboat;
  
void dataFromSolarboat (const MessageData< DATA_FROM_SOLARBOAT > *data, uint8_t length);
void battery (const MessageData< Message::FromSolarboat::BATTERY > *data, uint8_t length);
void sendMPPT (const MessageData< SEND_MPPT > *data, uint8_t length);
void responseMPPTInterval (const MessageData< RESPONSE_MPPT_INTERVAL > *data, uint8_t length);
void connection (const MessageData< MyXBee::CONNECTION_INTERRUPTED > *data, uint8_t length);

};
};


void Fernbedienung::initialize()
{
  dispatcher.addMethod(&sendData, 20);
  dispatcher.addMethod(&checkBatteryState, 60000);
  dispatcher.addMethod(menuInterval, 2000);
  dispatcher.addMethod(&Buttons::controlButtons, 0);
  
  lcd.begin(16,2);
  lcd.print ("Initialisiere2");
  
  using namespace Fernbedienung::Callback;
  using Message::MessageData;
  using namespace Message::FromSolarboat;
  
  xbee.registerMethod< DATA_FROM_SOLARBOAT >(dataFromSolarboat);
  xbee.registerMethod< Message::FromSolarboat::BATTERY >(battery);
  xbee.registerMethod< SEND_MPPT >(sendMPPT);
  xbee.registerMethod< RESPONSE_MPPT_INTERVAL >(responseMPPTInterval);
  xbee.registerMethod< MyXBee::CONNECTION_INTERRUPTED > (connection);
  
  
//  menu.initialize ();
}

void Fernbedienung::menuInterval()
{
  menu.interval();
}

void Fernbedienung::Callback::connection(const Message::MessageData< MyXBee::CONNECTION_INTERRUPTED >* data, uint8_t length)
{
  menu.setConnection(data->connected);
}

void Fernbedienung::Callback::dataFromSolarboat(const Message::MessageData< Message::FromSolarboat::DATA_FROM_SOLARBOAT >* data, uint8_t length)
{
  unsigned long strom = data->strom;
  if (strom < 512)
    strom = 512 - strom;
  else
    strom -= 512;
  unsigned long spannung = data->spannung;

  menu.writeRawStromAndSpannung (strom, spannung);

  //FIXME: set correct factor
  strom *= 26394;
  strom /= 1000;

  // 0 = 0V, 1024=(5V*(14,7)/4,7)=3.127*5=15,635
  // ==> 0,015267V pro Stelle
  spannung *= 15271;
  spannung /= 1000; // spannung in mV
  menu.setActualMPPTType (data->mpptType);
  menu.writeStromAndSpannung(spannung, strom);
}

void Fernbedienung::Callback::battery(const Message::MessageData< Message::FromSolarboat::BATTERY >* data, uint8_t length)
{
  menu.setSolarBattery(data->data);
}

void Fernbedienung::Callback::sendMPPT(const Message::MessageData< Message::FromSolarboat::SEND_MPPT >* data, uint8_t length)
{
  if (data->subtype == 'r')
    menu.setMPPTDiff (data->data);
}

void Fernbedienung::Callback::responseMPPTInterval(const Message::MessageData< Message::FromSolarboat::RESPONSE_MPPT_INTERVAL >* data, uint8_t length)
{
  menu.setMPPTInterval(data->interval);
}

void Fernbedienung::sendData ()
{
  xbee.writePackage< Message::ToSolarboat::POTI_DATA > 
    (menu.getPotiValue(Menu::SPEED), menu.getPotiValue(Menu::TURN) );
}


void Fernbedienung::Buttons::controlButtons()
{
  if (menu.isStarted())
  {
    steuerungX.control();
    if (steuerungX.isUp())
      menu.setAction(-1);
    else if (steuerungX.isDown())
      menu.setAction(+1);
    
    steuerungY.control();
    if (steuerungY.isUp())
      menu.goUp();
    
    if (steuerungPress.isPressed())
      menu.setExecute();
  }
  else
  {
    if (steuerungPress.isPressed())
      menu.start();
    else
    {
      steuerungX.control();
      if (steuerungX.isUp())
      {
	xbee.writePackage< Message::ToSolarboat::CHANGE_MPPT_TYPE > ((menu.getActualMPPTType() + 1) % Message::MPPT::COUNT);
	menu.setActualMPPTType(Message::MPPT::UNKNOWN);
      }
      else if (steuerungX.isDown())
      {
	menu.flipPage();
      }
      steuerungY.control();
      if (steuerungY.isUp())
      {
	uint8_t diff = menu.getMPPTDiff();
	xbee.writePackage< Message::ToSolarboat::REQUEST_MPPT > ('s', (diff+1)%10);
	menu.setMPPTDiff(255);
	menu.setPage (1);
      }
      else if (steuerungY.isDown())
      {
	/*unsigned int inte = menu.getMPPTInterval();
	inte += 50;
	if (inte > 1000)
	  inte = 100;
	sendMPPTInterval (inte);
	menu.setMPPTInterval(65535);
	menu.setPage(1);*/
	menu.startStopRecord();
	
      }
    }
  }
}

void Fernbedienung::checkBatteryState ()
{
  int value = analogRead (BATTERY);
  if (value < MIN_BATTERY_VALUE)
    menu.setFernBattery();

}