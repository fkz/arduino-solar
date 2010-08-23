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
#include "Dispatcher.h"

class MyXBee: public Dispatcheable
{
  public:
    enum SpecialCharacters
    {
      #ifdef REALLY_WORLD
      START_BYTE = 200,
      ESCAPE = 201
      #else
      START_BYTE = 's',
      ESCAPE = 'e'
      #endif
    };
    
    enum ErrorMessages
    {
      NO_START_BYTE = 100,
      START_BYTE_INSIDE_MESSAGE = 101
    };
    
    const static int MAX_TIME_BETWEEN_TWO_REQUESTS = 2000; // 2 seconds
    
    MyXBee ();
    
    void readPackages();
    bool isConnected ();
    
    void writeData (uint8_t *data, uint8_t length);
    void writeEscaped(uint8_t arg1);
    
    long getReadCount () { return read_count; }
    
  protected:
    virtual void error(uint8_t arg1) = 0;
    virtual void readData (uint8_t *data, uint8_t length) = 0;
    virtual void connectionInterrupted () = 0;
    
  private:
    
    uint8_t package[100];
    long unsigned int alredyRead;
    long unsigned int lastPackageRead;
    
    bool _isConnected;
    long read_count;
};