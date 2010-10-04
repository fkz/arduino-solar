#include "fernbedienung2.h"

void Fernbedienung::initialize()
{
  dispatcher.addMethod(&sendData, 20);
  dispatcher.addMethod(&checkBatteryState, 60000);
  dispatcher.addMethod(&Menu::interval, 2000);
  dispatcher.addMethod(&Buttons::controlButtons, 0);
  
  menu.initialize ();
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
	setMPPT ((menu.getActualMPPTType() + 1) % Message::MPPT::COUNT);
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
	sendMPPTDiff ((diff+1)%10);
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
