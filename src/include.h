/*
    <YetAnotherSlotProject a (yet another) Slotmachine simulator>
    Copyright (C) <2020>  <RAIMBAUD Killian & TOUGARD Enzo>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation version 2 of the License.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#ifndef INCLUDE
#define INCLUDE

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#include "types.h"

#include "GLOBAL_Functions.h"
#include "TEXT_Functions.h"
#include "SDL_Functions.h"

#define LINUX // Target os

#define SLOTMACHINE_NAME "YetAnotherSlotProject"

#define NB_OF_THEMES 1

#define LINES 77
#define COLUMNS 263

#define NBL 7
#define WIN 13

// Pour la futur portabilité
#ifdef LINUX
#define CLEAR "clear"
#elif WINDOWS
#define CLEAR "cls"
#endif

#define FontPath "Fonts/"
#define ImagePath "Images/"
#define SoundPath "Sounds/"


#endif