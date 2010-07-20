#pragma once

class Dispatcheable
{
  protected:
    virtual void __makeVirtual () {};
};

template< int count >
class Dispatcher
{
  public:
    Dispatcher ();
    
    void call ();
    void setMethod (int id, Dispatcheable *object, void (Dispatcheable::*) (), unsigned long interval);
    
    template< class T >
    void addMethod (Dispatcheable *object, void (T::*method) (), unsigned long interval)
    {
      setMethod (actualId++, object, static_cast< void (Dispatcheable::*) () > (method), interval);
    }
    
  private:
    void (Dispatcheable::*exec[count])();
    unsigned long int lastCall[count];
    unsigned long int interval[count];
    Dispatcheable *objects[count];
    int actualId;
};

typedef Dispatcher<8> NDispatcher;

#include "Dispatcher.cpp"