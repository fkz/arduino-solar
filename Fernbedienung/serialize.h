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