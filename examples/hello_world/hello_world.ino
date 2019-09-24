#include "LCDIC2.h"

LCDIC2 lcd(0x27, 16, 2);


void setup() {
  if (lcd.begin()) lcd.print("Hello, World!");
}

void loop() {
  for (uint8_t i = 0; i < 16; i++){
    lcd.setCursor(i, 0);
    delay(150);
  }
}
