# YetAnotherSlotProject
YASP aussi appelé "YetAnotherSlotProject" est un simulateur de machine a sous (encore un),
développé en tant que projet S2 pour l'ESIEA.

Compilation :

    Afin de compiler yasp, dans le dossier principal executez "make" via le terminal.
    Cela créras un dossier YASP contenant l'ensemble des assets ainsi que l'executable yasp

    Le projet est utilisable tel quel et peut être déplacé vers n'importe quel dossier.

Installation :

    Si vous désirez installer yasp comme n'importe quel autre programme,
    executez "sudo make install" dans le terminal

Désinstallation :

    Simmilaire a l'installation, si vous désirez desinstaller yasp de votre système,
    executez "sudo make uninstall".

Execution :

    Afin d'executer yasp, il vous suffit de tapez "yasp" dans votre terminal,
    (si vous l'avez installé).
    Sinon rendez vous dans le dossier compilé YASP et executez "yasp"
    
    Pour lancer yasp en mode terminal executez "yasp -Term"
    Afin de changer de thème rajoutez l'option "-t" suivi du numéro du thème
    ex "yasp -Term -t 1"

Dependences :

    Afin de pouvoir compiler yasp vérifiez bien que vous possédez toutes les dépendances
    nécessaires.

    - libsdl2-dev
    - libsdl2-mixer-dev
    - libsdl2-image-dev

    Vous pouvez installer les dépendances en executant "make dependencies"

Notes:

    Yasp a été conçu pour fonctionner sur Ubuntu 18.04 et + vérifiez au préalable
    la compatiblité de votre système

    Yasp a été consu avec des options pour les utilisateur avancés,
    executez "yasp -h" ou "yasp --help" pour en optenir une liste descriptive

    yasp se lance par défaut en mode plein ecran a la résolution native de celui-ci

    La version terminal a été conçu pour un terminal de résolution 77 Lignes * 263 Colones,
    veuillez ajuster votre terminal en conséquence

    Vous avez perdu votre chance légendaire et n'arrivez pas a décrocher un Jackpot ?
    Utilisez l'option -Lucky et devenez le meuilleur joueur de machine a sous du monde !

    Keybindings SDL:

        Pavénum     : Entrer montant de crédits
        Entrer      : Valider credits
        + (PN)      : +1 mise
        Espace      : Jouer
        Backspace   : Récupérez ses crédits
        F           : Basculer entre le mode plein ecran et le mode fenétré
        Fèches (GD) : Changer de thèmes (SDL)
        Echap       : Quitter