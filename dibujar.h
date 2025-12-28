#ifndef DIBUJAR_H
#define DIBUJAR_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h> 
#include "bezier.h"
#include "gameplay.h"
#include "bola.h"
#include "diccionariocurvas.h"

void mysetcolor(SDL_Renderer *renderer, color_t colorset);
void dibujar_bezier(bezier_t *curva, float dx, float dy, double angulo, SDL_Renderer *renderer);
void dibujar_bola(ball_t *bola, SDL_Renderer *renderer, contenedor_t *cont);
//void dibujar_bola(ball_t *bola, void *renderer, void *cont);

#endif