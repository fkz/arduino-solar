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
: _isConnected(false), alredyRead(0), read_count(0)
{
  //FIXME: baud rate
  Serial.begin(9600);
}


bool MyXBee::isConnected()
{
  return _isConnected;
}

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
    
    if (alredyRead >= 2 && alredyRead-2 == package[0])
    {
      writeData(package+1, package[0]);
      alredyRead = 0;
      lastPackageRead = now;
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