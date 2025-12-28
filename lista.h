#ifndef LISTA_H
#define LISTA_H

#include "bezier.h"
#include "bola.h"
#include <stdbool.h>
#include <stdint.h>

typedef struct nodo nodo_t;
typedef struct listofballs lista_t;

nodo_t *nodo_crear(ball_t *b);
void nodo_destruir(lista_t *l, nodo_t *n);
ball_t *devolver_dato(nodo_t *n);
lista_t *lista_crear();
void lista_destruir(lista_t *l);
bool bola_insertar_comienzo(lista_t *l, bezier_t *curvanivel);
bool lista_es_vacia(const lista_t *l);
nodo_t *bola_buscar(lista_t *l, ball_t *b);
void bolas_empujar(lista_t *l, bezier_t *curvanivel);
int bola_insertar_disparo(lista_t *l, nodo_t *bolabuscada, ball_t *disparo, bezier_t *curvanivel);
void imprimir_lista_adelante(lista_t *l);
void imprimir_lista_atras(lista_t *l);
void eliminar_especial(lista_t *l, nodo_t *n);
void bolas_empujar(lista_t *l, bezier_t *curvanivelo);
void bolas_retroceder(nodo_t *n);
void mover_bola_bezier(lista_t *l, bezier_t *curvanivel, size_t velocidad);
nodo_t *ultima_bola(lista_t *l);
ball_t **lista_a_vector(lista_t *l, size_t *n);

#endif
