#include "LCDIC2.h"

LCDIC2::LCDIC2(uint8_t address, uint8_t width, uint8_t height) {
  _address = address;
  _height = height;
  _width = width;
}

void LCDIC2::begin() {
  Wire.begin();
  delay(50);

  reset();

  Wire.beginTransmission(_address);
  Wire.write(0b11);
  Wire.write(0b11);
  Wire.write(0b11);
  Wire.endTransmission();

  transmit(LCDIC2_FUNCTION | LCDIC2_BITS_4 | LCDIC2_LINES_2 | LCDIC2_DOTS_8);
  transmit(LCDIC2_DISPLAY | _display << 2 | _cursor << 1 | _blink);

  clear();
  leftToRight();
  display(true);
}

void LCDIC2::backlight(bool state) {
  Wire.beginTransmission(_address);
  Wire.write(state << 3);
  Wire.endTransmission();
}

void LCDIC2::blink(bool state) {
  transmit(LCDIC2_DISPLAY | _display << 2 | _cursor << 1 | (_blink = state));
}

uint8_t LCDIC2::busy() {
  do Wire.requestFrom(uint8_t(_address), uint8_t(1));
  while (Wire.read() > 127);
}

void LCDIC2::clear() {
  transmit(0b1);
}

void LCDIC2::cursor(bool state) {
  transmit(LCDIC2_DISPLAY | _display << 2 | (_cursor = state) << 1 | _blink);
}

void LCDIC2::cursor(uint8_t x, uint8_t y) {
  transmit(LCDIC2_DDRAM | min(y, _height - 1) << 6 | min(x, _width - 1));
}

void LCDIC2::display(bool state) {
  transmit(LCDIC2_DISPLAY | (_display = state) << 2 | _cursor << 1 | _blink);
}

void LCDIC2::home() {
  transmit(0b10);
}

void LCDIC2::moveLeft() {
  transmit(LCDIC2_MOVE | LCDIC2_SHIFT | LCDIC2_LEFT);
}

void LCDIC2::moveRight() {
  transmit(LCDIC2_MOVE | LCDIC2_SHIFT | LCDIC2_RIGHT);
}

void LCDIC2::leftToRight() {
  transmit(LCDIC2_MODE | (_gain = LCDIC2_INC >> 1) << 1 | (_shift = false));
}

void LCDIC2::rightToLeft() {
  transmit(LCDIC2_MODE | (_gain = LCDIC2_DEC >> 1) << 1 | (_shift = false));
}

void LCDIC2::reset() {
  Wire.beginTransmission(_address);
  Wire.write(0b1);
  Wire.write(LCDIC2_FUNCTION | LCDIC2_BITS_8 | LCDIC2_LINES_1 | LCDIC2_DOTS_8);
  Wire.write(LCDIC2_DISPLAY);
  Wire.write(LCDIC2_MODE | LCDIC2_INC);
  Wire.endTransmission();
  busy();
}


uint8_t LCDIC2::transmit(uint8_t data, uint8_t mode = 0) {
  Wire.beginTransmission(_address);
  do {
    Wire.write((data & 0b11110000 | 0b1100 | mode));
    Wire.write(0b11111011);
  } while (data = (data << 4));
  Wire.endTransmission();
  delayMicroseconds(LCDIC2_SHORT_DELAY);
}
