#ifndef SDL_FUNCTIONS
#define SDL_FUNCTIONS

#include "include.h"

#include <SDL2/SDL.h> // Main sdl Header
#include <SDL2/SDL_mixer.h> // Audio support
#include <SDL2/SDL_image.h> // Png support

// Charge une image dans une texture
SDL_Texture* loadImage(const char path[], SDL_Renderer* renderer);

// Ces deux fonctions peuvent sembler redondantes, mais elles permettent d'éviter d'avoir a réécrire le code de gestion d'erreur
Mix_Chunk* loadSoundEffect(const char path[]);

// Charge une musique en mémoire
Mix_Music* loadMusic(const char path[]);

// Affiche un nombre avec des times (Algo droite gauche :3(comme quoi ça sert))
void drawNB(SDL_Renderer* renderer, SDL_Texture* digitTabTexture[], SDL_Rect* srcrect, SDL_Rect dstrect, Vector2i offset, int nbOfElements, int NB);

// Snap the slots to a certain combinaison
void snapSlots(SDL_Rect * coordinates,int originOffset, int stepOffset, int newID);

// Animates the slots smoothly
void animateSlots(SDL_Rect * coordinates,int originOffset, int stepOffset, int newID, float maxSpeed, int *Steps);

// Scale vers une nouvelle resolution en gardant le même ratio d'aspect
void Scale(int *A, int *B, int C);
// Scale by X %
void ScalePercent(int *A, int Percent);

// Scale une texture en fonction d'un pourcentage de la résolution LinkedRes
void ScaleTextureToLinkedPercent(SDL_Rect *Dimensions, int LinkedRes, float Percent);

// Prends en entrée une résolution SrcRes et la transforme en une résolution 16/9 DstRes de même taille ou inférieure a SrcRes
void ToWideScreen(Vector2i* SrcRes, Vector2i* DstRes, Vector2i* SclOffset);




#endif