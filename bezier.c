#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <assert.h>
#include <math.h>
#include <string.h>

#include "bezier.h"
#include "config.h"


struct bezier{ 
    size_t n_puntos;
    float (*puntos)[2];
    size_t grado;
};

bezier_t *bezier_crear(size_t grado){

    bezier_t *p;   
    p = malloc(sizeof(bezier_t));
    if(p == NULL) {
        //free(p); hace falta?
        return NULL;
    }
    p->puntos = NULL;
    p->n_puntos = 0;
    p->grado = grado;

    return p;      
}

void bezier_destruir(bezier_t *bezier){

    free(bezier->puntos);
    free(bezier);              

}

size_t bezier_cantidad_tramos(const bezier_t *bezier){

    if(bezier->n_puntos < ((bezier->grado) + 1)) return 0;
    return ((bezier->n_puntos)-1)/bezier->grado;

}

bool bezier_agregar_punto(bezier_t *bezier, float x, float y){
    
    float (*pointer)[2];
    pointer = realloc(bezier->puntos,  bezier->n_puntos * sizeof(float[2]) * 2 + sizeof(float[2]) * 2);

    if(pointer == NULL) return false;

    bezier->puntos = pointer;

    //free(pointer);

    bezier->n_puntos += 1;    
    bezier->puntos[bezier->n_puntos -1][0] = x;
    bezier->puntos[bezier->n_puntos - 1][1] = y;

    return true;

}

bezier_t *bezier_clonar(const bezier_t *bezier){

    bezier_t *clon;
    clon = malloc(sizeof(bezier_t));
    if(clon == NULL) {
        
        return NULL;
    }

    
    clon->puntos = malloc(bezier->n_puntos * sizeof(float[2])*2);
    if(clon->puntos == NULL) {
        free(clon);
        return NULL;
    }

    memcpy(clon->puntos,bezier->puntos,bezier->n_puntos * sizeof(float[2])*2);
    clon->n_puntos = bezier->n_puntos;
    clon->grado = bezier->grado;

    return clon;

}

static double potencia(double x, int y){
    double xpwry = 1;
    
    for(int i = 1; i <= y; i++ ){
        xpwry *= x;
    
    }

    return xpwry;
}

static int factorial(int n){
    int fact = 1;
    for(int i = 1; i <= n; i++){
        fact *= i;

    }
    return fact;
}

static int binomial(int n, int k){
    return(factorial(n) / ( factorial(k) * factorial(n - k) ));

}

static double bernstein(int n, int i, double t){
    return((binomial(n, i)) * (potencia(t, i)) * (potencia((1 - t), (n - i))));

}

double bezier1d(int n, float c[n + 1], double t){
    double bezieruni = 0; 
    for(int i = 0; i <= n; i++){
        bezieruni += c[i] * bernstein(n, i, t);

    }
    return bezieruni;

}

bool bezier_evaluar(const bezier_t *bezier, double t, double *x, double *y){

    if (t > bezier_cantidad_tramos(bezier) || t < 0){
        return false;
    }

    int numtramo = (int)t;

    float xx[bezier->grado + 1];
    float yy[bezier->grado + 1];


    for(int i = 0; i <= bezier->grado; i++){
        xx[i] = bezier->puntos[bezier->grado * numtramo + i][0];
        yy[i] = bezier->puntos[bezier->grado * numtramo + i][1];
        
    }

    
    *x = bezier1d(bezier->grado, xx, t-numtramo);
    *y = bezier1d(bezier->grado, yy, t-numtramo);


    return true;

}

void trasladar(bezier_t *bezier, float dx, float dy){
    for(int i = 0; i < bezier->n_puntos; i++){
        bezier->puntos[i][0] += dx;
        bezier->puntos[i][1] += dy;

    }
}

void rotar(bezier_t *bezier, double rad){
    float one;
    float two;
    for(int i = 0; i < bezier->n_puntos; i++){
        one = bezier->puntos[i][0] * cos(rad) - bezier->puntos[i][1] * sin(rad);
        two = bezier->puntos[i][0] * sin(rad) + bezier->puntos[i][1] * cos(rad);
        bezier->puntos[i][0] = one;
        bezier->puntos[i][1] = two;
    }
}

double dist(float px, float py, double bx, double by){
    double distancia = sqrt(potencia((px - bx), 2) + potencia((py - by), 2));
    return distancia;
}


double cercania_a_bezier(bezier_t *bezier, float px, float py){
    double bezierbix = 0;
    double bezierbiy = 0;
    float cerca = dist(px, py, bezier->puntos[0][0], bezier->puntos[0][1]);
    double tfinal = 0;
    for(double t = 0; t <= bezier_cantidad_tramos(bezier); t += PASO){


        bezier_evaluar(bezier, t, &bezierbix, &bezierbiy);

        float cerca2 = dist(px, py, bezierbix, bezierbiy);

        if(cerca2 < cerca){
            cerca = cerca2;
            tfinal = t;
        }

        if(cerca2 > -1e-5 && cerca2 < 1e-5){
            cerca = cerca2;
            return t;
        }

        bezierbix = 0;
        bezierbiy = 0;
    }

    return tfinal;
}


double bezier_avanzar(bezier_t *bezier, double t_inicial, double distancia){
    double izq = t_inicial;
    double der = (double)bezier_cantidad_tramos(bezier);

    int v = 0;

    double mitad = (izq + der)/2;
        
    double ix, iy, x, y, xx, yy, xxx, yyy;
    
    bezier_evaluar(bezier, izq, &x, &y);

    ix = x;
    iy = y;

    bezier_evaluar(bezier, der, &xx, &yy);
    bezier_evaluar(bezier, mitad, &xxx, &yyy);

    if(distancia >= dist(ix, iy, xx, yy)){
        return der;   
    }

    double resx = dist(ix, iy, xxx, yyy);
    
    while(resx > (distancia + 1e-3) || resx < (distancia - 1e-3)){

        v++;

        if(resx > (distancia + 1e-3)){
            der = mitad;
            bezier_evaluar(bezier, der, &xx, &yy);
        }
        else if(resx < (distancia - 1e-3)){
            izq = mitad;
            bezier_evaluar(bezier, izq, &x, &y);
        }

        mitad = (izq + der)/2;   

        bezier_evaluar(bezier, mitad, &xxx, &yyy);

        resx = dist(ix, iy, xxx, yyy);
    }
    return mitad; 
}