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

#ifndef TEXT_FUNCTIONS
#define TEXT_FUNCTIONS

#include "include.h"

// Clear stdin
void clearInputBuffer (void);

// Change la taille de la console
void SetConsoleSize(int Height, int Width);

// Place le curseur au coordonées (y, x)
void SetCursorAt(int y, int x);

// Charge un fichier dans un tableau char**
void LoadTabFromFile(int TabY, int TabX, char CardIndex[TabY][TabX], FILE* FileStream);

// Efface une zone rectangulaire de l'écran
void clearRegion(int y, int x, int height, int width);

// Affiche un fichier texte a l'ecran
void blitBackgroundToScreen(FILE* FileStream);

// Affiche le contenue d'un tableau char**
void DisplayTab(int TabY, int TabX, char CardIndex[TabY][TabX]);

// Affiche une carte a une endroid précis de l'écran
void DisplayCardAt(int TabY, int TabX, char CardIndex[TabY][TabX], int CardID, int CardYSize, int y, int x);

// Affiche une carte a une endroid précis de l'écran
void AnimateCard(int TabY, int TabX, char CardIndex[TabY][TabX], int CardID, int CardYSize, int y, int x, int *Offset, int *Steps);

#endif