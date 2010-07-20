#include "solarboot.h"
#include "../Dispatcher.h"

Solarboot::Solarboot(): MyXBee(XBeeAddress64 (addressOfRemoteControl1, addressOfRemoteControl2))
{
  pinMode (motorId, OUTPUT);
  pinMode (turnId, OUTPUT);
  
  //FIXME: initialisiere Motor
  analogWrite (motorId, 128);
  delay (1000);
  analogWrite (motorId, 0);
  delay (1000);
  analogWrite (motorId, 255);
  delay (1000);
  
  //bleibe zunächst stehen
  analogWrite (motorId, 128);
  
  addMethod(this, static_cast< void (Dispatcheable::*) () > (&Solarboot::readPackages), 0);
  addMethod(this, static_cast< void (Dispatcheable::*) () > (&Solarboot::sendData), 500);
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
      //FIXME: maybe use Servo-Class instead
      analogWrite (motorId, data[1]);
      analogWrite (turnId, data[2]);
      break;
    }
  }
}

