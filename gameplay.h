#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include <stdint.h>

#include "bezier.h"

struct nivel{
    char *curvanombre;  
    double x;
    double y;           //cordenadas del sapo
    int cant;           //cant bolas del nivel
    double segsinicial; // cantidad de segundos a velocidad inicial
    
};

typedef struct nivel nivel_t;

enum niveles{NIVEL1, NIVEL2, NIVEL3, NIVEL4};

typedef enum {ROJO, VERDE, AZUL, AMARILLO} color_t;


#endif