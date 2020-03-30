#include "include.h"
#include <SDL2/SDL.h> // Main sdl Header

#define NBL 6
#define WIN 14

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

void DisplayTab(int TabY, int TabX, char CardIndex[TabY][TabX]){ // Affiche le contenue d'un tableau char**
    for (int i = 0; i < TabY; i++){
        printf("%3d %s",i, CardIndex[i]); // récupère une ligne d'un tableau char**
    }
}

void DisplayCardAt(int TabY, int TabX, char CardIndex[TabY][TabX], int CardID, int CardYSize, int y, int x){ // Affiche une carte a une endroid précis de l'écran
    for (int i = 0; i < CardYSize; i++){
        SetCursorAt(y + i, x);
        printf("%s", CardIndex[(CardID * CardYSize + i)]); // récupère une ligne d'un tableau char**
    }
}

int main(int argc, char *argv[]){
    // Déclaration des variables principales
    int Gains = 0, Credits = 0, Mise = 0, BankIN = 0;
    char GUI = 0, ReturnStatus = 0; // Booléen de sélection (char car il n'a besoin que d'etre 0 ou 1)

    char TabDeck[NBL] = "BELNOS"; // 0 -> 5 Les lettres qui peuvent tomber
    char WinTable[WIN][4] = {"BON", "BEL", "BOL", "SOL", "SEL", "LES", "OSS", "BEN", "SEE", "NEO", "NOS", "LOL", "SON", "ONE"}; // Les combinaison gagnantes
    int WinRewards[WIN] = {1000, 500, 300, 250, 200, 150, 117, 50, 45, 40, 35, 30, 25, 1}; // Les gains associés
    char Slots[4] = "LOL"; // La combinaison par défaut

    FILE* SlotFont = NULL; // Notre fichier contenant les "Polices" a blit dans la console
    Vector2i SlotSize; SlotSize.x = 0; SlotSize.y = 0;
    int CardSize = 0;

    srand(time NULL); // Pour que rand() soit plus dificilement prédictible

    // Déclaration liée a la SDL
    SDL_Window* MainWindow;

    // Gestion des arguments
    if (argc > 1){ // Si il y a des arguments
        for (int i = 1; i < argc; i++){ // Recherche et traitement de tout les arguments (commence a 1 car l'argument #0 est le nom de l'executable)
            if (strcmp(argv[i], "-SDL") == 0){ // si l'utilisateur sélectione un gui (== 0 signifie que les deux strings n'ont aucune différence)
                GUI = 1;
            }else if (strcmp(argv[i], "-OSS") == 0){ // Easter egg
                printf("D'aucuns ont des aventures je suis une aventure.\n");
                exit(-1);
            }
        }
    }

    if (GUI){ // A effectuer seulement si l'utilisateur a choisi une gui
        // Initialisations liée a la SDL
        if (SDL_Init(SDL_INIT_VIDEO) != 0){ // initialisation de la sdl + Gestion des erreurs
            fprintf(stderr, "Erreur a l'initialisation de la SDL : %s\n", SDL_GetError()); // On affiche le message d'erreur s'il y en a un
            exit(-1);
        }

        MainWindow = SDL_CreateWindow(SLOTMACHINE_NAME, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_X,SCREEN_Y, SDL_WINDOW_SHOWN);
        if (MainWindow == NULL){ // Gestion des erreurs (Creation de la fenêtre)
            fprintf(stderr, "Erreur a la creation de la fenêtre : %s\n", SDL_GetError()); // On affiche le message d'erreur s'il y en a un
            exit(-1);
        }
    }

    if ((SlotFont = fopen(FontPath, "r")) == NULL){
        fprintf(stderr, "Erreur au chargement des cartes\n");
        exit(-1);
    }
    fscanf(SlotFont, "%d %d", &SlotSize.x, &SlotSize.y); fseek(SlotFont, 1, SEEK_CUR);
    SlotSize.x += 2; // +1 \n +1 \0
    CardSize = SlotSize.y;
    SlotSize.y *= NBL; // on a la taille d'une carte on veut toutes les cartes

    char CardIndex[SlotSize.y][SlotSize.x]; // On déclare un tableau pouvant contenir toutes les cartes
    LoadTabFromFile(SlotSize.y, SlotSize.x, CardIndex, SlotFont); // On charge nottre fichier dans notre tableau
    //DisplayTab(SlotSize.y, SlotSize.x, CardIndex); // On affiche le tableau pour vérifier que tout est bon
    
    //DisplayCardAt(SlotSize.y, SlotSize.x, CardIndex, 0, CardSize, 10, 10); // B
    //DisplayCardAt(SlotSize.y, SlotSize.x, CardIndex, 1, CardSize, 10, SlotSize.x + 10); // E
    //DisplayCardAt(SlotSize.y, SlotSize.x, CardIndex, 2, CardSize, 10, 2*SlotSize.x + 10); // L
    //exit(-1);

    SetConsoleSize(LINES, COLUMNS); // On standardise la taille de la console affin d'éviter les problèmes d'affichage

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

Shutdown:
    fclose(SlotFont);
    if (GUI){
        SDL_DestroyWindow(MainWindow); // On détruit la fenêtre
        SDL_Quit(); // On quitte la SDL avant de quitter notre programme
    }

    return ReturnStatus;
}