#ifndef INCLUDE
#define INCLUDE

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
//#include <unistd.h>

#define LINUX // Target os

#define SLOTMACHINE_NAME "YetAnotherSlotProject"

#define SCREEN_X 640
#define SCREEN_Y 480

#define LINES 24
#define COLUMNS 80

// Pour la futur portabilité
#ifdef LINUX
#define CLEAR "clear"
#elif WINDOWS
#define CLEAR "cls"
#endif

#define FontPath "Fonts/SlotFont.txt"
//#define FontPath "Fonts/Test.txt"
#define ImagePath "Images/"

typedef struct Vector2i {
    int x;
    int y;
} Vector2i;


#endif