#include "LCDIC2.h"

LCDIC2 lcd(0x27, 16, 2);

void setup() {
  if (lcd.begin()) lcd.print("Hello, World!");
}

void loop() {
  for (uint8_t i = 0; i < 15; i++) {
    lcd.setCursor(i, 1);
    delay(250);
  }
  for (uint8_t i = 15; i > 0; i--) {
    lcd.setCursor(i, 1);
    delay(250);
  }
}
