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

#include "wireless.h"

MyXBee::MyXBee()
: alredyRead(0), _isConnected(false), read_count(0), registrantsCount(0)
{
  Serial.begin(MyXBee::baudRate);
}


bool MyXBee::isConnected()
{
  return _isConnected;
}

#ifdef REALLY_WORLD

void MyXBee::readPackages()
{
  long unsigned int now = millis();
  
  if (now - lastPackageRead > MAX_TIME_BETWEEN_TWO_REQUESTS && _isConnected)
  {
    _isConnected = false;
    connectionInterrupted();
  }
  
  int b = Serial.read ();
  while (b >= 0)
  {
    if (alredyRead == 0)
    {
      if (b == START_BYTE)
      {
	alredyRead = 1;
      }
      else
	error (NO_START_BYTE);
    }
    else if (alredyRead >= 1)
    {
      if (b == ESCAPE)
	package[alredyRead++-1] = Serial.read ()+32;
      else if (b == START_BYTE)
      {
	error (START_BYTE_INSIDE_MESSAGE);
	alredyRead = 1;
      }
      else
	package[alredyRead++-1] = b;
    }
    
    if (alredyRead >= 2 && package[0] > MAX_MESSAGE_LENGTH)
    {
      package[0] = MAX_MESSAGE_LENGTH;
      error (ERROR_TO_LONG_MESSAGE);
    }
    
    if (alredyRead >= 2 && alredyRead-2 == package[0])
    {
      readData(package+1, package[0]);
      alredyRead = 0;
      lastPackageRead = now;
      if (!_isConnected)
      {
	connectionRestored();
      }
      _isConnected = true;
    }
    
    b = Serial.read();
  }
  ++read_count;
}

void MyXBee::writeData(uint8_t* data, uint8_t length)
{
  Serial.write (START_BYTE);
  writeEscaped (length);
  for (uint8_t* it = data; it != data + length; ++it)
    writeEscaped (*it);
  
}

/*void MyXBee::writePackage(uint8_t type, uint8_t* data, uint8_t length)
{
  Serial.write (START_BYTE);
  writeEscaped (length+1);
  writeEscaped (type);
  for (uint8_t *it = data; it != data + length; ++it)
    writeEscaped (*it);
}*/


void MyXBee::writeEscaped(uint8_t arg1)
{
  if (arg1 == START_BYTE || arg1 == ESCAPE)
  {
    Serial.write(ESCAPE);
    Serial.write (arg1-32);
  }
  else
    Serial.write (arg1);
}

#else
j
void MyXBee::readPackages ()
{
  long unsigned int now = millis();
  
  if (now - lastPackageRead > MAX_TIME_BETWEEN_TWO_REQUESTS  && _isConnected)
  {
    _isConnected = false;
    connectionInterrupted();
  }
  
  int b = Serial.read ();
  while (b >= 0)
  {
    if (alredyRead == 0)
    {
      if (b == 's')
      {
	alredyRead = 1;
	Serial.println ("Start eines Pakets");
	package[0] = 100;
	++alredyRead;
      }
      else
      {
	error (NO_START_BYTE);
	Serial.println ("Start-Byte erwartet, aber etwas anderes gefunden");
      }
    }
    else if (alredyRead >= 1)
    {
      if (b == ESCAPE)
	package[alredyRead++-1] = Serial.read ()+32;
      else if (b == START_BYTE)
      {
	error (START_BYTE_INSIDE_MESSAGE);
	Serial.println ("Fehler: Start-Byte innerhalb Nachricht");
	alredyRead = 1;
      }
      else if (b == 'E')
      {
	package[0] = alredyRead-2;
	Serial.println ("Ende der Nachricht");
      }
      else
      {
	if (b == 'x')
	{
	  //lese Hexadezimalzahl
	  while (Serial.available() < 2);
	  b = Serial.read();
	  if (b >= 'A')
	    b = b - 'A' + 10;
	  else
	    b = b - '0';
	  int b2 = Serial.read();
	  if (b2 >= 'A')
	    b2 = b2 - 'A' + 10;
	  else
	    b2 = b2 - '0';
	  b = b << 4 | (b2 & 0xF);
	}
	package[alredyRead++-1] = b;
      }
    }
    
    if (alredyRead >= 2 && alredyRead-2 == package[0])
    {
      Serial.print ("Sende Nachricht (Länge: ");
      Serial.print (package[0], DEC);
      Serial.print ("): ");
      package[package[0]+2]=0;
      Serial.println ((char*)(package+1));
      readData(package+1, package[0]);
      alredyRead = 0;
      lastPackageRead = now;
      if (!_isConnected)
      {
	connectionRestored();
      }
      _isConnected = true;
    }
    
    b = Serial.read();
  }
  ++read_count;
}

void MyXBee::writeData (unsigned char *data, unsigned char length)
{
  Serial.print ("Paket emfangen(Länge:");
  Serial.print ((int)length);
  Serial.print ("): ");
  for (int i = 0; i != length; ++i)
    Serial.write (data[i]);
  Serial.println ();
}

void MyXBee::writeEscaped(uint8_t arg1)
{
  Serial.write (arg1-32);
}


#endif

void MyXBee::_registerMethod(uint8_t type, MyXBee::ReadPackage package)
{
  registrants[registrantsCount].type = type;
  registrants[registrantsCount].delegate = package;
  
  ++registrantsCount;
}


void MyXBee::readData(const uint8_t* data, uint8_t length)
{
  for (int i = 0; i != registrantsCount; ++i)
    if (registrants[i].type == data[0])
    {
      if (registrants[i].delegate)
	registrants[i].delegate (data+1, length-1);
      return;
    }
  //if the error message is not registered, this would lead to a stack overflow i. e. CRASH, so
  //it is greatly apprecated, that the error message is registred
  error (MESSAGE_NOT_REGISTERED);
}