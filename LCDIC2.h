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
  public:
    bool _backlight = true, _blink = false, _cursor = true, _display = true, _font = 0, _gain = LCDIC2_INC, _shift = false;
    uint8_t _address, _height = 0, _width = 0;
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
    uint8_t charAt(uint8_t x, uint8_t y);
    bool clear();
    bool cursorLeft();
    bool cursorRight();
    void getCursor(uint8_t &x, uint8_t &y);
    bool glyph(uint8_t id);
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
    bool setGlyph(uint8_t id, uint8_t map[]);
    bool setLines(uint8_t height);
    bool setShift(bool state);
};

#endif
