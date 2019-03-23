#include "LCDIC2.h"

LCDIC2 lcd(0x27, 16, 2);

void setup() {
  Serial.begin(9600);
  lcd.begin();

  lcd.print(12);
  lcd.print("H ello z0!{|}");
  delay(1500);
  lcd.clear();
  lcd.cursor(2, 1);
  lcd.print("Hello, World!");

}

void loop() {
}
