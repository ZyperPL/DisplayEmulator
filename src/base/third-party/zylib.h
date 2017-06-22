#pragma once

#include <cstdio>
#include <cstdlib>
#include <string>
#include <cstring>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define COMPONENTS 3

/*
 * pixels
 */
struct zPixel
{
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a = 255;
};

zPixel zRGB (uint8_t r, uint8_t g, uint8_t b);
zPixel zRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

/*
 * window
 */
struct Zwindow
{
  uint32_t width;
  uint32_t height;
  double aspect;  
  const char *name;

  uint8_t *buffer;
};


struct Zwindow zCreateWindow(uint32_t w, uint32_t h, const char *name);

bool zIsOpen();
void zClear(zPixel c);
void zClear();

void zUpdate();

void zDrawPixel(uint16_t x, uint16_t y, zPixel c = { 255, 255, 255, 255 });

void zFree();
