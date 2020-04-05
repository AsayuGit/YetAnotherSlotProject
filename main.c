#include "include.h"
#include <SDL2/SDL.h> // Main sdl Header

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

// fonctionne de la même façon que strcmp mais avec des int
int intcmp(int tab1[], int tab2[], int arraySize){
    for (int i = 0; i < arraySize; i++){
        if (tab1[i] != tab2[i]){
            return 0;
        }
    }
    return 1;
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

// Affiche un nombre avec des times (Algo droite gauche :3(comme quoi ça sert))
void drawNB(SDL_Renderer* renderer, SDL_Texture* digitTabTexture[], SDL_Rect* srcrect, SDL_Rect* dstrect, Vector2i offset, int nbOfElements, int NB){
    (*dstrect).x += offset.x * (nbOfElements - 1);
    (*dstrect).y += offset.y * (nbOfElements);
    for (int i = 0; i < nbOfElements; i++){
        SDL_RenderCopy(renderer, digitTabTexture[NB % 10], srcrect, dstrect); // Affichage de la mise
        (*dstrect).x -= offset.x;
        (*dstrect).y -= offset.y;
        NB /= 10;
    }
}

// Will animates the slots smoothly
void animateSlots(SDL_Rect * coordinates,int originOffset, int stepOffset, int newID){
    (*coordinates).y = stepOffset * newID + originOffset;
}

void tirage(int * Gains, int Mise, char TabDeck[], int SlotIndex[], int WinRewards[]){
    // Génération aléatoire des slots (tirage)
    for (int i = 0; i < 3; i++){
        SlotIndex[i] = rand()%NBL; // int
    }

    // Recherche de la valeur du gain
    *Gains = 0;
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
    } else if (intcmp(SlotIndex, (int[3]){0, 0, 0}, 3)){ // Full red sevens{
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

    //printf("Gains %d | Mise %d \n", *Gains, Mise);
}

int main(int argc, char *argv[]){
    // Déclaration des variables principales
    int Gains = 0, Credits = 0, Mise = 0, MaxMise = 3, BankIN = 0, LastMise = 0;
    char GUI = 0, ReturnStatus = 0, TextInput = 1; // Booléen de sélection (char car il n'a besoin que d'etre 0 ou 1)

    char TabDeck[NBL] = "BELNOSI"; // 0 -> 5 Les lettres qui peuvent tomber
    int WinRewards[WIN] = {1, 2, 5, 10, 20, 40, 50, 100, 200, 300, 400, 4000, 20000}; // Les gains associés
    int SlotIndex[3] = {2, 4, 2}; // Index de la combinaison par défaut

    FILE* SlotFont = NULL; // Notre fichier contenant les "Polices" a blit dans la console
    Vector2i SlotSize; SlotSize.x = 0; SlotSize.y = 0;
    int CardSize = 0;

    srand(time NULL); // Pour que rand() soit plus dificilement prédictible

    unsigned int time = 0, oldTime = 0; // contient la valeur du temps courant et de la frame d'avant
    char creditBlink = 0; // Booléen indiquant si le compteur des crédits clignote ou pas (saisie des crédits au clavier en mode gui)
    int creditBlinkTimer = 0, creditBlinkDelay = 500; // En milisecondes

    // Déclaration liée a la SDL
    SDL_Window* MainWindow; // Fenêtre principale
    SDL_Renderer* Renderer; // Structure nous permettant de dessinner dans la fenêtre

    SDL_Texture* Faceplate; // Le dash
    SDL_Texture* Digits[10]; // tableau contenant les numéro
    SDL_Texture* Buttons; // Les différents boutons et leurs états
    SDL_Texture* Reel; // Les rouleaux de cartes / slots
    SDL_Texture* Shadow; // Ombre projeté sur les slots

    SDL_Rect Faceplate_DIM = {0}, Digits_DIM = {0}, Buttons_DIM = {0}, BMiser1 = {0}, BMiserMax = {0}, BJouer = {0};
    SDL_Rect Reel1 = {0}, Reel2 = {0}, Reel3 = {0}; // Coordonées pour les 3 slots
    //Vector2i ReelOffset; ReelOffset.y = 340; ReelOffset.x = 448;
    Vector2i ReelOffset; ReelOffset.y = 210; ReelOffset.x = 448;
    //      Position par défaut (Démarage) / Offset case a case

    SDL_Event event; // Structure contenant tous les événements relatif a la fenêtre (souris clavier menus etc)
    SDL_Point MousePosition;

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

        Renderer = SDL_CreateRenderer(MainWindow, -1, 0); // -1, 0 Essaye d'utiliser l'accélération matérielle sinon revient au rendu software
        if (Renderer == NULL){
            fprintf(stderr, "Erreur a la creation du renderer : %s\n", SDL_GetError());
            exit(-1);
        }

        Faceplate = loadImage(ImagePath"faceplate.bmp", Renderer);
        SDL_QueryTexture(Faceplate, NULL, NULL, &Faceplate_DIM.w, &Faceplate_DIM.h); // on récupère la taille de la texture
        Faceplate_DIM.y = SCREEN_Y - Faceplate_DIM.h; // On déplace la texture en bas de l'écran

        for (int i = 0; i < 10; i++){
            char filename[11 + sizeof(ImagePath)]; // On aloue un buffer pour contenir le nom du fichier a charger
            sprintf(filename,ImagePath"digit%d.bmp", i);
            Digits[i] = loadImage(filename, Renderer);
        } SDL_QueryTexture(Digits[0], NULL, NULL, &Digits_DIM.w, &Digits_DIM.h);

        Buttons = loadImage(ImagePath"buttons.bmp", Renderer);
        Buttons_DIM.w = 175; Buttons_DIM.h = 125; // Vu que la texture contient tout les boutons on renseignes leur tailles manuellement
        // On initialise la position (et par conséquant leurs hitboxes) des différents boutons de l'interface
        BMiser1 = (SDL_Rect){Faceplate_DIM.x + 135, Faceplate_DIM.y + 90, Buttons_DIM.w / 2, Buttons_DIM.h / 2};
        BMiserMax = (SDL_Rect){Faceplate_DIM.x + 275, Faceplate_DIM.y + 90, Buttons_DIM.w / 2, Buttons_DIM.h / 2};
        BJouer = (SDL_Rect){Faceplate_DIM.x + 450, Faceplate_DIM.y + 90, Buttons_DIM.w / 2, Buttons_DIM.h / 2};

        Reel = loadImage(ImagePath"reel.bmp", Renderer);
        SDL_QueryTexture(Reel, NULL, NULL, &Reel1.w, NULL); // On récupère seulement l'épaisseur de la texture
        Reel3.w = Reel2.w = Reel1.w; // On définit les dimensions des trois rouleaux
        Reel3.h = Reel2.h = Reel1.h = Reel1.w * 1.5f;
        Reel3.y = Reel2.y = Reel1.y = ReelOffset.y; // On déffini la position par défaut (offset) des rouleaux

        Shadow = loadImage(ImagePath"shadow.bmp", Renderer);

        SDL_StartTextInput(); // On active l'entré texte par défaut car la machine a sou ne contient pas de crédits au démarrage
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

    while (1){ // Main loop

        if (!GUI){ // If in console mode
            system(CLEAR); // Clear the console

            // On Affiche les cartes du précédent tirage
            DisplayCardAt(SlotSize.y, SlotSize.x, CardIndex, SlotIndex[0], CardSize, 2, 10); // Card 1
            DisplayCardAt(SlotSize.y, SlotSize.x, CardIndex, SlotIndex[1], CardSize, 2, SlotSize.x + 10); // Card 2
            DisplayCardAt(SlotSize.y, SlotSize.x, CardIndex, SlotIndex[2], CardSize, 2, 2*SlotSize.x + 10); // Card 3

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
                tirage(&Gains, Mise, TabDeck, SlotIndex, WinRewards); // tirage des combinaisons
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
                    if (!TextInput){
                        // On check si l'utilisateur a appuyé sur un des boutons
                        if (SDL_PointInRect(&MousePosition, &BJouer)){
                            if (((Mise > 0) || (LastMise > 0))){
                                if ((Mise == 0) && (Credits >= LastMise)){
                                    Credits -= LastMise;
                                    Mise = LastMise;
                                }else{
                                    LastMise = Mise;
                                }
                                tirage(&Gains, Mise, TabDeck, SlotIndex, WinRewards); // tirage des combinaisons
                                Mise = 0;
                                Credits += Gains;
                            }
                            if (Credits == 0){
                                TextInput = 1;
                                SDL_StartTextInput();
                            }
                        }else if (SDL_PointInRect(&MousePosition, &BMiser1)){
                            if ((Mise < 3) && (Mise < Credits)){
                                Mise++;
                                Credits--;
                            }
                        }else if (SDL_PointInRect(&MousePosition, &BMiserMax)){
                            if ((Credits < 3) && (Credits > 0)){
                                Mise = Credits;
                                Credits = 0;
                            }else if (Mise != 3){
                                Credits -= 3 - Mise;
                                Mise = 3;
                            }
                        }
                    }
                    break;
                case SDL_KEYDOWN:
                    if ((event.key.keysym.scancode == SDL_SCANCODE_RETURN) || (event.key.keysym.scancode == SDL_SCANCODE_RETURN2)){
                        TextInput = 0;
                        creditBlink = 0;
                        SDL_StopTextInput();
                    } 
                    switch (event.key.keysym.scancode)
                    {
                    case SDL_SCANCODE_ESCAPE: // The bankrupt key
                        Credits = 0;
                        TextInput = 1;
                        SDL_StartTextInput();
                        break;
                    case SDL_SCANCODE_BACKSPACE:
                        Credits /= 10;
                        break;
                    default:
                        break;
                    }
                    break;
                case SDL_TEXTINPUT:             
                    if ((event.text.text[0] >= '0') && (event.text.text[0] <= '9')){
                        Credits *= 10;
                        Credits += (event.text.text[0] - 48);
                    }
                    break;
                default:
                    break;
                }
            }

            // Affichage des éléments (Back to Front)
            SDL_RenderCopy(Renderer, Faceplate, NULL, &Faceplate_DIM); // Affichage faceplate
            SDL_RenderCopy(Renderer, Digits[Mise], NULL, &(SDL_Rect){Faceplate_DIM.x + 471, Faceplate_DIM.y + 30, 23, 32}); // Affichage de la mise
            drawNB(Renderer, Digits, NULL, &(SDL_Rect){Faceplate_DIM.x + 120, Faceplate_DIM.y + 30, 23, 32}, (Vector2i){27, 0}, 4, Gains); // Affichage des gains
            if (!creditBlink){
                drawNB(Renderer, Digits, NULL, &(SDL_Rect){Faceplate_DIM.x + 263, Faceplate_DIM.y + 30, 23, 32}, (Vector2i){27, 0}, 4, Credits); // Affichage du nombre de crédits
            }
            // On décompose les rect affin d'avoir un plus grand control sur leurs valeurs
            SDL_RenderCopy(Renderer, Buttons, &(SDL_Rect){Buttons_DIM.w * 0, Buttons_DIM.h * 1, Buttons_DIM.w, Buttons_DIM.h}, &BMiser1); // Miser 1
            SDL_RenderCopy(Renderer, Buttons, &(SDL_Rect){Buttons_DIM.w * 1, Buttons_DIM.h * 1, Buttons_DIM.w, Buttons_DIM.h}, &BMiserMax); // Miser Max
            SDL_RenderCopy(Renderer, Buttons, &(SDL_Rect){Buttons_DIM.w * 2, Buttons_DIM.h * 1, Buttons_DIM.w, Buttons_DIM.h}, &BJouer); // Jouer

            // On affiche les slots
            animateSlots(&Reel1, ReelOffset.y, ReelOffset.x, SlotIndex[0]);
            animateSlots(&Reel2, ReelOffset.y, ReelOffset.x, SlotIndex[1]);
            animateSlots(&Reel3, ReelOffset.y, ReelOffset.x, SlotIndex[2]);

            SDL_RenderCopy(Renderer, Reel, &Reel1, &(SDL_Rect){(SCREEN_X / 4) - (Reel1.w / 8), 50, Reel1.w / 4, Reel1.h / 4});
            SDL_RenderCopy(Renderer, Shadow, NULL, &(SDL_Rect){(SCREEN_X / 4) - (Reel1.w / 8), 50, Reel1.w / 4, Reel1.h / 4});

            SDL_RenderCopy(Renderer, Reel, &Reel2, &(SDL_Rect){(SCREEN_X / 4) * 2 - (Reel2.w / 8), 50, Reel2.w / 4, Reel2.h / 4});
            SDL_RenderCopy(Renderer, Shadow, NULL, &(SDL_Rect){(SCREEN_X / 4) * 2 - (Reel2.w / 8), 50, Reel2.w / 4, Reel2.h / 4});

            SDL_RenderCopy(Renderer, Reel, &Reel3, &(SDL_Rect){(SCREEN_X / 4) * 3 - (Reel3.w / 8), 50, Reel3.w / 4, Reel3.h / 4});
            SDL_RenderCopy(Renderer, Shadow, NULL, &(SDL_Rect){(SCREEN_X / 4) * 3 - (Reel3.w / 8), 50, Reel3.w / 4, Reel3.h / 4});

            SDL_RenderPresent(Renderer); // on termine le rendu et l'affiche a l'écran
        }
    }

Shutdown:
    fclose(SlotFont);
    if (GUI){
        SDL_DestroyRenderer(Renderer); // On détruit le renderer
        SDL_DestroyWindow(MainWindow); // On détruit la fenêtre
        SDL_Quit(); // On quitte la SDL avant de quitter notre programme
    }

    return ReturnStatus;
}