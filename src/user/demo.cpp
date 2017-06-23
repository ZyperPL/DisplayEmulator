#include "base/display.h"
#include "demo.h"

#define WIDTH   128
#define HEIGHT  64

Display *display;

void init()
{
  display = new Display(WIDTH, HEIGHT);
}

void update()
{
  display->clearDisplay();
  display->setCursor(1, 0);
  display->println("Hello World");
  display->println(time(0));
  display->fillCircle(60, 50, 10, 1);
}

