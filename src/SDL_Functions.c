#include "SDL_Functions.h"

SDL_Texture* loadImage(const char path[], SDL_Renderer* renderer){
    // Nottre surface temporaire pour le chargement des textures
    SDL_Surface* LoadingSurface = IMG_Load(path); // On charge le fichier en mémoire
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
            (*coordinates).y = stepOffset * newID + originOffset; // TEMP
            (*Steps)--;
        }
        //(*coordinates).y = stepOffset * newID + originOffset;
    }

    //printf("%d\n", (*coordinates).y);
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

void ToWideScreen(Vector2i* SrcRes, Vector2i* DstRes, Vector2i* SclOffset){
    int TEMP = (float)((*SrcRes).x / 16.0f * 9.0f);
    if (TEMP < (*SrcRes).y){
        (*DstRes).x = (*SrcRes).x;
        (*DstRes).y = TEMP;
        (*SclOffset).x = 0;
        (*SclOffset).y = ((*SrcRes).y - (*DstRes).y) >> 1;
    }else{
        (*DstRes).y = (*SrcRes).y;
        (*DstRes).x = (float)((*SrcRes).y / 9.0f * 16.0f);
        (*SclOffset).y = 0;
        (*SclOffset).x = ((*SrcRes).x - (*DstRes).x) >> 1;
    }
    //printf("Src %d %d\nDst %d %d\nOff %d %d\n", (*SrcRes).x, (*SrcRes).y, (*DstRes).x, (*DstRes).y, (*SclOffset).x, (*SclOffset).y);
}