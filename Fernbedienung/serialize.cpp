#include "serialize.h"
#include <WConstants.h>
#include <HardwareSerial.h>

void FileManagement::startRecord()
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
}

void FileManagement::newData(int spannung, int strom)
{
  actual->write (pointer++, spannung & 0xFF);
  actual->write (pointer++, ((spannung >> 8) & 0x0F) | ((strom >> 4) & 0xF0));
  actual->write (pointer++, strom & 0xFF);
}

void FileManagement::endRecord(uint8_t size, uint8_t* filename)
{
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
    
    for (int index = 5; index < filenamelength-segsize; index+=3)
    {
      uint8_t i1 = segment->read (index+1);
      int spannung = segment->read(index) | ((i1 & 0x0F) << 8);
      int strom = segment->read (index) | ((i1 & 0xF0) << 4);
      
      unsigned long sp = (unsigned long)spannung * 15271 / 1000;
      unsigned long st = (unsigned long)strom * 15271 / 1000;
      Serial.print (sp);
      Serial.write (',');
      Serial.println (st);
      Serial.println ();
    }
  }
}
