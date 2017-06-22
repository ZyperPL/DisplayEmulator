#pragma once

#include "Adafruit-GFX-Library/Adafruit_GFX.h"

#include "zylib.h"

#define WINDOW_NAME "Display Emulator Screen"

#define WHITE 1
#define BLACK 0

class Display : public Adafruit_GFX
{
  public:
    Display(int16_t w, int16_t h);
    void drawPixel(int16_t x, int16_t y, uint16_t color);

    void clearDisplay(void);
    void invertDisplay(uint8_t i);
    void display();

    void startscrollright(uint8_t start, uint8_t stop);
    void startscrollleft(uint8_t start, uint8_t stop);

    void startscrolldiagright(uint8_t start, uint8_t stop);
    void startscrolldiagleft(uint8_t start, uint8_t stop);
    void stopscroll(void);

    void dim(boolean dim);
};
