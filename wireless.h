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
    
    enum SpecialPackageTypes
    {
      ERROR = 254,
      CONNECTION_INTERRUPTED = 255
    };
    
    enum ErrorMessages
    {
      NO_START_BYTE = 1,
      START_BYTE_INSIDE_MESSAGE = 2,
      MESSAGE_NOT_REGISTERED = 3
    };
    
    const static int MAX_TIME_BETWEEN_TWO_REQUESTS = 2000; // 2 seconds
    
    MyXBee ();
    
    void readPackages();
    bool isConnected ();
    
    /// @deprecated use writePackage instead
    void writeData (uint8_t *data, uint8_t length);
    
    void writePackage (uint8_t type, uint8_t *data, uint8_t length);
    
    long getReadCount () { return read_count; }
    
    typedef void (*ReadPackage) (const uint8_t *data, uint8_t length);
    
    void registerMethod (uint8_t type, ReadPackage package);
    
    
  private:
    void readData (const uint8_t *data, uint8_t length);
    void writeEscaped(uint8_t arg1);
    
    void error(uint8_t arg1)
    {
      uint8_t data[2];
      data[0] = CONNECTION_INTERRUPTED;
      data[1] = arg1;
      return readData (data, 2);
    }
    
    void connectionInterrupted ()
    {
      static const uint8_t connectionInterrupted = CONNECTION_INTERRUPTED;
      readData (&connectionInterrupted, 1);
    }
    
  private:
    
    uint8_t package[100];
    long unsigned int alredyRead;
    long unsigned int lastPackageRead;
    
    bool _isConnected;
    long read_count;
    
    struct RegisterPair
    {
      uint8_t type;
      ReadPackage delegate;
    };
    
    RegisterPair registrants[16];
    int registrantsCount;
};