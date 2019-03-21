#include "LCDIC2.h"

LCDIC2 lcd(0x27, 16, 2);

void setup()
{
  Serial.begin(9600);
  lcd.begin();                      // initialize the lcd

  // Print a message to the LCD.
  //
  lcd.home();delay(10);
  lcd.print("FIRST");
  //lcd.transmit(LCDIC2_MODE | LCDIC2_DEC | 0);

 // delay(2500);
  //lcd.clear();

  //lcd.cursor(8,0);
  //lcd.print("12345");
  //delay(2500);
  //lcd.scroll();
  //lcd.transmit(0x10 | 0x08);

  //lcd.cursor(12, 0);
  //delay(1500);
  //lcd.scroll();
  //lcd.print("ABC");
  //lcd.cursor(12, 0);
  /*
    delay(3500);
    lcd.clear();
    lcd.rightToLeft();

    delay(1500);
    lcd.cursor(0, 8);
    lcd.print("THIRD");

    delay(1500);
    lcd.cursor(0, 8);
    lcd.autoscroll();
    lcd.print("THIRD");*/
}

void loop()
{
}
