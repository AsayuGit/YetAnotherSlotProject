#ifndef INCLUDE
#define INCLUDE

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define LINUX // Target os

// Pour la futur portabilité
#ifdef LINUX
#define CLEAR "clear"
#elif WINDOWS
#define CLEAR "cls"
#endif

#endif