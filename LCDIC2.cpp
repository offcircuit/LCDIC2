#include "LCDIC2.h"

LCDIC2::LCDIC2(uint8_t address, uint8_t width, uint8_t height, bool font): _address(address), _font(font), _height(height), _width(width) {}

bool LCDIC2::begin() {
  wait(20000);
  Wire.begin(_address);
  Wire.beginTransmission(_address);
  return (_height && _width)
         & !Wire.endTransmission(1)
         & send(0b11, 4100)
         & send(0b11, 100)
         & send(0b11, 100)
         & send(0b10, 100)
         & write(LCDIC2_FUNCTION | LCDIC2_BITS_4 | (_height > 1) << 3 | (_font & (_height == 1)) << 2)
         & write(LCDIC2_DISPLAY | _display << 2 | _cursor << 1 | _blink)
         & write(0b1)
         & write(LCDIC2_MODE | _gain << 1 | _shift);
}

void LCDIC2::bounds(uint8_t &x, uint8_t &y) {
  uint8_t length = 0x4F;
  if (_height == 4)
    if (_width == 16) length = 0x10 + (y / 2) * 0x0E;
    else length = _width;
  else if (_height == 2) length = 0x27;
  x = x < uint8_t(length - 1) ? x : uint8_t(length - 1);
  y = y < uint8_t(_height - 1) ? y : uint8_t(_height - 1);
}

bool LCDIC2::busy() {
  do flag(); while (flag() & 0b10000000);
  return true;
}

uint8_t LCDIC2::charAt(uint8_t x, uint8_t y) {
  bounds(x, y);
  uint8_t data = LCDIC2_DDRAM | (y % 2) << 6 | ((y / 2) * _width) | x;
  getCursor(x, y);
  write(data);
  data = request(0b11);
  setCursor(x, y);
  return data;
}

bool LCDIC2::clear() {
  return write(0b1);
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
  return Wire.read();
}

void LCDIC2::getCursor(uint8_t &x, uint8_t &y) {
  x = request(0b10);
  y = x > 0x39;
  x = x - y * 0x40;
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

bool LCDIC2::print(uint8_t glyph) {
  return write(glyph, 0b1);
}

size_t LCDIC2::print(String data) {
  size_t i = 0;
  do write(data[i], 0b1); while (++i < data.length());
  return i;
}

uint8_t LCDIC2::request(uint8_t rs) {
  writeHigh(0b1111111, rs);
  Wire.requestFrom(uint8_t(_address), uint8_t(1));
  uint8_t data = Wire.read() & 0b11110000;
  writeLow(0b1111111, rs);
  Wire.requestFrom(uint8_t(_address), uint8_t(1));
  return data | Wire.read() >> 4;
}

bool LCDIC2::rightToLeft() {
  return write(LCDIC2_MODE | (_gain = LCDIC2_DEC) << 1 | _shift);
}

bool LCDIC2::send(uint8_t data, uint16_t us) {
  Wire.beginTransmission(_address);
  Wire.write(data << 4 | _backlight << 3);
  Wire.write(data << 4 | _backlight << 3 | 0b100);
  wait(us);
  return !Wire.endTransmission(0);
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
  return write(LCDIC2_DDRAM | (y % 2) << 6 | ((y / 2) * _width) | x);
}

bool LCDIC2::setDisplay(bool state) {
  return write(LCDIC2_DISPLAY | (_display = state) << 2 | _cursor << 1 | _blink);
}

bool LCDIC2::setFont(bool font) {
  return write(LCDIC2_FUNCTION | LCDIC2_BITS_4 | (_height > 1) << 3 | ((_font = font) & (_height == 1)) << 2) & write(0b1);
}

bool LCDIC2::setGlyph(uint8_t glyph, uint8_t data[]) {
  uint8_t x, y;
  getCursor(x, y);
  if (!write(LCDIC2_CGRAM | glyph << 3)) return false;
  for (uint8_t i = 0; i < 8 + 2 * _font; i++) if (!write(data[i], 0b1)) return false;
  return setCursor(x, y);
}

bool LCDIC2::setLines(uint8_t height) {
  return write(LCDIC2_FUNCTION | LCDIC2_BITS_4 | ((_height = height) > 1) << 3 | (_font & (height == 1)) << 2) & write(0b1);
}

bool LCDIC2::setShift(bool state) {
  return write(LCDIC2_MODE | _gain << 1 | (_shift = state));
}

bool LCDIC2::sift(uint8_t glyph, uint8_t *&data) {
  uint8_t x, y;
  getCursor(x, y);
  data = (uint8_t *) malloc(11);
  for (uint8_t i = 0; i < 11; i++) {
    write(LCDIC2_CGRAM | glyph | i);
    data[i] = request(0b11);
  }
  return setCursor(x, y);
}

void LCDIC2::wait(uint16_t us) {
  for (uint32_t i = 0; i < ((us * (F_CPU / 1000L)) / 1000L); i++);
}

bool LCDIC2::write(uint8_t data, uint8_t rs) {
  return writeHigh(data, rs) & busy() & writeLow(data, rs) & busy();
}

bool LCDIC2::writeHigh(uint8_t data, uint8_t rs) {
  Wire.beginTransmission(_address);
  Wire.write(data & 0b11110000 | _backlight << 3 | rs);
  Wire.write(data & 0b11110000 | _backlight << 3 | rs | 0b100);
  return !Wire.endTransmission(0);
}

bool LCDIC2::writeLow(uint8_t data, uint8_t rs) {
  Wire.beginTransmission(_address);
  Wire.write(data << 4 | _backlight << 3 | rs);
  Wire.write(data << 4 | _backlight << 3 | rs | 0b100);
  return !Wire.endTransmission(0);
}
