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

#ifdef OLD_CASE

/**
 @short Hilfsklasse, von der alle Klassen, die Timer-Methoden anbieten, abgeleitet werden müssen
*/
class Dispatcheable
{
  protected:
    virtual void __makeVirtual () {};
};

#endif

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
#ifdef OLD_CASE
    void setMethod (int id, Dispatcheable *object, void (Dispatcheable::*) (), unsigned long interval);
#else
    void setMethod (int id, void (*) (), unsigned long interval);
#endif
    
    /**
     fügt eine Methode hinzu. Es dürfen maximal @p count Methoden hinzugefügt werden, weitere werden nicht erkannt, bzw.
     führen zu undefiniertem Verhalten
     @param object Objekt, von dem aus aufgerufen wird
     @param method aufzurufende Methode des Objekts
     @param interval Intervall in ms, in dem die Methode aufgerufen werden soll
     @note die Methode wird beim nächsten @ref call Aufruf ausfeführt
    */
#ifdef OLD_CASE
    template< class T >
    int addMethod (Dispatcheable *object, void (T::*method) (), unsigned long interval)
#else
    int addMethod (void (*method) (), unsigned long interval)
#endif
    {
#ifdef OLD_CASE
      setMethod (actualId, object, static_cast< void (Dispatcheable::*) () > (method), interval);
#else
      setMethod (actualId, method, interval);
#endif
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
#ifdef OLD_CASE
    void (Dispatcheable::*exec[count])();
#else
    void (*exec[count])();
#endif
    unsigned long int lastCall[count];
    unsigned long int interval[count];
#ifdef OLD_CASE
    Dispatcheable *objects[count];
#endif
    int actualId;
};

/// Standardinstanz von Dispatcher, der bis zu 8 Methoden füllen kann
typedef Dispatcher<8> NDispatcher;

#include "Dispatcher.cpp"