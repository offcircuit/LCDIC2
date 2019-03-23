#include "LCDIC2.h"

LCDIC2 lcd(0x27, 16, 2);

void setup() {
  Serial.begin(9600);
  lcd.begin();
  //for (int i = 1; i < 256; i++) {
    delay(150);
    lcd.clear();
    lcd.print("H, W! ç");
    delay(2500);
    lcd.cursor(false);
    delay(2500);
    lcd.clear();
    lcd.print("H, Wç");
  //}
  //lcd.display(true);
}

void loop() {
}
