#ifndef LCDIC2_H
#ifndef ILCDIC2_H

#define ILCDIC2_H
#define LCDIC2_H

#include "Arduino.h"
#include "Wire.h"
#include "syntax.h"

#define LCDIC2_MODE         0b0000000100
#define LCDIC2_DISPLAY      0b0000001000
#define LCDIC2_MOVE         0b0000010000
#define LCDIC2_FUNCTION     0b0000100000
#define LCDIC2_CGRAM        0b0001000000
#define LCDIC2_DDRAM        0b0010000000

#define LCDIC2_BITS_8       0b1000
#define LCDIC2_BITS_4       0b0

#define LCDIC2_LINES_1      0b0
#define LCDIC2_LINES_2      0b10000

#define LCDIC2_DOTS_10      0b100
#define LCDIC2_DOTS_8       0b0

#define LCDIC2_CURSOR       0b0000
#define LCDIC2_SHIFT        0b1000

#define LCDIC2_LEFT         0b000
#define LCDIC2_RIGHT        0b100

#define LCDIC2_DEC          0b00
#define LCDIC2_INC          0b10

#define LCDIC2_LONG_DELAY   5000
#define LCDIC2_SHORT_DELAY  150

class ILCDIC2 {
  protected:
    bool _blink = false, _cursor = true, _display = false, _gain, _shift = false;
    uint8_t _address, _height, _width;

  public:
    ILCDIC2(uint8_t address, uint8_t width, uint8_t height);
    void begin();
    void clear();
    void cursor(uint8_t x, uint8_t y);
    void display(bool state);
    void home();
    void leftToRight();
    void rightToLeft();

    void transmit(uint8_t data, uint8_t = 0);
};

class LCDIC2: public ILCDIC2 {
  public:
    LCDIC2(uint8_t address, uint8_t width, uint8_t height): ILCDIC2(address, width, height) {}

    scroll(bool state = true) {
      transmit(LCDIC2_MODE | _gain << 1 | (_shift = state));
    }

    void blink(bool state) {
      transmit(LCDIC2_DISPLAY | _display << 2 | _cursor << 1 | (_blink = state));
    }

    void backlight(bool state) {
      Wire.beginTransmission(_address);
      Wire.write(state << 3);
      Wire.endTransmission();
    }

    void cursor(bool state) {
      transmit(LCDIC2_DISPLAY | _display << 2 | (_cursor = state) << 1 | _blink);
    }

    void cursor(uint8_t x, uint8_t y) {
      ILCDIC2::cursor(x, y);
    }

    void moveLeft() {
      transmit(LCDIC2_MOVE | LCDIC2_SHIFT | LCDIC2_LEFT);
    }

    void moveRight() {
      transmit(LCDIC2_MOVE | LCDIC2_SHIFT | LCDIC2_RIGHT);
    }

    size_t print(const String str) {
      size_t n = 0;
      while (n < str.length()) transmit(str[n++], Rs);
      return n;
    }
};

#endif
#endif
