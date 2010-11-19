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
#pragma once
#include <stddef.h>
#include <stdint.h>

class MemorySegment
{
public:
  typedef unsigned long int size_type;
  int size () { return _size; }
  uint8_t read (int index);
  void write (int index, uint8_t data);  
  
  void reset(uint8_t id, int startAddress, uint8_t size, bool resize);
  void resize (size_type newSize);
  
  int getId ()
  {
    return id;
  }
  
private:
  int startAddress;
  int _size;
  bool resizeable;
  uint8_t id;
};

/**
 * @short the main memory class
 * used for receiving "files" (called segments here) of data
 * @warning a MemorySegment is rewritten, when a method is called 
 * (since static memory is used, because of new/delete aren't available)
 */
class Memory
{
public:
  typedef unsigned long int size_type;
  
  Memory ();
  
  int getCount ();
  MemorySegment *getSegement (int index);
  MemorySegment *startSegment ();
  
  size_type diskSpace ();
  size_type getCapacity ()
  {
    return capacity;
  }
  
  void sync();
  
private:
  MemorySegment actual;
  size_type capacity;
};