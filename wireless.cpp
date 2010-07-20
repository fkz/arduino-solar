#include "wireless.h"

MyXBee::MyXBee(const XBeeAddress64 &address)
: lastConnection(0), waitAck(false), connectionError(false), receiver(address)
{
  //FIXME: Baud rate
  xbee.begin (9600);
}

void MyXBee::readPackages()
{
  while (true)
  {
    xbee.readPacket();
    XBeeResponse &response = xbee.getResponse();
    if (response.isError ())
    {
      error (response.getErrorCode());
      break;
    }
    if (!response.isAvailable())
      break;
    
    switch (response.getApiId())
    {
      case ZB_TX_STATUS_RESPONSE:
      {
	ZBTxStatusResponse txStatus;
	response.getZBTxStatusResponse(txStatus);
	if (txStatus.isSuccess())
	{
	  waitAck = false;
	  lastConnection = millis();
	  connectionError = false;
	}
	else
	{
	  connectionError = true;
	}
	break;
      }
      case ZB_RX_RESPONSE:
      {
	ZBRxResponse rx;
	response.getZBRxResponse(rx);
	uint8_t length = rx.getDataLength();
	uint8_t* data = rx.getData();
	readData(data, length);
	break;
      }
    }
  }
}

void MyXBee::writeData(uint8_t* data, uint8_t length)
{
  ZBTxRequest request (receiver, data, length);
  waitAck = true;
  xbee.send(request);
}

