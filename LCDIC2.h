#ifndef LCDIC2_H
#define LCDIC2_H

#include "Wire.h"
#include "Arduino.h"

#define LCDIC2_MODE         0b100
#define LCDIC2_DISPLAY      0b1000
#define LCDIC2_MOVE         0b10000
#define LCDIC2_FUNCTION     0b100000
#define LCDIC2_CGRAM        0b1000000
#define LCDIC2_DDRAM        0b10000000

#define LCDIC2_BITS_4       0b0
#define LCDIC2_BITS_8       0b10000

#define LCDIC2_LINES_1      0b0
#define LCDIC2_LINES_2      0b1000

#define LCDIC2_DOTS_8       0b0
#define LCDIC2_DOTS_10      0b100

#define LCDIC2_CURSOR       0b0000
#define LCDIC2_SHIFT        0b1000

#define LCDIC2_LEFT         0b000
#define LCDIC2_RIGHT        0b100

#define LCDIC2_DEC          0b0
#define LCDIC2_INC          0b1

class LCDIC2 {
  private:
    bool _backlight = true, _blink = false, _busy, _cursor = true, _display = true, _font = 0, _gain = LCDIC2_INC, _shift = false;
    uint8_t _address, _height = 0, _width = 0;
    uint8_t line[4];
    bool flag();
    bool send(uint8_t data, uint16_t us = 0);
    void wait(uint16_t us);
    bool write(uint8_t data, uint8_t rs = 0);
    bool writeHigh(uint8_t data, uint8_t rs = 0);
    bool writeLow(uint8_t data, uint8_t rs = 0);

  public:
    const bool backlight = _backlight, blink = _blink, cursor = _cursor, display = _display, font = _font, gain = _gain, shift = _shift;
    const uint8_t height = _height, width = _height & 0b11110000, busy = _busy;
    LCDIC2(uint8_t address, uint8_t width, uint8_t height, bool font = 0);
    bool begin();
    bool clear();
    bool cursorLeft();
    bool cursorRight();
    bool glyph(uint8_t character);
    bool glyph(uint8_t id, uint8_t map[]);
    bool home();
    bool leftToRight();
    bool moveLeft();
    bool moveRight();
    size_t print(String data);
    bool rightToLeft();
    bool setBacklight(bool state);
    bool setBlink(bool state);
    bool setCursor(bool state);
    bool setCursor(uint8_t x, uint8_t y);
    bool setDisplay(bool state);
    bool setFont(bool font);
    bool setLines(uint8_t height);
    bool setShift(bool state);


    void getXY(uint8_t &x, uint8_t &y) {
      Wire.beginTransmission(_address);
      Wire.endTransmission(1);
      writeHigh(0b1111, 0b110);
      Wire.requestFrom(uint8_t(_address), uint8_t(2));
      x = Wire.read();
      writeLow(0b1111, 0b110);
      Wire.requestFrom(uint8_t(_address), uint8_t(2));
      x = Wire.read() & 0b11110000 | x >> 4;
      y = x > 0x39;
      x = x - y * 0x40;
    }
};

#endif
