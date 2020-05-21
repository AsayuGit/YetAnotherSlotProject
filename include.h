#ifndef INCLUDE
#define INCLUDE

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
//#include <unistd.h>

#define LINUX // Target os

#define SLOTMACHINE_NAME "YetAnotherSlotProject"

typedef struct {
    int x;
    int y;
} Vector2i;

Vector2i FullscreenRES = {1920, 1080};
Vector2i WindowRES = {1600, 900};
Vector2i ScreenRES = {1600, 900};

#define NB_OF_THEMES 1

#define LINES 24
#define COLUMNS 80

// Pour la futur portabilité
#ifdef LINUX
#define CLEAR "clear"
#elif WINDOWS
#define CLEAR "cls"
#endif

#define FontPath "Fonts/SlotFont.txt"
#define ImagePath "Images/"
#define SoundPath "Sounds/"


#endif