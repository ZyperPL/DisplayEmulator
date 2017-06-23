#pragma once

#include "zylib.h"

extern "C"
{
  void init();
  void update();
  Zwindow_t *getWindow() { return zGetWindow(); };
}
