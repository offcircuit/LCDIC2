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
#define LCDIC2_RS           0b1000000000

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

#define LCDIC2_BITS 4

#define rs 0b00000001  // Register select bit


class LCDIC2 {
  private:
    bool _blink = true, _cursor = true, _display = true, _shift = false;
    uint8_t _address, _height, _width;
    String s;

  public:
    LCDIC2(uint8_t address, uint8_t width, uint8_t height);
    void backlight(bool state);
    void blink(bool state);
    void busy();
    void clear();
    void cursor(bool state);
    void cursor(uint8_t x, uint8_t y);
    void display(bool state);
    void home();
    void leftToRight();
    void rightToLeft();
    void moveLeft();
    void moveRight();
    size_t print(const String str, uint8_t mode=rs) {
      size_t n = 0;
      while (n < str.length()) transmit(str[n++], rs);
      return n;
    }

    void reset();

    void begin() {
      Wire.begin();
      delay(50);

      reset();

      Wire.beginTransmission(_address);
      Wire.write(0b11);
      Wire.write(0b11);
      Wire.write(0b11);
      Wire.write(0b10);
      Wire.endTransmission();

      busy();

      transmit(LCDIC2_FUNCTION | LCDIC2_BITS_4 | LCDIC2_LINES_2 | LCDIC2_DOTS_8);
      transmit(LCDIC2_DISPLAY | _display << 2 | _cursor << 1 | _blink);

      clear();
      leftToRight();
      display(true);
    }

    uint8_t transmit(uint8_t data, uint8_t mode = 0) {
      Wire.beginTransmission(_address);
      Wire.write(data & 0b11110000 | 0b100 | mode);
      Wire.write(0b1000);
      busy();
      Wire.write((data & 0b00001111) << 4 | 0b100 | mode);
      Wire.write(0b1000);
      busy();
      Wire.endTransmission();
      delayMicroseconds(150);
    }
};

#endif
