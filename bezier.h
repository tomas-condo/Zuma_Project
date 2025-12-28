#ifndef BEZIER_H
#define BEZIER_H

#include <stdbool.h>
#include <stddef.h>

typedef struct bezier bezier_t;

bezier_t *bezier_crear(size_t grado);
void bezier_destruir(bezier_t *bezier);
size_t bezier_cantidad_tramos(const bezier_t *bezier);
bool bezier_agregar_punto(bezier_t *bezier, float x, float y);
bezier_t *bezier_clonar(const bezier_t *bezier);
double bezier1d(int n, float c[n + 1], double t); //SACAR DESP
bool bezier_evaluar(const bezier_t *bezier, double t, double *x, double *y);
double dist(float px, float py, double bx, double by);
void trasladar(bezier_t *bezier, float dx, float dy);
void rotar(bezier_t *bezier, double rad);
double cercania_a_bezier(bezier_t *bezier, float px, float py);
double bezier_avanzar(bezier_t *bezier, double t_inicial, double distancia);


#endif