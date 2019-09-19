#include "LCDIC2.h"

LCDIC2 lcd(0x27, 16, 2);


void setup() {
  if (lcd.begin()) lcd.print("Hello, World!");
}

void loop() {
}
