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
#include "serialize.h"
#include <WConstants.h>
#include <HardwareSerial.h>

int FileManagement::startRecord()
{
  if (actual != 0)
    endRecord();
  actual = memory.startSegment();
  actual->resize(255);
  long unsigned int act = millis();
  actual->write(0, act & 0xFF);
  actual->write(1, (act >> 8) & 0xFF);
  actual->write(2, (act >> 16) & 0xFF);
  actual->write(3, (act >> 24) & 0xFF);
  actual->write(4, 0);
  pointer = 5;
  lastTime = millis();
  lastDrehzahl = 0;
  return actual->getId();
}

void FileManagement::newData(int spannung, int strom, unsigned int long completeDrehzahl)
{
  actual->write (pointer++, spannung & 0xFF);
  actual->write (pointer++, ((spannung >> 8) & 0x0F) | ((strom >> 4) & 0xF0));
  actual->write (pointer++, strom & 0xFF);
  
  unsigned int drehzahl = completeDrehzahl - lastDrehzahl;
  lastDrehzahl = completeDrehzahl;
  
  long unsigned int now = millis();
  int diff = now - lastTime;
  lastTime = now;
  if (diff > 4095)
    diff = 1;
  
  actual->write (pointer++, drehzahl & 0xFF);
  actual->write (pointer++, ((drehzahl >> 8) & 0x0F) | ((diff >> 4) & 0xF0));
  actual->write (pointer++, diff & 0xFF);
}

void FileManagement::endRecord(uint8_t size, uint8_t* filename)
{
  if (actual == 0)
    return;
  
  for (int i = 0; i != size; ++i, ++filename, ++pointer)
  {
    actual->write (pointer, i);
  }
  actual->write (4, size);
  actual->resize (pointer);
  actual = 0;
}

void FileManagement::readAllCvs()
{
  int count = memory.getCount();
  for (int i = 0; i != count; ++i)
  {
    MemorySegment *segment = memory.getSegement(i);
    Serial.print ("Data Record ");
    Serial.print (i);
    
    unsigned long int date = (unsigned long)segment->read (0) | (unsigned long)segment->read (1) << 8 | (unsigned long)segment->read (2) << 16 | (unsigned long)segment->read (3) << 24;
    Serial.print ("Time: ");
    Serial.print (date);
    uint8_t filenamelength = segment->read (4);
    int segsize = segment->size();
    if (filenamelength == 0)
      Serial.println (" (No Name)");
    else
    {
      Serial.print (" Name: ");
      for (int i = segsize-filenamelength; i != segsize; ++i)
      {
	Serial.write (segment->read (i));
      }
      Serial.println();
    }
    Serial.println ("Zeit[ms],Drehzahl,Spannung[mV],Strom[mA]");
    for (int index = 5; index < segsize-filenamelength; index+=6)
    {
      uint8_t i1 = segment->read (index+1);
      int spannung = segment->read(index) | ((i1 & 0x0F) << 8);
      int strom = segment->read (index+2) | ((i1 & 0xF0) << 4);
      
      uint8_t i2 = segment->read (index+4);
      int drehzahl = segment->read(index+3) | ((i1 & 0x0F) << 8);
      int timediff = segment->read (index+5) | ((i1 & 0xF0) << 4);
      
      date += timediff;
      
      if (timediff == 1)
	Serial.println ("Error Wrong Time");
      
      unsigned long sp = (unsigned long)spannung * 15271 / 1000;
      unsigned long st = (unsigned long)strom * 26394 / 1000;
      Serial.print (date);
      Serial.write (',');
      Serial.print (drehzahl);
      Serial.write (',');
      Serial.print (sp);
      Serial.write (',');
      Serial.println (st);
    }
  }
}
