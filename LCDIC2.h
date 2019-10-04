#ifndef LCDIC2_H
#define LCDIC2_H

#include <Wire.h>

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

class LCDIC2 {
  private:
    bool _backlight = true, _blink = false, _cursor = true, _display = true, _font = 0, _gain = true, _shift = false;
    uint8_t _address, _height = 0, _width = 0;

    bool busy();
    uint8_t request(uint8_t rs);
    uint8_t start(uint8_t y);
    bool wait(uint16_t us);
    bool write(uint8_t data, uint8_t rs = 0);
    bool writeHigh(uint8_t data, uint8_t rs = 0);
    bool writeLow(uint8_t data, uint8_t rs = 0);

  public:
    const bool backlight = _backlight, blink = _blink, cursor = _cursor, display = _display, font = _font, gain = _gain, shift = _shift;
    const uint8_t height = _height, width = _width;

    LCDIC2(uint8_t address, uint8_t width, uint8_t height, bool font = 0);
    bool begin();
    char charAt(uint8_t x, uint8_t y);
    bool clear();
    bool cursorLeft();
    bool cursorRight();
    void getCursor(uint8_t &x, uint8_t &y);
    bool getGlyph(uint8_t glyph, uint8_t *&buffer);
    bool home();
    bool leftToRight();
    bool moveLeft();
    bool moveRight();
    bool print(uint8_t glyph);
    uint8_t print(String data);
    bool rightToLeft();
    bool setBacklight(bool state);
    bool setBlink(bool state);
    bool setCursor(bool state);
    bool setCursor(uint8_t x, uint8_t y);
    bool setDisplay(bool state);
    bool setFont(bool font);
    bool setGlyph(uint8_t glyph, uint8_t buffer[]);
    bool setLines(uint8_t height);
    bool setShift(bool state);
};

#endif
