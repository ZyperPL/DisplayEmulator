#include "display.h"

Display::Display(int16_t w, int16_t h) : Adafruit_GFX (w, h)
{
  zSetWindow(zCreateWindow(w, h, WINDOW_NAME));
  setTextColor(1);
}

void Display::drawPixel(int16_t x, int16_t y, uint16_t color) 
{
  if((x < 0) || (y < 0) || (x >= _width) || (y >= _height)) return;

  x *= COMPONENTS;
  y *= COMPONENTS;

  int16_t t;
  switch (rotation) 
  {
    case 1:
      t = x;
      x = WIDTH  - 1 - y;
      y = t;
      break;
    case 2:
      x = WIDTH  - 1 - x;
      y = HEIGHT - 1 - y;
      break;
    case 3:
      t = x;
      x = y;
      y = HEIGHT - 1 - t;
      break;
    }


  uint8_t c = (uint8_t)(color*255);
  zDrawPixel(x, y, zRGB(c, c, c));
}

void Display::clearDisplay(void)
{
  zClear();
}
