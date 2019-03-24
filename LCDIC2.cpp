#include "LCDIC2.h"

LCDIC2::LCDIC2(uint8_t address, uint8_t width, uint8_t height) {
  _address = address;
  _height = height;
  _width = width;
}

void LCDIC2::begin() {
  Wire.begin();
  pulseIn(A4, LOW, 500000);

  reset();

  Wire.beginTransmission(_address);
  Wire.write(0b11);
  Wire.write(0b11);
  Wire.write(0b11);
  Wire.write(0b10);
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
  busy();
  Wire.endTransmission();
}

void LCDIC2::blink(bool state) {
  transmit(LCDIC2_DISPLAY | _display << 2 | _cursor << 1 | (_blink = state));
}

uint8_t LCDIC2::busy() {
  uint8_t data = 0;
  do Wire.requestFrom(uint8_t(_address), uint8_t(1));
  while ((data = Wire.read()) > 127);
  pulseIn(A4, LOW, 500);
  return data;
}

void LCDIC2::clear() {
  transmit(0b1);
  pulseIn(A4, LOW, 2500);
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

void LCDIC2::glyph(uint8_t id, uint8_t map[]) {
  transmit(0b1000000 | id << 3);
  for (int i = 0; i < 8; i++) transmit(map[i], 1);
  transmit(0b10000000);
  display(true);
}

void LCDIC2::home() {
  transmit(0b10);
  pulseIn(A4, LOW, 2500);
}

void LCDIC2::leftToRight() {
  transmit(LCDIC2_MODE | LCDIC2_INC | (_shift = false));
}

void LCDIC2::rightToLeft() {
  transmit(LCDIC2_MODE | LCDIC2_DEC | (_shift = false));
}

void LCDIC2::moveLeft() {
  transmit(LCDIC2_MOVE | LCDIC2_SHIFT | LCDIC2_LEFT);
}

void LCDIC2::moveRight() {
  transmit(LCDIC2_MOVE | LCDIC2_SHIFT | LCDIC2_RIGHT);
}

size_t LCDIC2::print(int32_t number) {
  return print(String(number));
}

size_t LCDIC2::print(String string) {
  size_t i;
  for (i = 0; i < string.length(); i++) transmit(string[i], true);
  return i;
}

void LCDIC2::reset() {
  Wire.beginTransmission(_address);
  Wire.write(0b1);
  Wire.write(LCDIC2_FUNCTION | LCDIC2_BITS_8 | LCDIC2_LINES_1 | LCDIC2_DOTS_8);
  Wire.write(LCDIC2_DISPLAY);
  Wire.write(LCDIC2_MODE | LCDIC2_INC);
  busy();
  Wire.endTransmission();
}

uint8_t LCDIC2::transmit(uint8_t data, uint8_t mode = 0) {
  Wire.beginTransmission(_address);
  Wire.write(data & 0b11110000 | 0b100 | mode);
  Wire.write(0b1000);
  busy();
  Wire.write((data & 0b00001111) << 4 | 0b100 | mode);
  Wire.write(0b1000);
  data = busy();
  Wire.endTransmission();
  return data;
}

void LCDIC2::write(uint8_t character) {
  transmit(character, true);
}
