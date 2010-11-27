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

/**
 @short Diese Klasse handelt wiederkehrende Aufgaben
 
*/
template< int count >
class Dispatcher
{
  public:
    Dispatcher ();
    
    /**
     diese Methode sollte möglichst häufig aufgerufen werden
    */
    void call ();
    /// setze eine Methode an Stelle @p id. @p id muss zwischen 0 und count liegen. Möglicherweise bereits dort
    /// befindliche Methoden werden überschrieben
    void setMethod (int id, void (*) (), unsigned long interval);
    
    /**
     fügt eine Methode hinzu. Es dürfen maximal @p count Methoden hinzugefügt werden, weitere werden nicht erkannt, bzw.
     führen zu undefiniertem Verhalten
     @param object Objekt, von dem aus aufgerufen wird
     @param method aufzurufende Methode des Objekts
     @param interval Intervall in ms, in dem die Methode aufgerufen werden soll
     @note die Methode wird beim nächsten @ref call Aufruf ausfeführt
    */
    int addMethod (void (*method) (), unsigned long interval)
    {
      setMethod (actualId, method, interval);
      return actualId++;
    }
    
    unsigned long getInterval (int id)
    {
      return interval[id];
    }
    
    void setInterval (int id, unsigned long _interval)
    {
      interval[id] = _interval;
    }
    
  private:
    void (*exec[count])();
    unsigned long int lastCall[count];
    unsigned long int interval[count];
    int actualId;
};

/// Standardinstanz von Dispatcher, der bis zu 8 Methoden füllen kann
typedef Dispatcher<8> NDispatcher;

#include "Dispatcher.cpp"