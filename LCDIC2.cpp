#include "LCDIC2.h" // needed a better datasheet
// try https://panda-bg.com/datasheet/2134-091834-LCD-module-TC1602D-02WA0-16x2-STN.pdf

LCDIC2::LCDIC2(uint8_t address, uint8_t width, uint8_t height) {
  _address = address;
  _height = height;
  _width = width;
}

void LCDIC2::begin() {
  Wire.begin(_address);
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
  Wire.endTransmission();
  busy();
}

void LCDIC2::blink(bool state) {
  transmit(LCDIC2_DISPLAY | _display << 2 | _cursor << 1 | (_blink = state));
}

uint8_t LCDIC2::busy() { // TODO cant extract ack
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

uint8_t LCDIC2::cursor(uint8_t x, uint8_t y) { // TODO x/y conflict; y change assembly
  return transmit(LCDIC2_DDRAM | min(y, _height - 1) << 6 | min(x, _width - 1));
}

void LCDIC2::cursorLeft() {
  transmit(LCDIC2_MOVE | LCDIC2_CURSOR | LCDIC2_LEFT);
}

void LCDIC2::cursorRight() {
  transmit(LCDIC2_MOVE | LCDIC2_CURSOR | LCDIC2_RIGHT);
}

void LCDIC2::display(bool state) {
  transmit(LCDIC2_DISPLAY | (_display = state) << 2 | _cursor << 1 | _blink);
}

void LCDIC2::glyph(uint8_t character) {
  transmit(character, true);
}

void LCDIC2::glyph(uint8_t id, uint8_t map[]) {
  transmit(LCDIC2_CGRAM | id << 3);
  for (uint8_t i = 0; i < 8; i++) transmit(map[i], true);
  transmit(LCDIC2_DDRAM);
}

void LCDIC2::home() {
  transmit(0b10);
  pulseIn(A4, LOW, 2500);
}

void LCDIC2::leftToRight() {
  transmit(LCDIC2_MODE | (_gain = LCDIC2_INC >> 1) << 1 | _shift);
}

void LCDIC2::moveLeft() {
  transmit(LCDIC2_MOVE | LCDIC2_SHIFT | LCDIC2_LEFT);
}

void LCDIC2::moveRight() {
  transmit(LCDIC2_MOVE | LCDIC2_SHIFT | LCDIC2_RIGHT);
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
  Wire.write(LCDIC2_MODE | _gain << 1);
  Wire.endTransmission();
  busy();
}

void LCDIC2::rightToLeft() {
  transmit(LCDIC2_MODE | (_gain = LCDIC2_DEC >> 1) << 1 | _shift);
}

void LCDIC2::shift(bool state) {
  transmit(LCDIC2_MODE | _gain << 1 | (_shift = state));
}

uint8_t LCDIC2::transmit(uint8_t data, bool mode = false) {
  Wire.beginTransmission(_address);
  Wire.write(data & 0b11110000 | 0b100 | mode);
  Wire.write(0b1000);
  Wire.write(data << 4 | 0b0100 | mode);
  Wire.write(0b1000);
  Wire.endTransmission();
  return busy();
}
