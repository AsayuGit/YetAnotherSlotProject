#include "include.h"

#define NBL 6
#define WIN 14

void clearInputBuffer (void) {
    while ((getchar()) != '\n');
}

int main(int argc, char *argv[]){
    // Déclaration des variables principales
    int Gains = 0, Credits = 0, Mise = 0, BankIN = 0;
    char GUI = 0; // Booléen de sélection (char car il n'a besoin que d'etre 0 ou 1)

    char TabDeck[NBL] = "BELNOS"; // 0 -> 5 Les lettres qui peuvent tomber
    char WinTable[WIN][4] = {"BON", "BEL", "BOL", "SOL", "SEL", "LES", "OSS", "BEN", "SEE", "NEO", "NOS", "LOL", "SON", "ONE"}; // Les combinaison gagnantes
    int WinRewards[WIN] = {1000, 500, 300, 250, 200, 150, 117, 50, 45, 40, 35, 30, 25, 1}; // Les gains associés
    char Slots[4] = "LOL"; // La combinaison par défaut

    srand(time NULL); // Pour que rand() soit plus dificilement prédictible

    // Gestion des arguments
    if (argc > 1){ // Si il y a des arguments
        for (int i = 1; i < argc; i++){ // Recherche et traitement de tout les arguments (commence a 1 car l'argument #0 est le nom de l'executable)
            if (strcmp(argv[i], "-SDL") == 0){ // si l'utilisateur sélectione un gui (== 0 signifie que les deux strings n'ont aucune différence)
                GUI = 1;
            }else if (strcmp(argv[i], "-OSS") == 0){ // Easter egg
                printf("D'aucuns ont des aventures je suis une aventure.\n");
                exit(0);
            }
        }
    }


    while (1){ // Main loop
        system(CLEAR); // Clear the console
        printf("Gains : %d | Credits : %d | Mise : %d | Slots : %s\n", Gains, Credits, Mise, Slots); // Header
        
        if (Credits == 0){ // BankIN (Fin de partie / Début de partie quand les crédits tombent a 0)
            printf("\nVeuiller insérer des credits pour continuer : ");
            BankIN = 0;
            while (BankIN <= 0){ // Saisie sécurisé
                while (scanf("%d", &BankIN) == 0){
                    clearInputBuffer();
                }
            }
            Credits += BankIN;
        }else { // Sloot Loop
            printf("\nVeuiller entrer la mise (1 - 3) 0 pour encaisser : ");
            Mise = -1;
            while ((Mise < 0) || (Mise > 3) || (Mise > Credits)){ // Saisie sécurisé
                while (scanf("%d", &Mise) == 0){
                    clearInputBuffer();
                }
            }
            if (Mise != 0){
                Credits -= Mise;
            }else{
                exit(0); // L'utilisateur a choisi de repartir avec ses crédits
            }
            if (Credits < 0){ // On évité de passer dans les négatifs
                Credits = 0;
            }

            // SlotMachine Logic
            
            // Génération aléatoire des slots (tirage)
            for (int i = 0; i < 3; i++){
                Slots[i] = TabDeck[rand()%NBL];
            }

            //strcpy(Slots, "LOL");
            Gains = 0;
            for (int i = 0; i < WIN; i++){ // Recherche d'une combinaison gagnante et calcul des gains
                if (strcmp(Slots, WinTable[i]) == 0){
                    Gains = WinRewards[i] * Mise;
                }
            }
            Credits += Gains;
        }
    }
    return 0;
}