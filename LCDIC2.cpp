#include "LCDIC2.h"

LCDIC2::LCDIC2(uint8_t address, uint8_t width, uint8_t height, bool font): _address(address), _width(width), _height(height), _font(font) {}

bool LCDIC2::begin() {
  wait(20000);
  Wire.begin(_address);
  Wire.beginTransmission(_address);
  Wire.endTransmission(1);

  return (_height && _width)
         & writeLow(0b11)
         & wait(4100)
         & writeLow(0b11)
         & wait(100)
         & writeLow(0b11)
         & wait(100)
         & writeLow(0b10)
         & wait(100)
         & write(LCDIC2_FUNCTION | LCDIC2_BITS_4 | (_height > 1) << 3 | (_font & (_height == 1)) << 2)
         & write(LCDIC2_DISPLAY | _display << 2 | _cursor << 1 | _blink)
         & write(0b1)
         & write(LCDIC2_MODE | _gain << 1 | _shift)
         ;
}

void LCDIC2::bounds(uint8_t &x, uint8_t &y) {
  y = y > (_height - 1) ? (_height - 1) : y;
  x = x > (length(y) - 1) ? (length(y) - 1) : x;
}

bool LCDIC2::busy() {
  while (flag() > 0b1000);
  return true;
}

bool LCDIC2::clear() {
  return write(0b1) & wait(4400);
}

char LCDIC2::charAt(uint8_t x, uint8_t y) {
  bounds(x, y);
  uint8_t data = LCDIC2_DDRAM + start(y) + x;
  getCursor(x, y);
  write(data);
  data = request(0b11);
  setCursor(x, y);
  return data;
}

bool LCDIC2::cursorLeft() {
  return write(LCDIC2_MOVE | LCDIC2_CURSOR | LCDIC2_LEFT);
}

bool LCDIC2::cursorRight() {
  return write(LCDIC2_MOVE | LCDIC2_CURSOR | LCDIC2_RIGHT);
}

uint8_t LCDIC2::flag() {
  Wire.beginTransmission(_address);
  Wire.write(_backlight << 3 | 0b10);
  Wire.endTransmission(0);
  Wire.requestFrom(uint8_t(_address), uint8_t(1), uint8_t(0));
  //while (Wire.available() < 1);
  return Wire.read();
}

void LCDIC2::getCursor(uint8_t &x, uint8_t &y) {
  x = request(0b10);
  y = 0;
  if (_height == 2) y = x >= start(1);
  else if (_height == 4) {
    if (x >= start(3)) y = 3;
    else if (x >= start(1)) y = 1;
    else if (x >= start(2)) y = 2;
  }
  x -= start(y);
}

bool LCDIC2::getGlyph(uint8_t glyph, uint8_t *&data) {
  uint8_t x, y;
  data = (uint8_t *) malloc(11);
  getCursor(x, y);
  for (uint8_t i = 0; i < 1 << (3 + _font) ; i++) {
    if (!write(LCDIC2_CGRAM | (glyph << (3 + _font)) | i)) return false;
    data[i] = request(0b11);
  }
  return setCursor(x, y);
}

bool LCDIC2::home() {
  return write(0b10);
}

bool LCDIC2::leftToRight() {
  return write(LCDIC2_MODE | (_gain = LCDIC2_INC) << 1 | _shift);
}

uint8_t LCDIC2::length(uint8_t y) {
  return (0x50 / _height) - (((_height == 4) && (_width == 16)) * (4 - ((y / 2) * 8)));
}

bool LCDIC2::moveLeft() {
  return write(LCDIC2_MOVE | LCDIC2_SHIFT | LCDIC2_LEFT);
}

bool LCDIC2::moveRight() {
  return write(LCDIC2_MOVE | LCDIC2_SHIFT | LCDIC2_RIGHT);
}

bool LCDIC2::print(uint8_t glyph) {
  return write(glyph << _font, 0b1);
}

uint8_t LCDIC2::print(String data) {
  uint8_t i = 0;
  while (write(data[i], 0b1) && ++i < data.length());
  return i;
}

uint8_t LCDIC2::request(uint8_t rs) {
  writeHigh(0b11111111, rs);
  Wire.requestFrom(uint8_t(_address), uint8_t(1), uint8_t(0));
  while (Wire.available() < 1);
  uint8_t data = Wire.read() & 0b11110000;
  writeLow(0b11111111, rs);
  Wire.requestFrom(uint8_t(_address), uint8_t(1), uint8_t(0));
  while (Wire.available() < 1);
  return data | Wire.read() >> 4;
}

bool LCDIC2::rightToLeft() {
  return write(LCDIC2_MODE | (_gain = LCDIC2_DEC) << 1 | _shift);
}

bool LCDIC2::setBacklight(bool state) {
  Wire.beginTransmission(_address);
  Wire.write((_backlight = state) << 3);
  return !Wire.endTransmission(0);
}

bool LCDIC2::setBlink(bool state) {
  return write(LCDIC2_DISPLAY | _display << 2 | _cursor << 1 | (_blink = state));
}

bool LCDIC2::setCursor(bool state) {
  return write(LCDIC2_DISPLAY | _display << 2 | (_cursor = state) << 1 | _blink);
}

bool LCDIC2::setCursor(uint8_t x, uint8_t y) {
  bounds(x, y);
  return write(LCDIC2_DDRAM + start(y) + x);
}

bool LCDIC2::setDisplay(bool state) {
  return write(LCDIC2_DISPLAY | (_display = state) << 2 | _cursor << 1 | _blink);
}

bool LCDIC2::setFont(bool font) {
  return write(LCDIC2_FUNCTION | LCDIC2_BITS_4 | (_height > 1) << 3 | ((_font = font) & (_height == 1)) << 2) && write(0b1);
}

bool LCDIC2::setGlyph(uint8_t glyph, uint8_t data[]) {
  uint8_t x, y;
  getCursor(x, y);
  if (!write(LCDIC2_CGRAM | (glyph << (3 + _font)))) return false;
  for (uint8_t i = 0; i < 1 << (3 + _font); i++) if (!write((i > 7 + 2 * _font ? 0 : data[i]), 0b1)) return false;
  return setCursor(x, y);
}

bool LCDIC2::setLines(uint8_t height) {
  return write(LCDIC2_FUNCTION | LCDIC2_BITS_4 | ((_height = height) > 1) << 3 | (_font & (height == 1)) << 2) && write(0b1);
}

bool LCDIC2::setShift(bool state) {
  return write(LCDIC2_MODE | _gain << 1 | (_shift = state));
}

uint8_t LCDIC2::start(uint8_t y) {
  return ((y % 2) << 6) + ((y / 2) * _width);
}

bool LCDIC2::wait(uint16_t us) {
  for (uint32_t i = 0; i < ((us * (F_CPU / 1000L)) / 1000L); i++);
  return true;
}

bool LCDIC2::write(uint8_t data, uint8_t rs) {
  return writeHigh(data, rs) && busy() && writeLow(data, rs) && busy();
}

bool LCDIC2::writeHigh(uint8_t data, uint8_t rs) {
  Wire.beginTransmission(_address);
  Wire.write((data & 0b11110000) | (_backlight << 3) | rs | 0b100);
  return !Wire.endTransmission(0);
}

bool LCDIC2::writeLow(uint8_t data, uint8_t rs) {
  Wire.beginTransmission(_address);
  Wire.write((data << 4) | (_backlight << 3) | rs | 0b100);
  return !Wire.endTransmission(0);
}
