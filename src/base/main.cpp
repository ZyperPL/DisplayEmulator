#include <cstdio>
#include <cstdlib>
#include <string>
#include <dlfcn.h>

#include "zylib.h"

void printHelp(int argc, char *argv[])
{
  argc=argc; // prevent warning
  printf("Usage:\n%s\t ./<program_path>\n", argv[0]);
}

int main(int argc, char *argv[])
{
  if (argc <= 1)
  {
    printHelp(argc, argv);
    return 1;
  }

  // load external library (user main)
  void* handle = dlopen(argv[1], RTLD_LAZY);

  if (!handle)
  {
    fprintf(stderr, "Cannot load '%s'!\n", argv[1]);
    fprintf(stderr, "Error: %s\n", dlerror());
    printHelp(argc, argv);
    return 2;
  }

  void (*init)();
  void (*update)();

  init   = (void(*)())dlsym(handle, "init");
  if (!init) fprintf(stderr, "%s\n", dlerror());  
  update = (void(*)())dlsym(handle, "update");  
  if (!update) fprintf(stderr, "%s\n", dlerror());

  init();

  Zwindow_t* (*getWnd)() = (Zwindow_t*(*)())(dlsym(handle, "getWindow"));
  zSetWindow(getWnd());  

  while (zIsOpen())
  {
    zClear();

    update();
    
    zUpdate();
  }

  dlclose(handle);

  zFree();

  return 0;
}
