#pragma once
#include "MemoryManagement.h"

// Data structure:
// 
// 0x00  4  Time stamp (milliseconds since last reset)
// 0x04  1  Length of filename (or zero, if none)
// 0x05  ?  Data (3 Bytes per Spannung-Strom-Paar)
//size-length length filename (at the end of the file, the filename)

class FileManagement
{
public:
  FileManagement () : actual (0) {}
  
  void startRecord ();
  void endRecord (uint8_t size = 0, uint8_t *filename = 0);
  
  void newData (int spannung, int strom);
  
  void readAllCvs ();
  
  int diskSpace ()
  {
    return memory.diskSpace();
  }
    
private:
  MemorySegment* actual;
  Memory memory;
  int pointer;
};