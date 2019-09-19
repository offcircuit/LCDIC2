#include "LCDIC2.h"

LCDIC2::LCDIC2(uint8_t address, uint8_t width, uint8_t height) {
  _address = address;
  _height = height;
  _width = width;
}

bool LCDIC2::begin() {
  delay(500);
  Wire.begin(_address);

   if (reset()) {
    delayMicroseconds(2000);
    backlight(true);
    home();
    clear();
    return true;
  }
  return false;
}

bool LCDIC2::backlight(bool state) {
  Wire.beginTransmission(_address);
  Wire.write(state << 3);
  Wire.endTransmission();
  return flag();
}

bool LCDIC2::blink(bool state) {
  return write(LCDIC2_DISPLAY | _display << 2 | _cursor << 1 | (_blink = state));
}

bool LCDIC2::clear() {
  return write(0b1);
}

bool LCDIC2::cursor(bool state) {
  return write(LCDIC2_DISPLAY | _display << 2 | (_cursor = state) << 1 | _blink);
}

bool LCDIC2::cursor(uint8_t y, uint8_t x) {
  return write(LCDIC2_DDRAM | (y < _height - 1 ? y : _height - 1) << 6 | (x < _width - 1 ? x : _width - 1));
}

bool LCDIC2::cursorLeft() {
  return write(LCDIC2_MOVE | LCDIC2_CURSOR | LCDIC2_LEFT);
}

bool LCDIC2::cursorRight() {
  return write(LCDIC2_MOVE | LCDIC2_CURSOR | LCDIC2_RIGHT);
}

bool LCDIC2::display(bool state) {
  return write(LCDIC2_DISPLAY | (_display = state) << 2 | _cursor << 1 | _blink);
}

bool LCDIC2::flag() {
  Wire.beginTransmission(_address);
  Wire.write(0b110);
  Wire.endTransmission();
  do Wire.requestFrom(uint8_t(_address), uint8_t(2));
  while (Wire.available() < 2);
  while ((Wire.read() << 4 | (Wire.read() && 0b00001111)) > 127);
  return true;
}

bool LCDIC2::glyph(uint8_t character) {
  return write(character, true);
}

bool LCDIC2::glyph(uint8_t id, uint8_t map[]) {
  if (!write(LCDIC2_CGRAM | id << 3)) return false;
  for (uint8_t i = 0; i < 8; i++) if (!write(map[i], true)) return false;
  return write(LCDIC2_DDRAM);
}

bool LCDIC2::home() {
  return write(0b10);
}

bool LCDIC2::leftToRight() {
  return write(LCDIC2_MODE | (_gain = LCDIC2_INC) << 1 | _shift);
}

bool LCDIC2::moveLeft() {
  return write(LCDIC2_MOVE | LCDIC2_SHIFT | LCDIC2_LEFT);
}

bool LCDIC2::moveRight() {
  return write(LCDIC2_MOVE | LCDIC2_SHIFT | LCDIC2_RIGHT);
}

size_t LCDIC2::print(String data) {
  size_t i = 0;
  while (write(data[i], true) && (++i < data.length()));
  return i;
}

bool LCDIC2::reset() {
  Wire.beginTransmission(_address);
  Wire.write(uint8_t(0b11));
  delayMicroseconds(4100);
  Wire.write(uint8_t(0b11));
  delayMicroseconds(100);
  Wire.write(uint8_t(0b11));
  delayMicroseconds(100);
  Wire.write(uint8_t(0b10));
  Wire.endTransmission();

  delayMicroseconds(100);
  send(0b0, LCDIC2_FUNCTION | LCDIC2_BITS_4 | LCDIC2_LINES_2 | LCDIC2_DOTS_8);
  send(0b10, 0b1000);
  send(0b0, 0b1);
  send(0b0, _display << 2 | _cursor << 1 | _blink);
  return true;
}

bool LCDIC2::rightToLeft() {
  return write(LCDIC2_MODE | (_gain = LCDIC2_DEC) << 1 | _shift);
}

bool LCDIC2::send(uint8_t registry, uint8_t data) {
  Wire.beginTransmission(_address);
  Wire.write(registry);
  Wire.write(data);
  return (Wire.endTransmission() == 0) && flag();
}

bool LCDIC2::shift(bool state) {
  return write(LCDIC2_MODE | _gain << 1 | (_shift = state));
}

bool LCDIC2::write(uint8_t data, bool mode) {
  Wire.beginTransmission(_address);
  Wire.write(data & 0b11110000 | 0b100 | mode);
  Wire.write(0b1000);
  Wire.write(data << 4 | 0b100 | mode);
  Wire.write(0b1000);
  return (Wire.endTransmission() == 0) && flag();
}
