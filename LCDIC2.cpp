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

void LCDIC2::backlight(bool state) {
  Wire.beginTransmission(_address);
  Wire.write(state << 3);
  Wire.endTransmission();
  flag();
}

void LCDIC2::blink(bool state) {
  write(LCDIC2_DISPLAY | _display << 2 | _cursor << 1 | (_blink = state));
}

void LCDIC2::clear() {
  write(0b1);
  delayMicroseconds(3000);
}

void LCDIC2::cursor(bool state) {
  write(LCDIC2_DISPLAY | _display << 2 | (_cursor = state) << 1 | _blink);
}

uint8_t LCDIC2::cursor(uint8_t y, uint8_t x) {
  return write(LCDIC2_DDRAM | (y < _height - 1 ? y : _height - 1) << 6 | (x < _width - 1 ? x : _width - 1));
}

void LCDIC2::cursorLeft() {
  write(LCDIC2_MOVE | LCDIC2_CURSOR | LCDIC2_LEFT);
}

void LCDIC2::cursorRight() {
  write(LCDIC2_MOVE | LCDIC2_CURSOR | LCDIC2_RIGHT);
}

void LCDIC2::display(bool state) {
  write(LCDIC2_DISPLAY | (_display = state) << 2 | _cursor << 1 | _blink);
  flag();
}

bool LCDIC2::flag() {
  uint8_t data = 0;
  Wire.beginTransmission(_address);
  Wire.write(0b110);
  Wire.endTransmission(1);
  do Wire.requestFrom(uint8_t(_address), uint8_t(2));
  while (!Wire.available());
  data = Wire.read();
  Wire.read();
  while (data > 127);
  return true;
}

void LCDIC2::glyph(uint8_t character) {
  write(character, true);
}

void LCDIC2::glyph(uint8_t id, uint8_t map[], uint8_t height) {
  write(LCDIC2_CGRAM | id << 3);
  for (uint8_t i = 0; i < 8; i++) write(map[i], true);
  write(LCDIC2_DDRAM);
}

void LCDIC2::home() {
  write(0b10);
  delayMicroseconds(2000);
}

void LCDIC2::leftToRight() {
  write(LCDIC2_MODE | (_gain = LCDIC2_INC) << 1 | _shift);
}

void LCDIC2::moveLeft() {
  write(LCDIC2_MOVE | LCDIC2_SHIFT | LCDIC2_LEFT);
}

void LCDIC2::moveRight() {
  write(LCDIC2_MOVE | LCDIC2_SHIFT | LCDIC2_RIGHT);
}

size_t LCDIC2::print(String string) {
  size_t i;
  for (i = 0; i < string.length(); i++) write(string[i], true);
  return i;
}

void LCDIC2::shift(bool state) {
  write(LCDIC2_MODE | _gain << 1 | (_shift = state));
}

bool LCDIC2::reset() {
  Wire.beginTransmission(_address);
  Wire.write(0b11);
  delayMicroseconds(4100);
  Wire.write(0b11);
  delayMicroseconds(100);
  Wire.write(0b11);
  delayMicroseconds(100);
  Wire.write(0b10);
  Wire.endTransmission(1);
  delayMicroseconds(100);
  send(0b0, LCDIC2_FUNCTION | LCDIC2_BITS_4 | LCDIC2_LINES_2 | LCDIC2_DOTS_8);
  send(0b10, 0b1000);
  send(0b0, 0b1);
  send(0b0, _display << 2 | _cursor << 1 | _blink);
  return true;
}

void LCDIC2::rightToLeft() {
  write(LCDIC2_MODE | (_gain = LCDIC2_DEC) << 1 | _shift);
}

bool LCDIC2::write(uint8_t data, bool mode) {
  Wire.beginTransmission(_address);
  Wire.write(data & 0b11110000 | 0b100 | mode);
  Wire.write(0b1000);
  Wire.write(data << 4 | 0b100 | mode);
  Wire.write(0b1000);
  Wire.endTransmission(1);
  return flag();
}
