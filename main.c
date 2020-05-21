#include "include.h"
#include <SDL2/SDL.h> // Main sdl Header
#include <SDL2/SDL_mixer.h> // Audio support

#define NBL 7
#define WIN 13

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

void DisplayCardAt(int TabY, int TabX, char CardIndex[TabY][TabX], int CardID, int CardYSize, int y, int x, int Delay){ // Affiche une carte a une endroid précis de l'écran
    for (int i = 0; i < CardYSize; i++){
        SetCursorAt(y + i, x);
        printf("%s", CardIndex[(CardID * CardYSize + i)]); // récupère une ligne d'un tableau char**
        milliSleep(Delay);
    }
}

SDL_Texture* loadImage(const char path[], SDL_Renderer* renderer){
    // Nottre surface temporaire pour le chargement des textures
    SDL_Surface* LoadingSurface = SDL_LoadBMP(path); // On charge le fichier en mémoire
    SDL_Texture* ReturnTexture = NULL;
    if (LoadingSurface == NULL){
        fprintf(stderr, "Erreur chargement surface : %s\n", SDL_GetError());
        exit(-1);
    }
    ReturnTexture = SDL_CreateTextureFromSurface(renderer, LoadingSurface); // On le "copie" en vram
    if (ReturnTexture == NULL){
        fprintf(stderr, "Erreur creation texture : %s\n", SDL_GetError());
        exit(-1);
    }
    SDL_FreeSurface(LoadingSurface); // On libère la mémoire

    return ReturnTexture;
}

// Ces deux fonctions peuvent sembler redondantes, mais elles permettent d'avoir a réécrire le code de gestion d'erreur
Mix_Chunk* loadSoundEffect(const char path[]){
    Mix_Chunk* loadingChunk = Mix_LoadWAV(path);
    if (loadingChunk == NULL){
        fprintf(stderr, "Erreur lors du chargement en mémoire de l'effet : %s\n%s\n", path, Mix_GetError());
        exit(-1);
    }
    return loadingChunk;
}

Mix_Music* loadMusic(const char path[]){
    Mix_Music* loadingMusic = Mix_LoadMUS(path);
    if (loadingMusic == NULL){
        fprintf(stderr, "Erreur lors du chargement en mémoire de la musique : %s\n%s\n", path, Mix_GetError());
        exit(-1);
    }
    return loadingMusic;
}

// Affiche un nombre avec des times (Algo droite gauche :3(comme quoi ça sert))
void drawNB(SDL_Renderer* renderer, SDL_Texture* digitTabTexture[], SDL_Rect* srcrect, SDL_Rect dstrect, Vector2i offset, int nbOfElements, int NB){
    dstrect.x += offset.x * (nbOfElements - 1);
    dstrect.y += offset.y * (nbOfElements);
    for (int i = 0; i < nbOfElements; i++){
        SDL_RenderCopy(renderer, digitTabTexture[NB % 10], srcrect, &dstrect); // Affichage de la mise
        dstrect.x -= offset.x;
        dstrect.y -= offset.y;
        NB /= 10;
    }
}

// Snap the slots to a certain combinaison
void snapSlots(SDL_Rect * coordinates,int originOffset, int stepOffset, int newID){
    (*coordinates).y = stepOffset * newID + originOffset;
}

// Animates the slots smoothly
void animateSlots(SDL_Rect * coordinates,int originOffset, int stepOffset, int newID, float maxSpeed, int *Steps){  
    (*coordinates).y += maxSpeed;
    int loopbackLimit;
    
    if ((*Steps) > 0){// LoopCode
        loopbackLimit = stepOffset * NBL + originOffset;
        if ((*coordinates).y > loopbackLimit){
            (*coordinates).y = (*coordinates).y - loopbackLimit + originOffset;
            (*Steps)--;
        }
    }else{
        loopbackLimit = stepOffset * newID + originOffset; // Arriver précisément au bon endroit
        if ((*coordinates).y > loopbackLimit){
            (*coordinates).y = stepOffset * newID + originOffset;
            (*Steps)--;
        }
        //(*coordinates).y = stepOffset * newID + originOffset;
    }

    //printf("%d\n", (*coordinates).y);
}

void tirage(int * Gains, int Mise, char TabDeck[], int SlotIndex[], int WinRewards[], char Mode){
    // Génération aléatoire des slots (tirage)
    for (int i = 0; i < 3; i++){
        SlotIndex[i] = rand()%NBL; // int
    }

    // Recherche de la valeur du gain
    *Gains = 0;
    // Luck manipulation (for debug purposes)
    /*
    SlotIndex[0] = 2;
    SlotIndex[1] = 6;
    SlotIndex[2] = 2;*/

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
            }
        }
    }else{ // Texte mode
        // "Any" combination
        if ((SlotIndex[0] == SlotIndex[1]) && (SlotIndex[0] == SlotIndex[2])){ // Any same
            *Gains = WinRewards[9] * Mise;
        } else if (((nbOfOcurences(SlotIndex, 6, 3) == 1) && (nbOfOcurences(SlotIndex, 1, 3) == 0) && (nbOfOcurences(SlotIndex, 4, 3) == 0)) || ((nbOfOcurences(SlotIndex, 1, 3) == 1) && (nbOfOcurences(SlotIndex, 6, 3) == 0) && (nbOfOcurences(SlotIndex, 4, 3) == 0)) || ((nbOfOcurences(SlotIndex, 4, 3) == 1) && (nbOfOcurences(SlotIndex, 1, 3) == 0) && (nbOfOcurences(SlotIndex, 6, 3) == 0))){ // Any one vowel
            *Gains = WinRewards[1] * Mise;
        } // 6 1 4 
        
        // Half specific combination
        if (intUni(SlotIndex, (int[3]){3, 1, 0}, 3)){ // Any mixed NEB
            *Gains = WinRewards[6] * Mise;
        } else if (nbOfOcurences(SlotIndex, 5, 3) == 2){ // Any two S
            *Gains = WinRewards[7] * Mise;
        } else if (nbOfOcurences(SlotIndex, 5, 3) == 1){ // Any one S
            *Gains = WinRewards[4] * Mise;
        }

        // Specific combinaition
        if (intcmp(SlotIndex, (int[3]){2, 4, 2}, 3)){ // LOL
            *Gains = WinRewards[1] * Mise;
        } else if (intcmp(SlotIndex, (int[3]){0, 6, 2}, 3)){ // Bal
            *Gains = WinRewards[2] * Mise;
        } else if (intcmp(SlotIndex, (int[3]){0, 1, 2}, 3)){ // Bel
            *Gains = WinRewards[2] * Mise;
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
            }
        }
    }

    //printf("Gains %d | Mise %d \n", *Gains, Mise);
}

void Scale(int *A, int *B, int C){ // Scale to a new resolution
    // A = Old Horizontal
    // B = Old Vertical
    // C = New Horizontal
    (*B) = (int)(((float)C/(float)(*A))*(float)(*B));
    *A = C;
}

void ScalePercent(int *A, int Percent){ // Scale by X %
    *A *= (float)(Percent / 100.f);
}

void ScaleTextureToLinkedPercent(SDL_Rect *Dimensions, int LinkedRes, float Percent){
    Scale(&(*Dimensions).w, &(*Dimensions).h, ((LinkedRes / 100.0f) * Percent));
}

int main(int argc, char *argv[]){
    // Déclaration des variables principales
    int Gains = 0, GuiGains = 0, Credits = 0, Mise = 0, MaxMise = 3, BankIN = 0, LastMise = 0;
    char GUI = 1, ReturnStatus = 0, TextInput = 1; // Booléen de sélection (char car il n'a besoin que d'etre 0 ou 1)

    char TabDeck[NBL] = "BELNOSI"; // 0 -> 5 Les lettres qui peuvent tomber
    int WinRewards[WIN] = {1, 2, 5, 10, 20, 40, 50, 100, 200, 300, 400, 4000, 20000}; // Les gains associés
    int SlotIndex[3] = {2, 4, 2}; // Index de la combinaison par défaut

    FILE* SlotFont = NULL; // Notre fichier contenant les "Polices" a blit dans la console
    Vector2i SlotSize; SlotSize.x = 0; SlotSize.y = 0;
    int CardSize = 0;
    int TextCardDrawDelay = 0; // Contiendras le délais ligne a ligne de l'affichage d'une carte quand une combinaison est tiré
    float ReelSpeed = 200.0f; // Contient la vitesse de rotation des rouleaux
    int ReelStep[3] = {-2, -2, -2}; // Contient le nombre de tours a effectuer // -2 signifie pas d'animation
    int ReelSize = 0; // Taille verticale de la texture des slots (Utilisé pour leurs animation)

    srand(time NULL); // Pour que rand() soit plus dificilement prédictible

    unsigned int time = 0, oldTime = 0; // contient la valeur du temps courant et de la frame d'avant
    char creditBlink = 0; // Booléen indiquant si le compteur des crédits clignote ou pas (saisie des crédits au clavier en mode gui)
    int creditBlinkTimer = 0, creditBlinkDelay = 500; // En milisecondes

    char selectedTheme = 0; // Le thème qu'utiliseras le programme : 0 = NEO (chiffres), 1 = CASINO (ExcluSDL) / DICE (Terminal)
    char rewardMode = 0; // 0 mode adapté pour du texte, 1 mode plus complexe adapté pour le thème casino

    // Déclaration liée a la SDL
    char Fullscreen = 1;
    SDL_Window* MainWindow; // Fenêtre principale
    SDL_Renderer* Renderer; // Structure nous permettant de dessinner dans la fenêtre
    SDL_DisplayMode DesktopDisplayMode; // Contient les propriétés de l'écran

    SDL_Texture* Faceplate; // Le dash
    SDL_Texture* NeoPlate; // --- Neo
    SDL_Texture* CasinoPlate; // --- Casino
    SDL_Texture* Digits[10]; // tableau contenant les numéro
    SDL_Texture* Buttons; // Les différents boutons et leurs états
    SDL_Texture* Reel; // Les rouleaux de cartes / slots
    SDL_Texture* NeoReel; // --- Neo
    SDL_Texture* casinoReel; // --- Casino
    SDL_Texture* Shadow; // Ombre projeté sur les slots
    SDL_Texture* Sign; // Liste des combinaisons
    SDL_Texture* NeoSign; // --- Neo
    SDL_Texture* casinoSign; // --- Casino
    SDL_Texture* BackGround; // L'arrière plan
    SDL_Texture* neoBG; // Arrière plan du thème NEO
    SDL_Texture* casinoBG; // Arrière plan du thème Casino

    SDL_Rect Faceplate_DIM = {0}, Digits_DIM = {0}, Gains_DIM = {0}, Credits_DIM = {0}, Mise_DIM = {0}, Buttons_DIM = {0}, BMiser1 = {0}, BMiserMax = {0}, BJouer = {0};
    SDL_Rect Reel1 = {0}, Reel2 = {0}, Reel3 = {0}; // Coordonées pour les 3 slots (Dans la texture)
    SDL_Rect Reel1_DIM = {0}, Reel2_DIM = {0}, Reel3_DIM = {0}; // Dimensions pour les 3 slots (A l'écran)
    SDL_Rect SignDIM = {0};
    Vector2i ReelOffset; ReelOffset.y = 210; ReelOffset.x = 448;
    Vector2i Digits_OFFSETS;
    //      Position par défaut (Démarage) / Offset case a case

    // Sound effects
    Mix_Chunk *coinIn = NULL;
    Mix_Chunk *coinIn2 = NULL;
    Mix_Chunk *coinIn3 = NULL;
    Mix_Chunk *spin = NULL;
    Mix_Music *backgroundMusic = NULL; // Musique de fond
    Mix_Music *neoBGM = NULL; // BGM 80s
    Mix_Music *casinoBGM = NULL; // Classic casino BGM

    SDL_Event event; // Structure contenant tous les événements relatif a la fenêtre (souris clavier menus etc)
    SDL_Point MousePosition;

    // Gestion des arguments
    if (argc > 1){ // Si il y a des arguments
        for (int i = 1; i < argc; i++){ // Recherche et traitement de tout les arguments (commence a 1 car l'argument #0 est le nom de l'executable)
            if (strcmp(argv[i], "-SDL") == 0){ // si l'utilisateur sélectione un gui (== 0 signifie que les deux strings n'ont aucune différence)
                GUI = 1;
            } else if (strcmp(argv[i], "-Term") == 0){
                GUI = 0;
                rewardMode = 0;
            } else if (strcmp(argv[i], "-w") == 0){
                Fullscreen = 0;
            } else if (strcmp(argv[i], "-t") == 0){ // Theme selector
                if (argc > i+1){ // s'il y a un argument après -t
                    selectedTheme = atoi(argv[++i]);
                    if (selectedTheme < 0){
                        selectedTheme = 0;
                    }else if (selectedTheme > NB_OF_THEMES){
                        selectedTheme = NB_OF_THEMES;
                    }
                }
            } else if (strcmp(argv[i], "-r") == 0){ // Resolution selector
                if (argc > i+1){ // s'il y a un argument après -r
                    char Res = atoi(argv[++i]);
                    if ((Res < 0) || (Res > 5) || (argv[i][0] - 48 < 0) || (argv[i][0] - 48 > 5)){ // si arguments invalide
                        Res = 3;
                        i--;
                    }
                    switch (Res){
                        case 0:
                            WindowRES.x = 3840;
                            WindowRES.y = 2160;
                            break;
                        case 1:
                            WindowRES.x = 2560;
                            WindowRES.y = 1440;
                            break;
                        case 2:
                            WindowRES.x = 1920;
                            WindowRES.y = 1080;
                            break;
                        case 3:
                            WindowRES.x = 1600;
                            WindowRES.y = 900;
                            break;
                        case 4:
                            WindowRES.x = 1280;
                            WindowRES.y = 720;
                            break;
                        case 5:
                            WindowRES.x = 640;
                            WindowRES.y = 360;
                            break;
                    }
                }
            } else if (strcmp(argv[i], "-cr") == 0){
                if (argc > i+2){ // s'il y a deux arguments après -cr
                    WindowRES.x = atoi(argv[++i]);
                    WindowRES.y = atoi(argv[++i]);
                    if (WindowRES.x < 0){
                        WindowRES.x = 1600;
                    }
                    if (WindowRES.y < 0){
                        WindowRES.y = 900;
                    }
                }
            } else if (strcmp(argv[i], "-OSS") == 0){ // Easter egg
                printf("D'aucuns ont des aventures je suis une aventure.\n");
                exit(-1);
            } else if ((strcmp(argv[i], "--help") == 0) || (strcmp(argv[i], "-h") == 0)){
                printf("YetAnotherSlotProject est un jeu de machine a sous donné en projet de 1A-S2 ESIEA\n"
                        "Le jeu se lance par défaut en utilisant la SDL, pour obtenir la version terminal Utiliser l'option -Term\n\n"
                        "Liste des options :\n"
                        "-SDL  : Lance le jeu en mode SDL\n"
                        "-Term : Lance le jeu en mode Terminal\n"
                        "-t    : Selectionne le thème a utiliser\n"
                        "           0 = NEON (Lettres)\n"
                        "           1 = Casino (ExcluSDL) / Dice (ExcluTerminal)\n"
                        "-w    : Lance le jeu en mode fenétré\n"
                        "-r    : Change la résolution en mode fenétré entre certains préset:\n"
                        "           0 = 3840*2160 UHD (16/9)\n"
                        "           1 = 2560*1440 QHD(16/9)\n"
                        "           2 = 1920*1080 FullHD (16/9)\n"
                        "           3 = 1600*900 (16/9) [Default]\n"
                        "           4 = 1280*720 HD (16/9)\n"
                        "           5 = 640*360 SD(16/9)\n"
                        "-cr   : Change la résolution en mode fenétré par une résolution au choix\n"
                        "           (Gardez en tête que ce jeu a été conçu pour un affichage en 16/9     )\n"
                        "           (et qu'un ratio d'aspect différent peut causer des disfonctionnements)\n\n"
                        "-h ou --help : Affiche ce menu d'aide\n\n"
                        "Credits : RAIMBAUD Killian & TOUGARD Enzo / 2020\n");
                exit(0);
            }
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

    SetConsoleSize(LINES, COLUMNS); // On standardise la taille de la console affin d'éviter les problèmes d'affichage

    if (GUI){ // A effectuer seulement si l'utilisateur a choisi une gui
        // Initialisations liée a la SDL
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0){ // initialisation de la sdl + Gestion des erreurs
            fprintf(stderr, "Erreur a l'initialisation de la SDL : %s\n", SDL_GetError()); // On affiche le message d'erreur s'il y en a un
            exit(-1);
        }

        if(SDL_GetDesktopDisplayMode(0, &DesktopDisplayMode) != 0){
            fprintf(stderr, "Can't get the desktop display mode %s\n", SDL_GetError());
            exit(-1);
        }
        FullscreenRES.x = DesktopDisplayMode.w;
        FullscreenRES.y = DesktopDisplayMode.h;

        MainWindow = SDL_CreateWindow(SLOTMACHINE_NAME, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WindowRES.x,WindowRES.y, SDL_WINDOW_SHOWN);
        if (MainWindow == NULL){ // Gestion des erreurs (Creation de la fenêtre)
            fprintf(stderr, "Erreur a la creation de la fenêtre : %s\n", SDL_GetError()); // On affiche le message d'erreur s'il y en a un
            exit(-1);
        }

        Renderer = SDL_CreateRenderer(MainWindow, -1, 0); // -1, 0 Essaye d'utiliser l'accélération matérielle sinon revient au rendu software
        if (Renderer == NULL){
            fprintf(stderr, "Erreur a la creation du renderer : %s\n", SDL_GetError());
            exit(-1);
        }

        SDL_GL_SetSwapInterval(1); // Turn on vsync

        if (Fullscreen){
            SDL_SetWindowFullscreen(MainWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
            ScreenRES.x = FullscreenRES.x;
            ScreenRES.y = FullscreenRES.y;
        }else{
            SDL_SetWindowFullscreen(MainWindow, 0);
            ScreenRES.x = WindowRES.x;
            ScreenRES.y = WindowRES.y;
        }

        // Initialisation du moteur audio en qualité CD 44100Khz, Stereo, 1kb par chunk
        if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) != 0){
            fprintf(stderr, "Erreur a l'inintialisation du moteur audio : %s\n", Mix_GetError());
            exit(-1);
        }

        neoBG = loadImage(ImagePath"NeoBackGround.bmp", Renderer);
        casinoBG = loadImage(ImagePath"casinoBG1.bmp", Renderer); // Temporary
        NeoSign = loadImage(ImagePath"neoSign2.bmp", Renderer);
        casinoSign = loadImage(ImagePath"casinoSign.bmp", Renderer);
        NeoPlate = loadImage(ImagePath"NeoPlate.bmp", Renderer);
        CasinoPlate = loadImage(ImagePath"casinoPlate.bmp", Renderer);

        for (int i = 0; i < 10; i++){
            char filename[11 + sizeof(ImagePath)]; // On aloue un buffer pour contenir le nom du fichier a charger
            sprintf(filename,ImagePath"digit%d.bmp", i);
            Digits[i] = loadImage(filename, Renderer);
        }

        Buttons = loadImage(ImagePath"buttons3.bmp", Renderer);
        NeoReel = loadImage(ImagePath"reelNEO.bmp", Renderer);
        casinoReel = loadImage(ImagePath"reel.bmp", Renderer);
        Shadow = loadImage(ImagePath"shadow.bmp", Renderer);

        // Sound effects
        coinIn = loadSoundEffect(SoundPath"payout1.wav");
        coinIn2 = loadSoundEffect(SoundPath"payout2.wav");
        coinIn3 = loadSoundEffect(SoundPath"payout3.wav");
        spin = loadSoundEffect(SoundPath"spin.wav");

        // Musics
        casinoBGM = loadMusic(SoundPath"Halos of Eternity.ogg");
        neoBGM = loadMusic(SoundPath"tokyo-rose.wav");
themeini:
        // Theme specific initialisation
        switch (selectedTheme){
            case 0: // NEO
                BackGround = neoBG;
                Sign = NeoSign;
                Faceplate = NeoPlate;
                Reel = NeoReel;
                backgroundMusic = neoBGM;
                rewardMode = 0;
                break;
            case 1: // CASINO
                BackGround = casinoBG;
                Sign = casinoSign;
                Faceplate = CasinoPlate;
                Reel = casinoReel;
                backgroundMusic = casinoBGM;
                rewardMode = 1;
                break;
        }

        Mix_PlayMusic(backgroundMusic, -1);
        Mix_VolumeMusic(64);

scaleini:
        SDL_QueryTexture(Sign, NULL, NULL, &SignDIM.w, &SignDIM.h);
        ScaleTextureToLinkedPercent(&SignDIM, ScreenRES.x, 30);
        SignDIM.x = ScreenRES.x - SignDIM.w;
        SignDIM.y = (ScreenRES.y - SignDIM.h) / 4;

        SDL_QueryTexture(Faceplate, NULL, NULL, &Faceplate_DIM.w, &Faceplate_DIM.h); // on récupère la taille de la texture
        Scale(&Faceplate_DIM.h, &Faceplate_DIM.w, ScreenRES.y);

        SDL_QueryTexture(Digits[0], NULL, NULL, &Digits_DIM.w, &Digits_DIM.h);
        Gains_DIM = (SDL_Rect){Faceplate_DIM.x + (ScreenRES.x * 0.123f), Faceplate_DIM.y + (ScreenRES.y * 0.702f), Digits_DIM.w, Digits_DIM.h};
        Credits_DIM = (SDL_Rect){Faceplate_DIM.x + (ScreenRES.x * 0.285f), Faceplate_DIM.y + (ScreenRES.y * 0.702f), Digits_DIM.w, Digits_DIM.h};
        Mise_DIM = (SDL_Rect){Faceplate_DIM.x + (ScreenRES.x * 0.565f), Faceplate_DIM.y + (ScreenRES.y * 0.702f), Digits_DIM.w, Digits_DIM.h};
        ScaleTextureToLinkedPercent(&Gains_DIM, ScreenRES.x, 2.15f);
        ScaleTextureToLinkedPercent(&Credits_DIM, ScreenRES.x, 2.15f);
        ScaleTextureToLinkedPercent(&Mise_DIM, ScreenRES.x, 2.15f);
        Digits_OFFSETS = (Vector2i){(ScreenRES.x * 0.022f), 0};

        Buttons_DIM.w = 192; Buttons_DIM.h = 125; // Vu que la texture contient tout les boutons on renseignes leur tailles manuellement
        // On initialise la position (et par conséquant leurs hitboxes) des différents boutons de l'interface
        BMiser1 = (SDL_Rect){Faceplate_DIM.x + (ScreenRES.x * 0.51f), Faceplate_DIM.y + (ScreenRES.y * 0.79f), Buttons_DIM.w, Buttons_DIM.h};
        BMiserMax = (SDL_Rect){Faceplate_DIM.x + (ScreenRES.x * 0.59f), Faceplate_DIM.y + (ScreenRES.y * 0.79f), Buttons_DIM.w, Buttons_DIM.h};
        BJouer = (SDL_Rect){Faceplate_DIM.x + (ScreenRES.x * 0.14f), Faceplate_DIM.y + (ScreenRES.y * 0.79f), Buttons_DIM.w, Buttons_DIM.h};
        ScaleTextureToLinkedPercent(&BMiser1, ScreenRES.x, 6);
        ScaleTextureToLinkedPercent(&BMiserMax, ScreenRES.x, 6);
        ScaleTextureToLinkedPercent(&BJouer, ScreenRES.x, 6);

        SDL_QueryTexture(Reel, NULL, NULL, &Reel1.w, &ReelSize); // On récupère seulement l'épaisseur de la texture
        Reel1_DIM.w = Reel3.w = Reel2.w = Reel1.w; // On définit les dimensions des trois rouleaux
        Reel1_DIM.h = Reel3.h = Reel2.h = Reel1.h = Reel1.w * 1.5f;
        Reel3.y = Reel2.y = Reel1.y = ReelOffset.y; // On déffini la position par défaut (offset) des rouleaux

        ScaleTextureToLinkedPercent(&Reel1_DIM, ScreenRES.x, 15.5f);
        Reel1_DIM = (SDL_Rect){(ScreenRES.x * 0.120f), (ScreenRES.y * 0.215f), Reel1_DIM.w, Reel1_DIM.h};
        Reel2_DIM = (SDL_Rect){((Faceplate_DIM.w - Reel1_DIM.w) >> 1) , (ScreenRES.y * 0.215f), Reel1_DIM.w, Reel1_DIM.h};
        Reel3_DIM = (SDL_Rect){(ScreenRES.x * 0.482f), (ScreenRES.y * 0.215f), Reel1_DIM.w, Reel1_DIM.h};


        // Snaps the slots into place for the default combination
        snapSlots(&Reel1, ReelOffset.y, ReelOffset.x, SlotIndex[0]);
        snapSlots(&Reel2, ReelOffset.y, ReelOffset.x, SlotIndex[1]);
        snapSlots(&Reel3, ReelOffset.y, ReelOffset.x, SlotIndex[2]);

        //SDL_StartTextInput(); // On active l'entré texte par défaut car la machine a sou ne contient pas de crédits au démarrage
    }

    while (1){ // Main loop

        if (!GUI){ // If in console mode
            system(CLEAR); // Clear the console

            // On Affiche les cartes du précédent tirage
            DisplayCardAt(SlotSize.y, SlotSize.x, CardIndex, SlotIndex[0], CardSize, 2, 10, TextCardDrawDelay); // Card 1
            DisplayCardAt(SlotSize.y, SlotSize.x, CardIndex, SlotIndex[1], CardSize, 2, SlotSize.x + 10, TextCardDrawDelay); // Card 2
            DisplayCardAt(SlotSize.y, SlotSize.x, CardIndex, SlotIndex[2], CardSize, 2, 2*SlotSize.x + 10, TextCardDrawDelay); // Card 3

            TextCardDrawDelay = 0;

            SetCursorAt(CardSize + 4, 25);
            printf("Gains : %d | Credits : %d | Mise : %d\n", Gains, Credits, Mise); // Header
            
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
                if (Credits < 3){ // On calcul la mise maximum affin d'éviter que l'utilisateur mise plus que ce qu'il a crédité
                    MaxMise = Credits;
                }else {
                    MaxMise = 3;
                }
                printf("\nVeuiller entrer la mise (1 - %d) 0 pour encaisser : ", MaxMise);
                Mise = -1;
                while ((Mise < 0) || (Mise > MaxMise)){ // Saisie sécurisé
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
                tirage(&Gains, Mise, TabDeck, SlotIndex, WinRewards, rewardMode); // tirage des combinaisons
                TextCardDrawDelay = 50; // On délais l'affichage des cartes au tirage affin d'avoir un semblant d'annimation
                Credits += Gains;
            }
        } else { // if in GUI MODE
            
            // Permet de savoir depuis combien de temps le program s'execute ainsi que le /\ d'une frame
            oldTime = time;
            time = SDL_GetTicks();

            // Fait clignoter les crédits pendant la saisie
            if (TextInput){
                // Logique du clignotement des crédits en mode saisie
                creditBlinkTimer += time - oldTime;
                if (creditBlinkTimer > creditBlinkDelay){
                    if(creditBlink){
                        creditBlink = 0;
                    }else{
                        creditBlink = 1;
                    }
                    creditBlinkTimer = 0;
                }
            }

            
            // Gestion des événements
            while(SDL_PollEvent(&event)){ // Tant qu'il y a des événements a traiter
                switch (event.type)
                {
                case SDL_QUIT: // Si la fenètre reçois l'ordre de quitter (croix rouge, ctrl+c)
                    goto Shutdown; // On éteins proprement la sdl puis le programme
                    break;
                case SDL_MOUSEMOTION: // Capture des mouvements de la souris
                    MousePosition.x = event.motion.x;
                    MousePosition.y = event.motion.y;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if (!TextInput && (ReelStep[0] == -3) && (ReelStep[1] == -3) && (ReelStep[2] == -3)){
                        // On check si l'utilisateur a appuyé sur un des boutons
                        if (SDL_PointInRect(&MousePosition, &BJouer)){
PLAY:
                            if (((Mise > 0) || (LastMise > 0))){
                                if ((Mise == 0) && (Credits >= LastMise)){
                                    Credits -= LastMise;
                                    Mise = LastMise;
                                }else{
                                    LastMise = Mise;
                                }
                                tirage(&GuiGains, Mise, TabDeck, SlotIndex, WinRewards, rewardMode); // tirage des combinaisons
                                ReelStep[0] = 5; ReelStep[1] = 7; ReelStep[2] = 9; // On anime les rouleaux
                                Mix_PlayChannel(-1, spin, 0); // On joue le son du tirage -1 pour laisser la sdl choisir le channel
                                Mise = 0;
                                Gains = 0;
                            }
                        }else if (SDL_PointInRect(&MousePosition, &BMiser1)){
P1MISE:
                            if ((Mise < 3) && (Credits > 0)){
                                Mise++;
                                Credits--;
                                Mix_PlayChannel(-1, coinIn, 0);
                            }
                        }else if (SDL_PointInRect(&MousePosition, &BMiserMax)){
                            if ((Mise != 3) && (Credits > 0)){
                                if (Credits < 3){
                                    Mise = Credits;
                                    if (Credits == 2){
                                        Mix_PlayChannel(-1, coinIn2, 0);
                                    }else{
                                        Mix_PlayChannel(-1, coinIn, 0);
                                    }
                                    Credits = 0;
                                }else {
                                    Mise = 3 - Mise;
                                    Credits -= Mise;
                                    switch (Mise)
                                    {
                                    case 3:
                                        Mix_PlayChannel(-1, coinIn3, 0);
                                        break;
                                    case 2:
                                        Mix_PlayChannel(-1, coinIn2, 0);
                                        break;
                                    case 1:
                                        Mix_PlayChannel(-1, coinIn, 0);
                                        break;
                                    }
                                    Mise = 3;
                                }
                            }
                        }
                    }
                    break;
                case SDL_KEYDOWN:
                    if ((event.key.keysym.scancode == SDL_SCANCODE_RETURN) || (event.key.keysym.scancode == SDL_SCANCODE_KP_ENTER)){
                        if (Credits != 0){
                            TextInput = 0;
                            creditBlink = 0;
                            SDL_StopTextInput();
                        }
                    } 
                    switch (event.key.keysym.scancode)
                    {
                    case SDL_SCANCODE_ESCAPE: // Shutdown the game properly
                        goto Shutdown;
                        break;
                    case SDL_SCANCODE_BACKSPACE: // The bankrupt key
                        if (TextInput){
                            Credits /= 10;
                        }else{
                            Credits = 0;
                            TextInput = 1;
                            SDL_StartTextInput();
                        }
                        break;
                    case SDL_SCANCODE_LEFT:
                        selectedTheme--;
                        if (selectedTheme < 0){selectedTheme = NB_OF_THEMES;};
                        goto themeini;
                        break;
                    case SDL_SCANCODE_RIGHT:
                        selectedTheme++;
                        if (selectedTheme > NB_OF_THEMES){selectedTheme = 0;}
                        goto themeini;
                        break;
                    case SDL_SCANCODE_SPACE:
                        if (!TextInput && (ReelStep[0] == -3) && (ReelStep[1] == -3) && (ReelStep[2] == -3)){
                            goto PLAY;
                        }
                        break;
                    case SDL_SCANCODE_KP_PLUS:
                        if (!TextInput && (ReelStep[0] == -3) && (ReelStep[1] == -3) && (ReelStep[2] == -3)){
                            goto P1MISE;
                        }
                        break;
                    case SDL_SCANCODE_F:
                        if (Fullscreen){
                            SDL_SetWindowFullscreen(MainWindow, 0);
                            Fullscreen = 0;
                            ScreenRES.x = WindowRES.x;
                            ScreenRES.y = WindowRES.y;
                            goto scaleini;
                        }else{
                            SDL_SetWindowFullscreen(MainWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
                            Fullscreen = 1;
                            ScreenRES.x = FullscreenRES.x;
                            ScreenRES.y = FullscreenRES.y;
                            goto scaleini;
                        }
                        break;
                    default:
                        break;
                    }
                    break;
                case SDL_TEXTINPUT:             
                    if ((event.text.text[0] >= '0') && (event.text.text[0] <= '9')){
                        int i = 0;
                        int Cred = Credits;
                        while (Cred != 0){
                            i++;
                            Cred /= 10;
                        }
                        if (i < 8){
                            Credits *= 10;
                            Credits += (event.text.text[0] - 48);
                        }
                    }
                    break;
                default:
                    break;
                }
            }

            // Affichage des éléments (Back to Front)
            SDL_RenderCopy(Renderer, BackGround, NULL,  &(SDL_Rect){0, 0, ScreenRES.x, ScreenRES.y}); // On affiche le background

            // On affiche les slots
            if (ReelStep[0] > -2){
                animateSlots(&Reel1, ReelOffset.y, ReelOffset.x, SlotIndex[0], ReelSpeed, &ReelStep[0]);
            }
            if (ReelStep[1] > -2){
                animateSlots(&Reel2, ReelOffset.y, ReelOffset.x, SlotIndex[1], ReelSpeed, &ReelStep[1]);
            }
            if (ReelStep[2] > -2){
                animateSlots(&Reel3, ReelOffset.y, ReelOffset.x, SlotIndex[2], ReelSpeed, &ReelStep[2]);
            }

            if ((ReelStep[0] == -2) && (ReelStep[1] == -2) && (ReelStep[2] == -2)){
                if (Mix_Playing(-1) != 0){
                    Mix_HaltChannel(-1);
                }
                ReelStep[0] = ReelStep[1] = ReelStep[2] = -3;
                Gains = GuiGains;
                Credits += GuiGains;

                if (Credits == 0){
                    TextInput = 1;
                    SDL_StartTextInput();
                }
            }

            SDL_RenderCopy(Renderer, Reel, &Reel1, &Reel1_DIM);
            SDL_RenderCopy(Renderer, Shadow, NULL, &Reel1_DIM);

            SDL_RenderCopy(Renderer, Reel, &Reel2, &Reel2_DIM);
            SDL_RenderCopy(Renderer, Shadow, NULL, &Reel2_DIM);

            SDL_RenderCopy(Renderer, Reel, &Reel3, &Reel3_DIM);
            SDL_RenderCopy(Renderer, Shadow, NULL, &Reel3_DIM);

            SDL_RenderCopy(Renderer, Faceplate, NULL, &Faceplate_DIM); // Affichage faceplate
            SDL_RenderCopy(Renderer, Digits[Mise], NULL, &Mise_DIM); // Affichage de la mise
            drawNB(Renderer, Digits, NULL, Gains_DIM, Digits_OFFSETS, 5, Gains); // Affichage des gains
            if (!creditBlink){
                drawNB(Renderer, Digits, NULL, Credits_DIM, Digits_OFFSETS, 8, Credits); // Affichage du nombre de crédits
            }
            // On décompose les rect affin d'avoir un plus grand control sur leurs valeurs
            SDL_RenderCopy(Renderer, Buttons, &(SDL_Rect){Buttons_DIM.w * 0, Buttons_DIM.h * 1, Buttons_DIM.w, Buttons_DIM.h}, &BMiser1); // Miser 1
            SDL_RenderCopy(Renderer, Buttons, &(SDL_Rect){Buttons_DIM.w * 1, Buttons_DIM.h * 1, Buttons_DIM.w, Buttons_DIM.h}, &BMiserMax); // Miser Max
            SDL_RenderCopy(Renderer, Buttons, &(SDL_Rect){Buttons_DIM.w * 2, Buttons_DIM.h * 1, Buttons_DIM.w, Buttons_DIM.h}, &BJouer); // Jouer

            SDL_RenderCopy(Renderer, Sign, NULL, &SignDIM);

            SDL_RenderPresent(Renderer); // on termine le rendu et l'affiche a l'écran
        }
    }

Shutdown:
    fclose(SlotFont);
    if (GUI){
        
        // On libère les texture du programme
        SDL_DestroyTexture(NeoPlate);
        SDL_DestroyTexture(Buttons);
        SDL_DestroyTexture(NeoReel);
        SDL_DestroyTexture(casinoReel);
        SDL_DestroyTexture(Shadow);
        SDL_DestroyTexture(NeoSign);
        SDL_DestroyTexture(neoBG);
        SDL_DestroyTexture(casinoBG);

        for (int i = 0; i < 10; i++){
            SDL_DestroyTexture(Digits[i]);
        }

        // On libère les éffets sonores et la musique
        Mix_FreeChunk(coinIn);
        Mix_FreeChunk(coinIn2);
        Mix_FreeChunk(coinIn3);
        Mix_FreeChunk(spin);
        Mix_FreeMusic(neoBGM);
        Mix_FreeMusic(casinoBGM);

        Mix_Quit(); // On quitte le moteur audio 
        SDL_DestroyRenderer(Renderer); // On détruit le renderer
        SDL_DestroyWindow(MainWindow); // On détruit la fenêtre
        SDL_Quit(); // On quitte la SDL avant de quitter notre programme
    }

    return ReturnStatus;
}