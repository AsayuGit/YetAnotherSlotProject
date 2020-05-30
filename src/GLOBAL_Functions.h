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