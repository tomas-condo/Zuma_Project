#ifndef BOLA_H
#define BOLA_H

#include "bezier.h"
#include "gameplay.h"

#include <stdbool.h>
#include <stdint.h>


typedef struct ball ball_t;

ball_t *bola_crear(color_t color, float cordx, float cordy, bool normal);
void bola_destruir(ball_t *b);
color_t devolver_color(ball_t *b);
color_t cambiar_color(ball_t *boca, color_t color);
void coordenadas_devolver(ball_t *b, float *cordx, float *cordy);
void cambiar_coordenadas(ball_t *b, float cordx, float cordy);
bool devolver_normal(ball_t *b);
void cambiar_normal(ball_t *b);
double devolver_te(ball_t *b);
void cambiar_te(ball_t *b, double t);
bool mover_bola_linea(ball_t *b, double centrox, double centroy);
color_t boladevolvercolor(ball_t *b);



#endif