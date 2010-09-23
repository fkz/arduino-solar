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
#include "MemoryManagement.h"
#include <EEPROM/EEPROM.h>

// Memory management:

// Hex   Size    Data
// 0x00  0x1     Formatiert (Formatiert=0x27, sonst unformatiert)
// 0x01  0x1F    Länge der einzelnen Segmente, oder 0 falls leer
// 0x20  Rest    Beginn of Data


Memory::Memory()
{
  bool formatiert = EEPROM.read(0) == 0x27;
  if (!formatiert)
  {
    for (int i = 1; i != 0x20; ++i)
      EEPROM.write (i, 0);
    EEPROM.write (0, 0x27);
  }
}

int Memory::getCount()
{
  int result = 0;
  while (EEPROM.read (++result) != 0);
  return result-1;
}

MemorySegment* Memory::getSegement(int index)
{
  int startAddress = 0x20;
  for (int i = 1; i != index+1; ++i)
  {
    startAddress += EEPROM.read (i);
  }
  actual.reset (index, startAddress, EEPROM.read (index+1), false);
  return &actual;
}

MemorySegment* Memory::startSegment()
{
  int startAddress = 0x20;
  uint8_t size = 1;
  int index;
  for (index = 1; size!=0; ++index)
  {
    size = EEPROM.read (index);
    startAddress += size;
  }
  actual.reset (index-1, startAddress, 0, true);
  return &actual;
}

int Memory::diskSpace()
{
  int result = 0;
  for (int i = 1; i != 20; ++i)
  {
    uint8_t data = EEPROM.read (i);
    result += data;
    if (data == 0)
      break;
  }
  return result;
}


void MemorySegment::reset(uint8_t id, int startAddress, uint8_t size, bool resize)
{
  this->id = id;
  _size = size;
  this->startAddress = startAddress;
  resizeable = resize;
}

uint8_t MemorySegment::read(int index)
{
  EEPROM.read (index+startAddress);
}

void MemorySegment::write(int index, uint8_t data)
{
  EEPROM.write (index+startAddress, data);
}

void MemorySegment::resize(int newSize)
{
  if (resizeable)
  {
    _size = newSize;
    EEPROM.write (id+1, newSize);
  }
}
