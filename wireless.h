#include <XBee/XBee.h>
#include "Dispatcher.h"

class MyXBee: public Dispatcheable
{ 
  public:
    const static int MAX_TIME_BETWEEN_TWO_REQUESTS = 100;
    
    MyXBee (const XBeeAddress64 &receiverAddress);
    /**
     this method has to be called repeatly, often enough,
     so that there doesn't happen a buffer overrun
    */
    void readPackages ();
    bool isConnected ()
    {
      return !connectionError && lastConnection;
    }
    
    void writeData (uint8_t *data, uint8_t length);        
    
    
  protected:
    virtual void error(uint8_t arg1) = 0;
    virtual void readData (uint8_t *data, uint8_t length) = 0;
    virtual void connectionInterrupted () = 0;
    
  private:
    XBeeAddress64 receiver;
    XBee xbee;
    unsigned long lastConnection;
    bool waitAck;
    bool connectionError;
};