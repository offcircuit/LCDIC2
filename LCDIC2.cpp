#include "LCDIC2.h"

ILCDIC2::ILCDIC2(uint8_t address, uint8_t width, uint8_t height) {
  _address = address;
  _height = height;
  _width = width;
}

void ILCDIC2::begin() {
  Wire.begin();
  delay(50);

  clear();

  Wire.beginTransmission(_address);

  Wire.write(0b11);
  delayMicroseconds(LCDIC2_LONG_DELAY);

  Wire.write(0b11);
  delayMicroseconds(LCDIC2_SHORT_DELAY);

  Wire.write(0b11);
  delayMicroseconds(LCDIC2_SHORT_DELAY);

  //Wire.write(0b10);
  delayMicroseconds(LCDIC2_SHORT_DELAY);

  Wire.endTransmission();

  transmit(LCDIC2_FUNCTION | LCDIC2_DOTS_8 | LCDIC2_LINES_2 | LCDIC2_BITS_4);
  delayMicroseconds(LCDIC2_SHORT_DELAY);

  transmit(LCDIC2_DISPLAY | _display << 2 | _cursor << 1 | _blink);
  delayMicroseconds(LCDIC2_SHORT_DELAY);

  clear();

  leftToRight();
  delayMicroseconds(LCDIC2_SHORT_DELAY);

  transmit(0b10);
  delayMicroseconds(LCDIC2_LONG_DELAY);

  display(true);
  delayMicroseconds(LCDIC2_SHORT_DELAY);
}

void ILCDIC2::clear() {
  transmit(0b1);
  delayMicroseconds(LCDIC2_LONG_DELAY);
}

void ILCDIC2::cursor(uint8_t x, uint8_t y) {
  transmit(LCDIC2_DDRAM | min(y, _height - 1) << 6 | min(x, _width - 1));
}

void ILCDIC2::display(bool state) {
  transmit(LCDIC2_DISPLAY | (_display = state) << 2 | _cursor << 1 | _blink);
}

void ILCDIC2::home() {
  transmit(0b10);
  //else
  //cursor(0, 8);
  delayMicroseconds(LCDIC2_LONG_DELAY);
}

void ILCDIC2::leftToRight() {
  transmit(LCDIC2_MODE | (_gain = LCDIC2_INC >> 1) << 1 | (_shift = false));
  delayMicroseconds(LCDIC2_SHORT_DELAY);
}

void ILCDIC2::rightToLeft() {
  transmit(LCDIC2_MODE | (_gain = LCDIC2_DEC >> 1) << 1 | (_shift = false));
}

void ILCDIC2::transmit(uint8_t data, uint8_t mode = 0) {
  do {
    Wire.beginTransmission(_address);
    Wire.write((data & 0b11110000 | 0b1100 | mode));
    delayMicroseconds(1);
    Wire.write(0b11111011);
    delayMicroseconds(LCDIC2_SHORT_DELAY);
    Wire.endTransmission();
  } while (data = (data << 4));
}
