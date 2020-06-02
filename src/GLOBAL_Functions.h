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

#ifndef GLOBAL_FUNCTIONS
#define GLOBAL_FUNCTIONS

#include "include.h"

void milliSleep(int milliseconds);

// fonctionne de la même façon que strcmp mais avec des int
int intcmp(int tab1[], int tab2[], int arraySize);

// teste si une la combinaison tab1 est composé des chiffres tab2 (chaque chiffre étant présent qu'une fois dans la combinaison)
int intUni(int tab1[], int tab2[], int arraySize);

// Retourne le nombre de fois qu'un chiffre apparait dans une combinaision
int nbOfOcurences(int tab1[], int number, int arraySize);

void tirage(int * Gains, int Mise, char TabDeck[], int SlotIndex[], int WinRewards[], char *BJackpot, char Mode, char Lucky);

#endif