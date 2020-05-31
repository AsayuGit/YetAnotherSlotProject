#include "GLOBAL_Functions.h"

void milliSleep(int milliseconds){
    struct timespec TimeStructure;
    if (milliseconds > 0){ // S'il y a un délais alors on convertis le temps d'attente en secondes et nanosecondes avant d'attendre le délais correspondant
        TimeStructure.tv_sec = milliseconds / 1000;
        TimeStructure.tv_nsec = (milliseconds % 10000) * 1000000;
        nanosleep(&TimeStructure, NULL);
    }
}

// fonctionne de la même façon que strcmp mais avec des int
int intcmp(int tab1[], int tab2[], int arraySize){
    for (int i = 0; i < arraySize; i++){
        if (tab1[i] != tab2[i]){
            return 0;
        }
    }
    return 1;
}

// teste si une la combinaison tab1 est composé des chiffres tab2 (chaque chiffre étant présent qu'une fois dans la combinaison)
int intUni(int tab1[], int tab2[], int arraySize){
    int Ccount;
    for (int i = 0; i < arraySize; i++){
        Ccount = 0;
        for (int j = 0; j < arraySize; j++){
            if (tab2[i] == tab1[j]){
                Ccount++;
            }
        }
        if (Ccount != 1){
            return 0;
        }
    }
    return 1;
}

// Retourne le nombre de fois qu'un chiffre apparait dans une combinaision
int nbOfOcurences(int tab1[], int number, int arraySize){
    int Ccount = 0;
    for (int i = 0; i < arraySize; i++){
        if (tab1[i] == number){
            Ccount++;
        }
    }
    return Ccount;
}

void tirage(int * Gains, int Mise, char TabDeck[], int SlotIndex[], int WinRewards[], char *BJackpot, char Mode, char Lucky){
    (*BJackpot) = 0;
    // Génération aléatoire des slots (tirage)
    for (int i = 0; i < 3; i++){
        SlotIndex[i] = rand()%NBL; // int
    }

    // Recherche de la valeur du gain
    *Gains = 0;
    // Luck manipulation (for debug purposes)
    if (Lucky){
        
        if (Mode){
            SlotIndex[0] = 1;
            SlotIndex[1] = 1;
            SlotIndex[2] = 1;
        }else{
            SlotIndex[0] = 5;
            SlotIndex[1] = 5;
            SlotIndex[2] = 5;
        }
    }

    if (Mode){ // Casino mode
        // "Any color" combination
        if (((SlotIndex[0] == 2) || (SlotIndex[0] == 5) || (SlotIndex[0] == 1)) && ((SlotIndex[1] == 2) || (SlotIndex[1] == 5) || (SlotIndex[1] == 1)) && ((SlotIndex[2] == 2) || (SlotIndex[2] == 5) || (SlotIndex[2] == 1))){ // Any blue
            *Gains = WinRewards[1] * Mise; //printf("Nya 0\n");
        } else if (((SlotIndex[0] == 0) || (SlotIndex[0] == 1) || (SlotIndex[0] == 4)) && ((SlotIndex[1] == 0) || (SlotIndex[1] == 1) || (SlotIndex[1] == 4)) && ((SlotIndex[2] == 0) || (SlotIndex[2] == 1) || (SlotIndex[2] == 4))){ // Any red
            *Gains = WinRewards[1] * Mise; //printf("Nya 1\n");
        } else if (((SlotIndex[0] == 1) || (SlotIndex[0] == 3) || (SlotIndex[0] == 6)) && ((SlotIndex[1] == 1) || (SlotIndex[1] == 3) || (SlotIndex[1] == 6)) && ((SlotIndex[2] == 1) || (SlotIndex[2] == 3) || (SlotIndex[2] == 6))){ // Any white
            *Gains = WinRewards[1] * Mise; //printf("Nya 2\n");
        }
        
        // Half specific combination
        if (((SlotIndex[0] == 0) || (SlotIndex[0] == 3) || (SlotIndex[0] == 5)) && ((SlotIndex[1] == 0) || (SlotIndex[1] == 3) || (SlotIndex[1] == 5)) && ((SlotIndex[2] == 0) || (SlotIndex[2] == 3) || (SlotIndex[2] == 5))){ // Any mixed sevens
            *Gains = WinRewards[6] * Mise; //printf("Nya 3\n");
        } else if (((SlotIndex[0] == 0) || (SlotIndex[0] == 1) || (SlotIndex[0] == 4)) && ((SlotIndex[1] == 1) || (SlotIndex[1] == 3) || (SlotIndex[1] == 6)) && ((SlotIndex[2] == 2) || (SlotIndex[2] == 5) || (SlotIndex[2] == 1))){ // Any red, Any White, Any Blue
            *Gains = WinRewards[4] * Mise; //printf("Nya 4\n");
        } else if (((SlotIndex[0] == 2) || (SlotIndex[0] == 4) || (SlotIndex[0] == 6)) && ((SlotIndex[1] == 2) || (SlotIndex[1] == 4) || (SlotIndex[1] == 6)) && ((SlotIndex[2] == 2) || (SlotIndex[2] == 4) || (SlotIndex[2] == 6))){ // Any bar
            *Gains = WinRewards[2] * Mise; //printf("Nya 5\n");
        } else if (nbOfOcurences(SlotIndex, 1, 3) == 2){ // Any two wild
            *Gains = WinRewards[2] * Mise;
        } else if (nbOfOcurences(SlotIndex, 1, 3) == 1){ // Any one wild
            *Gains = WinRewards[1] * Mise;
        }

        // Specific combinaition
        if (intcmp(SlotIndex, (int[3]){4, 4, 4}, 3)){ // Full red bar
            *Gains = WinRewards[3] * Mise; //printf("Nya 6\n");
        } else if (intcmp(SlotIndex, (int[3]){6, 6, 6}, 3)){ // Full white bar
            *Gains = WinRewards[4] * Mise; //printf("Nya 7\n");
        } else if (intcmp(SlotIndex, (int[3]){2, 2, 2}, 3)){ // Full blue bar
            *Gains = WinRewards[5] * Mise; //printf("Nya 8\n");
        } else if (intcmp(SlotIndex, (int[3]){5, 5, 5}, 3)){ // Full blue sevens
            *Gains = WinRewards[7] * Mise; //printf("Nya 9\n");
        } else if (intcmp(SlotIndex, (int[3]){3, 3, 3}, 3)){ // Full white sevens
            *Gains = WinRewards[8] * Mise; //printf("Nya 10\n");
        } else if (intcmp(SlotIndex, (int[3]){0, 0, 0}, 3)){ // Full red sevens
            *Gains = WinRewards[9] * Mise; //printf("Nya 11\n");
        } else if (intcmp(SlotIndex, (int[3]){0, 3, 5}, 3)){ // Red white blue seven
            *Gains = WinRewards[10] * Mise; //printf("Nya 12\n");
        } else if (intcmp(SlotIndex, (int[3]){1, 1, 1}, 3)){ // Wild x3
            if (Mise < 3){
                *Gains = WinRewards[11] * Mise; //printf("Nya 13\n");
            }else{
                *Gains = WinRewards[12];
                (*BJackpot) = 1;
            }
        }
    }else{ // Texte mode
        // "Any" combination
        if ((SlotIndex[0] == SlotIndex[1]) && (SlotIndex[0] == SlotIndex[2])){ // Any same
            *Gains = WinRewards[9] * Mise;
        }
        
        // Half specific combination
        if (intUni(SlotIndex, (int[3]){3, 1, 0}, 3)){ // Any mixed NEB
            *Gains = WinRewards[6] * Mise;
        } else if (nbOfOcurences(SlotIndex, 5, 3) == 2){ // Any two S
            *Gains = WinRewards[4] * Mise;
        } else if (((nbOfOcurences(SlotIndex, 6, 3) == 1) && (nbOfOcurences(SlotIndex, 1, 3) == 0) && (nbOfOcurences(SlotIndex, 4, 3) == 0)) || ((nbOfOcurences(SlotIndex, 1, 3) == 1) && (nbOfOcurences(SlotIndex, 6, 3) == 0) && (nbOfOcurences(SlotIndex, 4, 3) == 0)) || ((nbOfOcurences(SlotIndex, 4, 3) == 1) && (nbOfOcurences(SlotIndex, 1, 3) == 0) && (nbOfOcurences(SlotIndex, 6, 3) == 0))){ // Any one vowel
            *Gains = WinRewards[2] * Mise;
        } else if (nbOfOcurences(SlotIndex, 5, 3) == 1){ // Any one S
            *Gains = WinRewards[1] * Mise;
        }

        // Specific combinaition
        if (intcmp(SlotIndex, (int[3]){2, 4, 2}, 3)){ // LOL
            *Gains = WinRewards[7] * Mise;
        } else if (intcmp(SlotIndex, (int[3]){0, 6, 2}, 3)){ // Bal
            *Gains = WinRewards[2] * Mise;
        } else if (intcmp(SlotIndex, (int[3]){0, 1, 2}, 3)){ // Bel
            *Gains = WinRewards[0] * Mise;
        } else if (intcmp(SlotIndex, (int[3]){3, 1, 2}, 3)){ // Nel (Bleach)
            *Gains = WinRewards[3] * Mise;
        } else if (intcmp(SlotIndex, (int[3]){0, 4, 3}, 3)){ // Bon
            *Gains = WinRewards[4] * Mise;
        } else if (intcmp(SlotIndex, (int[3]){0, 6, 3}, 3)){ // Ban (Hammer)
            *Gains = WinRewards[5] * Mise;
        } else if (intcmp(SlotIndex, (int[3]){6, 0, 1}, 3)){ // Abe (Odyssey)
            *Gains = WinRewards[6] * Mise;
        } else if (intcmp(SlotIndex, (int[3]){3, 1, 4}, 3)){ // Neo (Matrix)
            *Gains = WinRewards[8] * Mise;
        } else if (intcmp(SlotIndex, (int[3]){4, 5, 5}, 3)){ // OSS 117
            *Gains = WinRewards[10] * Mise;
        } else if (intcmp(SlotIndex, (int[3]){5, 5, 5}, 3)){ // SSS
            if (Mise < 3){
                *Gains = WinRewards[11] * Mise;
            }else{
                *Gains = WinRewards[12];
                (*BJackpot) = 1;
            }
        }
    }

    //printf("Gains %d | Mise %d \n", *Gains, Mise);
}
