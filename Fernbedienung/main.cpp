#include <WProgram.h>
#include <XBee/XBee.h>
#include <LiquidCrystal/LiquidCrystal.h>

int main ()
{
  const int pot1 = 0;
  const int pot2 = 1;
  
  LiquidCrystal lcd (12, 11, 5, 4, 3, 2);
  XBee xbee;
  xbee.begin();
  
  
  lcd.clear();
  //TODO: evtl. String in Flash auslagern
  lcd.print("Verbinde mit Solarboot");
  long unsigned int lastConnection = 0;
  
  while (true)
  {
    xbee.readPacket();
    XBeeResponse &response = xbee.getResponse();
    if (response.isError())
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Fehler in Xbee");
      lcd.setCursor(0,1);
      lcd.print(" Nr. ");
      lcd.print (response.getErrorCode() , DEC);
    }
    if (response.isAvailable())
    {
      switch (response.getApiId())
      {
	case ZB_RX_RESPONSE:
	{
	  lastConnection = millis();
	  ZBRxResponse zbResponse;
	  response.getZBRxResponse(zbResponse);
	  assert (zbResponse.getDataLength() == 4);
	  uint8_t* data = zbResponse.getData();
	  uint16_t strom = data[0] + data[1] << 8;
	  uint16_t spannung = data[2] + data[3] << 8;
	  uint32_t leistung = strom * spannung;
	  
	  lcd.clear();
	  lcd.setCursor(0, 0);
	  lcd.print ("Spannung: ");
	  lcd.print (spannung, DEC);
	  
	  lcd.setCursor(0, 1);
	  lcd.setCursor("Strom: ");
	  lcd.print (strom, DEC);
	  break;
	}
      }
    }
    
    uint8_t requestArray[3];
    requestArray[0] = 'P';
    requestArray[1] = analogRead (pot1) << 2;
    requestArray[2] = analogRead (pot2) << 2;
    ZBTxRequest request ();
  }
  
}