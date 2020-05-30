#include "include.h"

int main(int argc, char *argv[]){
    // Déclaration des variables principales
    int Gains = 0, GuiGains = 0, Credits = 0, Mise = 0, MaxMise = 3, BankIN = 0, LastMise = 0;
    char GUI = 1, ReturnStatus = 0, TextInput = 1, BJackpot = 0, Luck = 0; // Booléen de sélection (char car il n'a besoin que d'etre 0 ou 1)

    char TabDeck[NBL] = "BELNOSI"; // 0 -> 5 Les lettres qui peuvent tomber
    int WinRewards[WIN] = {1, 2, 5, 10, 20, 40, 50, 100, 200, 300, 400, 4000, 20000}; // Les gains associés
    int SlotIndex[3] = {2, 4, 2}; // Index de la combinaison

    FILE* SlotFont = NULL; // Notre fichier contenant les "Polices" a blit dans la console
    FILE* TextBackground = NULL; // La borne en mode texte
    Vector2i SlotSize; SlotSize.x = 0; SlotSize.y = 0;
    Vector2i TextSlot1Pos, TextSlot2Pos, TextSlot3Pos;
    Vector4i TextTerminalDimensions;
    int CardSize = 0;
    int Card1DrawOffset = -1, Card2DrawOffset = -1, Card3DrawOffset = -1; // Contiendras le décalage de l'affichage d'une carte quand une combinaison est tiré (Animation)
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
    Vector2i FullscreenRES = {1920, 1080};
    Vector2i WindowRES = {1600, 900};
    Vector2i ScreenRES = {1600, 900};
    Vector2i ScallingOffset = {0, 0};
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
    Mix_Chunk *Jackpot = NULL;
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
            } else if (strcmp(argv[i], "-p") == 0){
                printf("%s\n", argv[0]);
                exit(0);
            } else if (strcmp(argv[i], "-Lucky") == 0){ // Pour obtenir des Jackpot a tout les coups
                Luck = 1;
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
                        "           (et qu'un ratio d'aspect différent peut causer des disfonctionnements)\n"
                        "-Lucky: Permet d'obtenir une chance infini\n"
                        "\n-h ou --help : Affiche ce menu d'aide\n"
                        "\nCredits : RAIMBAUD Killian & TOUGARD Enzo / 2020\n");
                exit(0);
            }
        }
    }

    if (!GUI){ // If in console mode

        switch (selectedTheme){
            case 0: // BELNOSA
                if ((SlotFont = fopen(FontPath"BIGSlotFont.txt", "r")) == NULL){
                    fprintf(stderr, "Erreur au chargement des cartes\n");
                    exit(-1);
                }
                break;
            case 1: // DICE
                if ((SlotFont = fopen(FontPath"DICESlotFont.txt", "r")) == NULL){
                    fprintf(stderr, "Erreur au chargement des cartes\n");
                    exit(-1);
                }
                break;
        }
        fscanf(SlotFont, "%d %d", &SlotSize.x, &SlotSize.y); fseek(SlotFont, 1, SEEK_CUR);
        SlotSize.x += 2; // +1 '\n' +1 '\0'
        CardSize = SlotSize.y;
        SlotSize.y *= NBL; // on a la taille d'une carte on veut toutes les cartes

        if ((TextBackground = fopen(FontPath"TextBackground.txt", "r")) == NULL){
            fprintf(stderr, "Erreur au chargement de la borne (Texte)\n");
            exit(-1);
        }
        fscanf(TextBackground, "%d %d", &TextSlot1Pos.x, &TextSlot1Pos.y); fseek(TextBackground, 1, SEEK_CUR);
        fscanf(TextBackground, "%d %d", &TextSlot2Pos.x, &TextSlot2Pos.y); fseek(TextBackground, 1, SEEK_CUR);
        fscanf(TextBackground, "%d %d", &TextSlot3Pos.x, &TextSlot3Pos.y); fseek(TextBackground, 1, SEEK_CUR);
        fscanf(TextBackground, "%d %d %d %d", &TextTerminalDimensions.x, &TextTerminalDimensions.y, &TextTerminalDimensions.w, &TextTerminalDimensions.h); fseek(TextBackground, 1, SEEK_CUR);

        char CardIndex[SlotSize.y][SlotSize.x]; // On déclare un tableau pouvant contenir toutes les cartes
        LoadTabFromFile(SlotSize.y, SlotSize.x, CardIndex, SlotFont); // On charge nottre fichier dans notre tableau

        SetConsoleSize(LINES, COLUMNS); // On standardise la taille de la console affin d'éviter les problèmes d'affichage

        system(CLEAR);
        blitBackgroundToScreen(TextBackground);
        DisplayCardAt(SlotSize.y, SlotSize.x, CardIndex, SlotIndex[0], CardSize, TextSlot1Pos.y, TextSlot1Pos.x); // Card 1
        DisplayCardAt(SlotSize.y, SlotSize.x, CardIndex, SlotIndex[1], CardSize, TextSlot2Pos.y, TextSlot2Pos.x); // Card 2
        DisplayCardAt(SlotSize.y, SlotSize.x, CardIndex, SlotIndex[2], CardSize, TextSlot3Pos.y, TextSlot3Pos.x); // Card 3

            
        while (1){ // Main loop (TextMode)

            if (ReelStep[0] >= 0){
                AnimateCard(SlotSize.y, SlotSize.x, CardIndex, SlotIndex[0], CardSize, TextSlot1Pos.y, TextSlot1Pos.x, &Card1DrawOffset, &ReelStep[0]); // Card 1
            }
            
            if (ReelStep[1] >= 0){
                AnimateCard(SlotSize.y, SlotSize.x, CardIndex, SlotIndex[1], CardSize, TextSlot2Pos.y, TextSlot2Pos.x, &Card2DrawOffset, &ReelStep[1]); // Card 2
            } 
            if (ReelStep[2] >= 0){
                AnimateCard(SlotSize.y, SlotSize.x, CardIndex, SlotIndex[2], CardSize, TextSlot3Pos.y, TextSlot3Pos.x, &Card3DrawOffset, &ReelStep[2]); // Card 3
            }
            milliSleep(3);

            if ((ReelStep[0] < 0) && (ReelStep[1] < 0) && (ReelStep[2] < 0)){ // Si les rouleaux on fini de s'animer
                // On Affiche les cartes du précédent tirage
                clearRegion(TextTerminalDimensions.y, TextTerminalDimensions.x, TextTerminalDimensions.h, TextTerminalDimensions.w);
                SetCursorAt(TextTerminalDimensions.y + 1, TextTerminalDimensions.x + (TextTerminalDimensions.w >> 1) - 21);
                printf("Gains : %5d | Credits : %5d | Mise : %1d\n", Gains, Credits, Mise); // Header
                
                if (Credits == 0){ // BankIN (Fin de partie / Début de partie quand les crédits tombent a 0)
                    SetCursorAt(TextTerminalDimensions.y + 3, TextTerminalDimensions.x + (TextTerminalDimensions.w >> 1) - 23);
                    printf("Veuiller insérer des credits pour continuer : ");
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
                    SetCursorAt(TextTerminalDimensions.y + 3, TextTerminalDimensions.x + (TextTerminalDimensions.w >> 1) - 26);
                    printf("Veuiller entrer la mise (1 - %d) 0 pour encaisser : ", MaxMise);
                    Mise = -1;
                    while ((Mise < 0) || (Mise > MaxMise)){ // Saisie sécurisé
                        while (scanf("%d", &Mise) == 0){
                            clearInputBuffer();
                        }
                    }
                    if (Mise != 0){
                        Credits -= Mise;
                    }else{
                        goto Shutdown;//exit(0); // L'utilisateur a choisi de repartir avec ses crédits
                    }
                    if (Credits < 0){ // On évité de passer dans les négatifs
                        Credits = 0;
                    }

                    // SlotMachine Logic
                    tirage(&Gains, Mise, TabDeck, SlotIndex, WinRewards, &BJackpot, 0, Luck); // tirage des combinaisons // RewardMode de 0 = Combinaison Terminal Friendly
                    ReelStep[0] = 1; ReelStep[1] = 3; ReelStep[2] = 5; // On anime les rouleaux
                    Credits += Gains;
                }
            }
        }

    } else { // if in GUI MODE

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

        MainWindow = SDL_CreateWindow(SLOTMACHINE_NAME, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WindowRES.x,WindowRES.y, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
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
            ToWideScreen(&FullscreenRES, &ScreenRES, &ScallingOffset);
            SDL_SetWindowFullscreen(MainWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
        }else{
            SDL_SetWindowFullscreen(MainWindow, 0);
            ToWideScreen(&WindowRES, &ScreenRES, &ScallingOffset);
        }

        // Initialisation du moteur audio en qualité CD 44100Khz, Stereo, 1kb par chunk
        if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) != 0){
            fprintf(stderr, "Erreur a l'inintialisation du moteur audio : %s\n", Mix_GetError());
            exit(-1);
        }

        neoBG = loadImage(ImagePath"NeoBackGround.png", Renderer);
        casinoBG = loadImage(ImagePath"casinoBG1.png", Renderer); // Temporary
        NeoSign = loadImage(ImagePath"neoSign2.png", Renderer);
        casinoSign = loadImage(ImagePath"casinoSign.png", Renderer);
        NeoPlate = loadImage(ImagePath"NeoPlate.png", Renderer);
        CasinoPlate = loadImage(ImagePath"casinoPlate.png", Renderer);

        for (int i = 0; i < 10; i++){
            char filename[11 + sizeof(ImagePath)]; // On aloue un buffer pour contenir le nom du fichier a charger
            sprintf(filename,ImagePath"digit%d.png", i);
            Digits[i] = loadImage(filename, Renderer);
        }

        Buttons = loadImage(ImagePath"buttons3.png", Renderer);
        NeoReel = loadImage(ImagePath"reelNEO.png", Renderer);
        casinoReel = loadImage(ImagePath"reel.png", Renderer);
        Shadow = loadImage(ImagePath"shadow.png", Renderer);

        // Sound effects
        coinIn = loadSoundEffect(SoundPath"payout1.wav");
        coinIn2 = loadSoundEffect(SoundPath"payout2.wav");
        coinIn3 = loadSoundEffect(SoundPath"payout3.wav");
        Jackpot = loadSoundEffect(SoundPath"Jackpot.wav");
        spin = loadSoundEffect(SoundPath"spin.wav");

        // Musics
        casinoBGM = loadMusic(SoundPath"Halos of Eternity.ogg");
        neoBGM = loadMusic(SoundPath"tokyo-rose.ogg");
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
        SignDIM.x = ScreenRES.x - SignDIM.w + ScallingOffset.x;
        SignDIM.y = ((ScreenRES.y - SignDIM.h) >> 2) + ScallingOffset.y;

        SDL_QueryTexture(Faceplate, NULL, NULL, &Faceplate_DIM.w, &Faceplate_DIM.h); // on récupère la taille de la texture
        Scale(&Faceplate_DIM.h, &Faceplate_DIM.w, ScreenRES.y);
        Faceplate_DIM.x = ScallingOffset.x;
        Faceplate_DIM.y = ScallingOffset.y;

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
        Reel3.y = Reel2.y = Reel1.y = ReelOffset.y + ScallingOffset.y; // On déffini la position par défaut (offset) des rouleaux

        ScaleTextureToLinkedPercent(&Reel1_DIM, ScreenRES.x, 15.5f);
        Reel1_DIM = (SDL_Rect){(ScreenRES.x * 0.120f) + ScallingOffset.x, (ScreenRES.y * 0.215f) + ScallingOffset.y, Reel1_DIM.w, Reel1_DIM.h};
        Reel2_DIM = (SDL_Rect){((Faceplate_DIM.w - Reel1_DIM.w) >> 1) + ScallingOffset.x, (ScreenRES.y * 0.215f) + ScallingOffset.y, Reel1_DIM.w, Reel1_DIM.h};
        Reel3_DIM = (SDL_Rect){(ScreenRES.x * 0.482f) + ScallingOffset.x, (ScreenRES.y * 0.215f) + ScallingOffset.y, Reel1_DIM.w, Reel1_DIM.h};

        // Snaps the slots into place for the default combination
        snapSlots(&Reel1, ReelOffset.y, ReelOffset.x, SlotIndex[0]);
        snapSlots(&Reel2, ReelOffset.y, ReelOffset.x, SlotIndex[1]);
        snapSlots(&Reel3, ReelOffset.y, ReelOffset.x, SlotIndex[2]);

        while (1){ // Main loop (Gui Mode)
        
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
                                tirage(&GuiGains, Mise, TabDeck, SlotIndex, WinRewards, &BJackpot, rewardMode, Luck); // tirage des combinaisons
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
                            ToWideScreen(&WindowRES, &ScreenRES, &ScallingOffset);
                            SDL_SetWindowFullscreen(MainWindow, 0);
                            Fullscreen = 0;
                            goto scaleini;
                        }else{
                            ToWideScreen(&FullscreenRES, &ScreenRES, &ScallingOffset);
                            SDL_SetWindowFullscreen(MainWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
                            Fullscreen = 1;
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
                case SDL_WINDOWEVENT:
                    switch (event.window.event)
                    {
                    case SDL_WINDOWEVENT_RESIZED:
                        WindowRES.x = event.window.data1; // Nouvelle resolution X
                        WindowRES.y = event.window.data2; // Nouvelle resolution Y
                        ToWideScreen(&WindowRES, &ScreenRES, &ScallingOffset); // On scale pour que l'affichage reste correct
                        goto scaleini; // On redimentionne toutes les texture pour correspondre a la nouvelle resolution
                        break;
                    
                    default:
                        break;
                    }

                    break;
                default:
                    break;
                }
            }

            // Affichage des éléments (Back to Front)
            SDL_RenderFillRect(Renderer, NULL); // On efface la cible de rendu pour éviter le ghosting avec des résolutions hors aspect ratio
            SDL_RenderCopy(Renderer, BackGround, NULL,  &(SDL_Rect){ScallingOffset.x, ScallingOffset.y, ScreenRES.x, ScreenRES.y}); // On affiche le background

            // On anime les slots
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
                if (BJackpot){ // Si JackPot
                    Mix_PlayChannel(-1, Jackpot, 0); // On joue le son du tirage -1 pour laisser la sdl choisir le channel
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
    }else{
        fclose(SlotFont);
        fclose(TextBackground);
        system(CLEAR);
    }

    return ReturnStatus;
}