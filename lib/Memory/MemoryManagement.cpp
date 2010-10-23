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

#ifdef SD
#include <sd-reader_source_20101010/sd_raw.h>
#endif

// Memory management:

// Hex   Size    Data
// 0x00  0x1     Formatiert (Formatiert=0x27, sonst unformatiert)
// 0x01  0x1F    Länge der einzelnen Segmente, oder 0 falls leer
// 0x20  Rest    Beginn of Data

inline uint8_t read (Memory::size_type index)
{
#ifndef SD
  return EEPROM.read(index);
#else
  uint8_t buffer;
  sd_raw_read (index, &buffer, 1);
  return buffer;
#endif
}

inline void write (Memory::size_type index, uint8_t data)
{
#ifndef SD
  EEPROM.write (index, data);
#else
  sd_raw_write (index, &data, 1);
#endif
}

Memory::Memory()
{
#ifdef SD
  sd_raw_init();
  sd_raw_info info;
  sd_raw_get_info(&info);
  capacity = info.capacity() - 32;
#else
  capacity = 1024-32;
#endif
  
  bool formatiert = read(0) == 0x27;
  if (!formatiert)
  {
    for (int i = 1; i != 0x20; ++i)
      write (i, 0);
    write (0, 0x27);
  }
}

int Memory::getCount()
{
  size_type result = 0;
  while (read (++result) != 0);
  return result-1;
}

MemorySegment* Memory::getSegement(int index)
{
  size_type startAddress = 0x20;
  for (int i = 1; i != index+1; ++i)
  {
    startAddress += read (i);
  }
  actual.reset (index, startAddress, read (index+1), false);
  return &actual;
}

MemorySegment* Memory::startSegment()
{
  size_type startAddress = 0x20;
  uint8_t size = 1;
  int index;
  for (index = 1; size!=0; ++index)
  {
    size = read (index);
    startAddress += size;
  }
  actual.reset (index-2, startAddress, 0, true);
  return &actual;
}

Memory::size_type Memory::diskSpace()
{
  size_type result = 0;
  for (int i = 1; i != 20; ++i)
  {
    uint8_t data = read (i);
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
  return read (index+startAddress);
}

void MemorySegment::write(int index, uint8_t data)
{
  write (index+startAddress, data);
}

void MemorySegment::resize(size_type newSize)
{
  if (resizeable)
  {
    _size = newSize;
    write (id+1, newSize);
  }
}
