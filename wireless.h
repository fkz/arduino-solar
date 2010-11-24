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
#include "MessageTypes.h"

#ifdef OLD_CASE
class MyXBee: public Dispatcheable
#else
class MyXBee
#endif
{
  public:
    static const unsigned int baudRate = 57600;
    
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
    
    template< const char type >
    void writePackage (typename Message::MessageData< type >::Param1 param1)
    {
      static_assert (Message::MessageData< type >::ParamCount == 1, "wrong param count");
      Serial.write (START_BYTE);
      writeEscaped (1+sizeof (typename Message::MessageData< type >::Param1));
      writeEscaped (type);
      for (uint8_t* it = &param1; it != &param1 + 1; ++it)
	writeEscaped (*it);
    }
    
    template< const char type >
    void writePackage (typename Message::MessageData< type >::Param1 param1, typename Message::MessageData< type >::Param2 param2)
    {
      static_assert (Message::MessageData< type >::ParamCount == 2, "wrong param count");
      
      Serial.write (START_BYTE);
      writeEscaped (1+sizeof (typename Message::MessageData< type >::Param1)+sizeof (typename Message::MessageData< type >::Param2));
      writeEscaped (type);
      for (uint8_t* it = reinterpret_cast< uint8_t * > (&param1); it != reinterpret_cast< uint8_t * > (&param1 + 1); ++it)
	writeEscaped (*it);
      for (uint8_t* it = reinterpret_cast< uint8_t * > (&param2); it != reinterpret_cast< uint8_t * > (&param2 + 1); ++it)
	writeEscaped (*it);
    }
    
    template< const char type >
    void writePackage (typename Message::MessageData< type >::Param1 param1, typename Message::MessageData< type >::Param2 param2, typename Message::MessageData< type >::Param3 param3)
    {
      static_assert (Message::MessageData< type >::ParamCount == 3, "wrong param count");
      Serial.write (START_BYTE);
      writeEscaped (1+sizeof (typename Message::MessageData< type >::Param1) + sizeof (typename Message::MessageData< type >::Param2) + sizeof (typename Message::MessageData< type >::Param3));
      writeEscaped (type);
      for (uint8_t* it = reinterpret_cast< uint8_t * > (&param1); it != reinterpret_cast< uint8_t * > (&param1 + 1); ++it)
	writeEscaped (*it);
      for (uint8_t* it = reinterpret_cast< uint8_t * > (&param2); it != reinterpret_cast< uint8_t * > (&param2 + 1); ++it)
	writeEscaped (*it);
      for (uint8_t* it = reinterpret_cast< uint8_t * > (&param3); it != reinterpret_cast< uint8_t * > (&param3 + 1); ++it)
	writeEscaped (*it);
    }
    
    template< const char type >
    void writePackage (typename Message::MessageData< type >::Param1 param1, typename Message::MessageData< type >::Param2 param2, typename Message::MessageData< type >::Param3 param3, typename Message::MessageData< type >::Param4 param4)
    {
      static_assert (Message::MessageData< type >::ParamCount == 4, "wrong param count");
      Serial.write (START_BYTE);
      writeEscaped (1+sizeof (typename Message::MessageData< type >::Param1) + sizeof (typename Message::MessageData< type >::Param2) + sizeof (typename Message::MessageData< type >::Param3) + sizeof (typename Message::MessageData< type >::Param4));
      writeEscaped (type);
      for (uint8_t* it = reinterpret_cast< uint8_t * > (&param1); it != reinterpret_cast< uint8_t * > (&param1 + 1); ++it)
	writeEscaped (*it);
      for (uint8_t* it = reinterpret_cast< uint8_t * > (&param2); it != reinterpret_cast< uint8_t * > (&param2 + 1); ++it)
	writeEscaped (*it);
      for (uint8_t* it = reinterpret_cast< uint8_t * > (&param3); it != reinterpret_cast< uint8_t * > (&param3 + 1); ++it)
	writeEscaped (*it);
      for (uint8_t* it = reinterpret_cast< uint8_t * > (&param4); it != reinterpret_cast< uint8_t * > (&param4 + 1); ++it)
	writeEscaped (*it);
    }
    
    long getReadCount () { return read_count; }
    
#ifdef OLD_CASE
    typedef void (*ReadPackage) (const uint8_t *data, uint8_t length);
#else
    typedef void (*ReadPackage) (const void *data, uint8_t length);
#endif
    
    void _registerMethod (uint8_t type, ReadPackage package);
    
    template< const char type >
    void registerMethod (void (*p) (const Message::MessageData< type > *data, uint8_t length))
    {
      _registerMethod (type, reinterpret_cast< ReadPackage > (p));
    }
    
  private:
#ifndef OLD_CASE
    void readData (const uint8_t *data, uint8_t length);
#else
protected:
    virtual void readData (const uint8_t *data, uint8_t length) = 0;    
#endif
private:
    void writeEscaped(uint8_t arg1);
    
    void error(uint8_t arg1)
    {
      uint8_t data[2];
      data[0] = ERROR;
      data[1] = arg1;
      return readData (data, 2);
    }
    
    void connectionInterrupted ()
    {
      static const uint8_t connectionInterrupted[2] = { CONNECTION_INTERRUPTED, false };
      readData (connectionInterrupted, 2);
    }
    
    void connectionRestored()
    {
      static const uint8_t connectionRestored[2] = { CONNECTION_INTERRUPTED, true };
      readData (connectionRestored, 2);
    }
    
  private:
    
    uint8_t package[100];
    long unsigned int alredyRead;
    long unsigned int lastPackageRead;
    
    bool _isConnected;
    long read_count;
    
    struct RegisterPair
    {
      RegisterPair () : type (0), delegate (0) { }
      uint8_t type;
      ReadPackage delegate;
    };
    
    RegisterPair registrants[16];
    int registrantsCount;
};

namespace Message{
  MESSAGE_DATA_1 (MyXBee::CONNECTION_INTERRUPTED, bool, connected);
  MESSAGE_DATA_1 (MyXBee::ERROR, uint8_t, errorNumber);
};