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

#define LCDIC2_CURSOR       0b0000
#define LCDIC2_SHIFT        0b1000

#define LCDIC2_LEFT         0b000
#define LCDIC2_RIGHT        0b100

#define LCDIC2_DEC          0b0
#define LCDIC2_INC          0b1

class LCDIC2 {
  private:
    bool _backlight = true, _blink = false, _cursor = true, _display = true, _font = 0, _gain = true, _shift = false;
    uint8_t _address, _height = 0, _width = 0;
    //void bounds(uint8_t &x, uint8_t &y);
    bool busy();
    uint8_t flag();
    uint8_t request(uint8_t rs);
    bool send(uint8_t data, uint16_t us = 0);
    void wait(uint16_t us);
    bool write(uint8_t data, uint8_t rs = 0);
    bool writeHigh(uint8_t data, uint8_t rs = 0);
    bool writeLow(uint8_t data, uint8_t rs = 0);

  public:
    const bool backlight = _backlight, blink = _blink, cursor = _cursor, display = _display, font = _font, gain = _gain, shift = _shift;
    const uint8_t height = _height, width = _width;
    LCDIC2(uint8_t address, uint8_t width, uint8_t height, bool font = 0);
    bool begin();
    bool end();
    char charAt(uint8_t x, uint8_t y);
    bool clear();
    bool cursorLeft();
    bool cursorRight();
    // void getCursor(uint8_t &x, uint8_t &y);
    bool home();
    bool leftToRight();
    bool moveLeft();
    bool moveRight();
    bool print(uint8_t glyph);
    size_t print(String data);
    bool rightToLeft();
    bool setBacklight(bool state);
    bool setBlink(bool state);
    bool setCursor(bool state);
    //bool setCursor(uint8_t x, uint8_t y);
    bool setDisplay(bool state);
    bool setFont(bool font);
    bool setGlyph(uint8_t glyph, uint8_t data[]);
    bool setLines(uint8_t height);
    bool setShift(bool state);
    bool sift(uint8_t glyph, uint8_t *&data);


    uint8_t start(uint8_t y) {
      return ((y % 2) << 6) + ((y / 2) * _width);
    }

    uint8_t length(uint8_t y) {
      return (_height - 1) ? (((0b1001 >> (_height > 2)) - (_width / _height == 4)) << 2 | 0b11) + (((_width / _height == 4) & (y / 2)) << 3) : 0b1001111;
    }

    void bounds(uint8_t &x, uint8_t &y) {
      y = min(y, uint8_t(_height - 1));
      x = min(x, uint8_t(length(y)));
    }


    void getCursor(uint8_t &x, uint8_t &y) {
      x = request(0b10);
      y = 0;
      while ((y < _height - 1) && (x > start((0b10011100 >> 6 - (2 * y)) & 0b11))) y++;
      x = x - start((0b10011100 >> 6 - (2 * (y - 1))) & 0b11);
    }

    bool setCursor(uint8_t x, uint8_t y) {
      bounds(x, y);
      return write(LCDIC2_DDRAM | start(y) + x);
    }
};


#endif
