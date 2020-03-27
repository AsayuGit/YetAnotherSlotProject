#include "include.h"

#define NBL 6
#define WIN 14

void clearInputBuffer (void) {
    while ((getchar()) != '\n');
}

int main(){
    int Gains = 0, Credits = 0, Mise = 0;
    int BankIN = 0;

    char TabDeck[NBL] = "BELNOS"; // 0 -> 5
    char WinTable[WIN][4] = {"BON", "BEL", "BOL", "SOL", "SEL", "LES", "OSS", "BEN", "SEE", "NEO", "NOS", "LOL", "SON", "ONE"};
    int WinRewards[WIN] = {1000, 500, 300, 250, 200, 150, 117, 50, 45, 40, 35, 30, 25, 1};
    char Slots[4] = "LOL";

    srand(time NULL);

    while (1){ // main loop
        system(CLEAR);
        printf("Gains : %d | Credits : %d | Mise : %d | Slots : %s\n", Gains, Credits, Mise, Slots); // Head
        
        if (Credits == 0){ // BankIN
            printf("\nVeuiller insérer des credits pour continuer : ");
            BankIN = 0;
            while (BankIN <= 0){
                while (scanf("%d", &BankIN) == 0){
                    clearInputBuffer();
                }
            }
            Credits += BankIN;
        }else { // Sloot Loop
            printf("\nVeuiller entrer la mise (1 - 3) 0 pour encaisser : ");
            Mise = -1;
            while ((Mise < 0) || (Mise > 3) || (Mise > Credits)){
                while (scanf("%d", &Mise) == 0){
                    clearInputBuffer();
                }
            }
            if (Mise != 0){
                Credits -= Mise;
            }else{
                exit(0);
            }
            if (Credits < 0){ // On évité de passer dans les négatifs
                Credits = 0;
            }

            // SlotMachine Logic
            
            // Génération aléatoire des slots (tirage)
            for (int i = 0; i < 3; i++){
                Slots[i] = TabDeck[rand()%NBL];
            }

            Gains = 0;
            //strcpy(Slots, "LOL");
            for (int i = 0; i < WIN; i++){
                if (strcmp(Slots, WinTable[i]) == 0){
                    Gains = WinRewards[i] * Mise;
                }
            }
            Credits += Gains;
        }
    }
    return 0;
}