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
#include <avr/eeprom.h>

#ifdef SD
#include <sd-reader_source_20101010/sd_raw.h>
#endif

// Memory management:

// Hex   Size    Data
// 0x00  0x1     Formatiert (Formatiert=0x27, sonst unformatiert)
// 0x01  0x1F    Länge der einzelnen Segmente, oder 0 falls leer
// 0x20  Rest    Beginn of Data

#ifndef SD

struct EEPROMEntry
{
  int address;
  uint8_t value;
  
  bool isSet()
  {
    return address == -1;
  }
};

class EEPROMList
{
private:
  static const int size = 10;
  
  EEPROMEntry entrys[size];
  
  int start, end;
  
public:
  EEPROMList () : start(0), end(0) { }
  
  bool isEmpty()
  {
    return start == end;
  }
  
  void push (int address, uint8_t value)
  {
    int end2 = end+1;
    if (end2 == size)
      end2 = 0;
    
    if (end2 == start)
      pop();
    
    entrys[end].address = address;
    entrys[end].value = value;
    
    end = end2;    
  }
  
  void pop ()
  {
    if (!isEmpty())
    {
      EEPROM.write (entrys[start].address, entrys[start].value);
      ++start;
      if (start == size)
	start = 0;
    }
  }
};


EEPROMList eeprom;

void EEPROMdispatcher()
{
  if (eeprom_is_ready())
    eeprom.pop();
}
#endif


inline uint8_t _read (int index)
{
#ifndef SD
  return EEPROM.read(index);
#else
  uint8_t buffer;
  sd_raw_read (index, &buffer, 1);
  return buffer;
#endif
}


inline void _write (int index, uint8_t data)
{
#ifndef SD
  if (index < Memory::EEPROM_SIZE)
  {
    if (eeprom_is_ready())
      EEPROM.write (index, data);
    else
    {
      eeprom.push (index, data);
    }
  }
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
  
  format();
}

void Memory::format()
{
  bool formatiert = _read(0) == 0x27;
  if (!formatiert)
  {
    for (int i = 1; i != 0x20; ++i)
      _write (i, 0);
    _write (0, 0x27);
  }
}

void Memory::clear()
{
  _write (0, 0);
  format();
}


int Memory::getCount()
{
  size_type result = 1;
  for(;result < 0x10 && (_read(2*(result))!=0 || _read(2*(result)+1)!=0);++result);
  return result-1;
}

MemorySegment* Memory::getSegement(int index)
{
  size_type startAddress = 0x20;
  for (int i = 1; i != index+1; ++i)
  {
    startAddress += _read (2*i) | _read (2*i+1) << 8;
  }
  actual.reset (index, startAddress, _read (2*(index+1)) | _read (2*(index+1)+1) << 8, false);
  return &actual;
}

MemorySegment* Memory::startSegment()
{
  size_type startAddress = 0x20;
  int size = 1;
  int index;
  for (index = 1; size!=0; ++index)
  {
    size = _read (2*index) | _read (2*index+1) << 8;
    startAddress += size;
  }
  actual.reset (index-2, startAddress, 0, true);
  return &actual;
}

Memory::size_type Memory::diskSpace()
{
  size_type result = 0;
  for (int i = 1; i != 0x10; ++i)
  {
    uint16_t data = _read (2*i) | _read (2*i+1) << 8;
    result += data;
    if (data == 0)
      break;
  }
  return result;
}


void MemorySegment::reset(uint8_t id, int startAddress, size_type size, bool resize)
{
  this->id = id;
  _size = size;
  this->startAddress = startAddress;
  resizeable = resize;
}

uint8_t MemorySegment::read(int index)
{
  return _read (index+startAddress);
}

void MemorySegment::write(int index, uint8_t data)
{
  _write (index+startAddress, data);
}

void MemorySegment::resize(size_type newSize)
{
  if (resizeable)
  {
    _size = newSize;
    _write (2*(id+1), newSize & 0xFF);
    _write (2*(id+1)+1, newSize >> 8);
  }
}

void Memory::sync()
{
#ifdef SD
  sd_raw_sync();
#endif
}
