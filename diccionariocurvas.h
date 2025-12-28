#ifndef CONTENEDORBEZIER_H
#define CONTENEDORBEZIER_H


#include <stdbool.h>
#include "bezier.h"


typedef struct contenedor contenedor_t;

contenedor_t *contenedor_crear();
bool contenedor_llenar(contenedor_t *cont, FILE *f);
void contenedor_destruir(contenedor_t *cont);
bezier_t *bezier_buscar_contenedor(const contenedor_t *cont, const char *nombre);


#endif