#include "LCDIC2.h"

LCDIC2 lcd(0x27, 16, 2);

void setup()
{
  Serial.begin(9600);
  lcd.begin(); 
  delay(200);           
  lcd.print("1234");
  delay(90);
  lcd.transmit(LCDIC2_MODE | LCDIC2_INC | false);
  Wire.requestFrom(0x27, 1);
  int i = Wire.read();
  Serial.println(i, BIN);
  //lcd.reset();
}

void loop()
{
}
