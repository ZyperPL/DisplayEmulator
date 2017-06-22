#include <cstdio>
#include <cstdlib>
#include <string>

#include "display.h"

void printHelp(int argc, char *argv[])
{
  argc=argc; // prevent warning
  printf("Usage:\n%s\t <program_path>\n", argv[0]);
}

int main(int argc, char *argv[])
{
  if (argc <= 1)
  {
    printHelp(argc, argv);
    return 1;
  }

  int16_t w = 128;
  int16_t h = 64;

  // initialize screen
  Display *display = new Display(w, h);

  while (zIsOpen())
  {

	display->clearDisplay();
	display->setTextColor(WHITE);

  // demo
  display->setTextSize(1);
  display->setCursor(0, 0);
  display->print("22.06.2017 12:30:14");
  const int STAGE_DOT_SIZE = 2;
  const int STAGE_DOT_SPACING = 4;
  int stage = 2;
  for (int i = 0; i < 6; i++) {
    display->drawCircle(display->width()/2 - 
        (6 * STAGE_DOT_SIZE * STAGE_DOT_SPACING)/2 + STAGE_DOT_SPACING + 
        (i * STAGE_DOT_SIZE * STAGE_DOT_SPACING),
                       display->height() - STAGE_DOT_SIZE - 1, 
                       STAGE_DOT_SIZE, 1);
  }
  display->fillCircle(display->width()/2 - 
      (6 * STAGE_DOT_SIZE * STAGE_DOT_SPACING)/2 + STAGE_DOT_SPACING + 
      (stage * STAGE_DOT_SIZE * STAGE_DOT_SPACING),
                     display->height() - STAGE_DOT_SIZE - 1, 
                     STAGE_DOT_SIZE, 1);


    zClear();
    zUpdate();
  }

  zFree();

  return 0;
}
