#ifndef INCLUDE
#define INCLUDE

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define LINUX // Target os

#define SLOTMACHINE_NAME "YetAnotherSlotProject"

#define SCREEN_X 640
#define SCREEN_Y 480

// Pour la futur portabilité
#ifdef LINUX
#define CLEAR "clear"
#elif WINDOWS
#define CLEAR "cls"
#endif

#endif