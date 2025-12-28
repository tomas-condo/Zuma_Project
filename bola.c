#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <math.h>
#include <string.h>

#include "config.h"
#include "bola.h"
#include "gameplay.h"


struct ball{
    color_t color; //etiqueta enum
    float cordx, cordy; //posicion
    bool normal; //true si es normal false si es especial
    double t;

};

ball_t *bola_crear(color_t color, float cordx, float cordy, bool normal){
    ball_t *b = malloc(sizeof(ball_t));
    if(b == NULL) return NULL;

    b->color = color;
    b->cordx = cordx;
    b->cordy = cordy;
    b->normal = normal;
    b->t = 0;

    return b;
}

void bola_destruir(ball_t *b){
    free(b);
}

color_t devolver_color(ball_t *b){
    return b->color;
}

color_t cambiar_color(ball_t *boca, color_t color){
    color_t coloraux = boca->color;
    boca->color = color;
    return coloraux;

}

void coordenadas_devolver(ball_t *b, float *cordx, float *cordy){
    *cordx = b->cordx;
    *cordy = b->cordy;
}

void cambiar_coordenadas(ball_t *b, float cordx, float cordy){
    b->cordx = cordx;
    b->cordy = cordy;
}

bool devolver_normal(ball_t *b){
    return b->normal;
}

void cambiar_normal(ball_t *b){
    b->normal = !b->normal;
}

double devolver_te(ball_t *b){
    return b->t;
}

void cambiar_te(ball_t *b, double t){
    b->t = t;
}

//bola mover linea recta

bool mover_bola_linea(ball_t *b, double centrox, double centroy){

    float coordx, coordy;
    coordenadas_devolver(b, &coordx, &coordy);
    float angulodisparo = atan2(coordy - centroy, coordx - centrox);

    size_t pixeles_avanzar = VELOCIDAD_DISPARO / JUEGO_FPS;

    int x = cos(angulodisparo) * pixeles_avanzar;
    int y = sin(angulodisparo) * pixeles_avanzar;

    b->cordx += x;
    b->cordy += y;

    
    //SI LA BOLA SE VA DE LA PANTALLA DESTRUIRLA
    if((coordx < (0 - (DIAMETRO/2)) || coordx > (VENTANA_ANCHO + (DIAMETRO/2))) || (coordy < (0 - (DIAMETRO/2))|| coordy > (VENTANA_ALTO + (DIAMETRO/2)))){
        bola_destruir(b);
        return false;
    }

    return true;

}

color_t boladevolvercolor(ball_t *b){
    return b->color;
}