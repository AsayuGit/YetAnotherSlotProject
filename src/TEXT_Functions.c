#include "TEXT_Functions.h"

void clearInputBuffer (void) { // Clear stdin
    while ((getchar()) != '\n');
}

// Change la taille de la console
void SetConsoleSize(int Height, int Width){
    printf("\e[8;%d;%dt\n", Height, Width);
}

// Place le curseur au coordonées (y, x)
void SetCursorAt(int y, int x){
    printf("\033[%d;%dH", y, x);
}

void LoadTabFromFile(int TabY, int TabX, char CardIndex[TabY][TabX], FILE* FileStream){ // Charge un fichier dans un tableau char**
    for (int i = 0; i < TabY; i++){
        fgets(CardIndex[i], TabX, FileStream); // récupère une ligne d'un fichier
        //fscanf(FileStream, "%[^\n]", CardIndex[i]);
    }
}

// Efface une zone rectangulaire de l'écran
void clearRegion(int y, int x, int height, int width){
    height += y;
    for (; y < height; y++){
        SetCursorAt(y, x); // On déplace le curseur a l'endoit de l'ecran que l'on veut éffacer
        for (int i = 0; i < width; i++){
            printf(" ");
        }
    }
}

// Affiche un fichier texte a l'ecran
void blitBackgroundToScreen(FILE* FileStream){
    char tempChar;
    while ((tempChar = fgetc(FileStream)) != EOF){
        printf("%c", tempChar);
    }
}

void DisplayTab(int TabY, int TabX, char CardIndex[TabY][TabX]){ // Affiche le contenue d'un tableau char**
    for (int i = 0; i < TabY; i++){
        printf("%3d %s",i, CardIndex[i]); // récupère une ligne d'un tableau char**
    }
}

void DisplayCardAt(int TabY, int TabX, char CardIndex[TabY][TabX], int CardID, int CardYSize, int y, int x){ // Affiche une carte a une endroid précis de l'écran
    int CurrentLine = (CardID * CardYSize); // Ligne a afficher
    for (int i = 0; i < CardYSize; i++){ // On affiche la carte a l'ecran
        SetCursorAt(y + i, x); // On déplace le curseur a l'endoit de l'ecran ou l'on veut dessiner la carte
        printf("%s", CardIndex[CurrentLine]); // récupère une ligne d'un tableau char**
        CurrentLine++;
    }
}

void AnimateCard(int TabY, int TabX, char CardIndex[TabY][TabX], int CardID, int CardYSize, int y, int x, int *Offset, int *Steps){ // Affiche une carte a une endroid précis de l'écran
    (*Offset)++; // On décale l'offset d'un cran
    int loopbackLimit; // l'endroit on on dois remonter au dessus du tableau
    
    if ((*Steps) > 0){// LoopCode
        loopbackLimit = TabY;
        if ((*Offset) > loopbackLimit){
            (*Offset) = 0;
            (*Steps)--;
        }
    }else{
        loopbackLimit = (CardID * CardYSize); // Arriver précisément au bon endroit
        if ((*Offset) > loopbackLimit){
            (*Offset) = (CardID * CardYSize);
            (*Steps)--;
        }
        //(*coordinates).y = stepOffset * newID + originOffset;
    }

    int CurrentLine; // Ligne a afficher
    for (int i = 0; i < CardYSize; i++){ // On affiche la carte a l'ecran
        SetCursorAt(y + i, x); // On déplace le curseur a l'endoit de l'ecran ou l'on veut dessiner la carte
        CurrentLine = (*Offset) + i;
        if (CurrentLine >= TabY){
            CurrentLine -= TabY;
        }
        printf("%s", CardIndex[CurrentLine]); // récupère une ligne d'un tableau char**
    }
}