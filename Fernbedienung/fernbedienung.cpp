#include "fernbedienung.h"

Fernbedienung::Fernbedienung()
: MyXBee(XBeeAddress64 (receiver1, receiver2)), lcd (LCD_RS, LCD_ENABLE, LCD_D0, LCD_D1, LCD_D2, LCD_D3)
{
  addMethod(this, &Fernbedienung::readPackages, 0);
}

void Fernbedienung::error(uint8_t arg1)
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print ("Fehler XBee Nr.");
  lcd.setCursor(0,1);
  lcd.print (arg1, DEC);
}

void Fernbedienung::readData(uint8_t* data, uint8_t length)
{
  if (data[0] == 'd')
  {
    // show data
    int spannung = data[1] + data[2] << 8;
    int strom = data[3] + data[4] << 8;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print ("Spannung: ");
    lcd.print (spannung, DEC);
    lcd.setCursor (0, 1);
    lcd.print ("Strom: ");
    lcd.print(strom, DEC);
  }
}

void Fernbedienung::connectionInterrupted()
{
  lcd.clear();
  lcd.setCursor (0,0);
  lcd.print ("Connection with");
  lcd.setCursor (0, 1);
  lcd.print ("Solarboat lost");
}

