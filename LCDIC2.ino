#include "LCDIC2.h"

LCDIC2 lcd(0x27, 16, 2);

uint8_t heart[8] = {
  0b10000,
  0b01000,
  0b00100,
  0b00010,
  0b00001,
  0b00010,
  0b00100,
  0b01000
};

void setup() {
  Serial.begin(9600);
  uint16_t i, data;

  uint8_t address ;
  lcd.begin();
  lcd.print("123456"); delay(1000);
  lcd.glyph(6, heart);
  lcd.print("H");
  lcd.write(6);
  lcd.print("H");
}

void loop() {
}


/*data = 0b1000000;
  //for (i = 0; i < 256; i++)
  //{

  Serial.println(lcd.transmit(0b100000));
  lcd.display(true);
  lcd.print("3210");delay(100);
  // if ((address == 3) || (address == 4) || (address == 5)) {
   Serial.println("");
   Serial.print(i);
   Serial.print("  ==  ");
   Serial.print(address);
   Serial.print("  ==  ");
   Serial.print(address, BIN);
   Serial.print("  ==  ");
   Serial.println(address, HEX);
  // } else {
  //   Serial.print(String(address) + ",");
  // }
  delay(100);
  }

  lcd.createChar(0, heart);*/
