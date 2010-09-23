#pragma once
#include <stddef.h>
#include <stdint.h>

class MemorySegment
{
public:
  int size () { return _size; }
  uint8_t read (int index);
  void write (int index, uint8_t data);  
  
  void reset(uint8_t id, int startAddress, uint8_t size, bool resize);
  void resize (int newSize);
  
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
  Memory ();
  
  int getCount ();
  MemorySegment *getSegement (int index);
  MemorySegment *startSegment ();
  
  int diskSpace ();
  
private:
  MemorySegment actual;
};