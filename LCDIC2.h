#ifndef LCDIC2_H
#define LCDIC2_H

#include "Arduino.h"
#include "Wire.h"

#define LCDIC2_MODE         0b0000000100
#define LCDIC2_DISPLAY      0b0000001000
#define LCDIC2_MOVE         0b0000010000
#define LCDIC2_FUNCTION     0b0000100000
#define LCDIC2_CGRAM        0b0001000000
#define LCDIC2_DDRAM        0b0010000000

#define LCDIC2_RW           0b10
#define LCDIC2_RS           0b1

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

#define LCDIC2_DEC          0b00
#define LCDIC2_INC          0b10

#define LCDIC2_LONG_DELAY   5000
#define LCDIC2_SHORT_DELAY  150

class LCDIC2 {
  private:
    bool _blink = false, _cursor = true, _display = true, _gain, _shift = false;
    uint8_t _address, _height, _width, k;

  public:
    LCDIC2(uint8_t address, uint8_t width, uint8_t height);
    void backlight(bool state);
    void begin();
    void blink(bool state);
    uint8_t busy();
    void clear();
    void cursor(bool state);
    void cursor(uint8_t x, uint8_t y);
    void display(bool state);
    void home();
    void leftToRight();
    void rightToLeft();
    void moveLeft();
    void moveRight();
    size_t print(const String str) {
      size_t n = 0;
      while (n < str.length()) transmit(str[n++], LCDIC2_RS);
      return n;
    }

    void reset();
    uint8_t transmit(uint8_t data, uint8_t mode = 0);
};

#endif
