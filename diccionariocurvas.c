#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "bezier.h"
#include "diccionariocurvas.h"
#include "config.h"


struct contenedor{
    size_t num;
    bezier_t *vectorbezier[MAX_CURVAS]; // tamanio n
    char *nombres[MAX_CURVAS]; //tamanio n
    //mismos indices representan mismas curvas
};


static bool leer_nombre(FILE *f, char **s){

    unsigned char cantletras; //longitud de la cadena

    if(fread(&cantletras, sizeof(unsigned char), 1, f) != 1) return false;

    //*s = malloc(sizeof(char) * (cantletras + 1));
    *s = calloc((cantletras+1), sizeof(char));
    if(*s == NULL) return false;

    if(fread(*s, sizeof(char), cantletras, f) != cantletras) return false;

    //s[cantletras] = '\0'; //NUNCA SACARLE EL COMENTARIO

    return true;
}

static bool leer_numero_puntos(FILE *f, uint32_t *n){

    if(fread(n, sizeof(uint32_t), 1, f) != 1) return false;

    return true;
}

static bool leer_punto(FILE *f, float *x, float *y){

    if(fread(x, sizeof(float), 1, f) != 1) return false;
    if(fread(y, sizeof(float), 1, f) != 1) return false;

    return true;
}

contenedor_t *contenedor_crear(){
    contenedor_t *cont = malloc(sizeof(contenedor_t));
    if(cont == NULL) return NULL;
    cont->num = 0;
    /*cont->vectorbezier = NULL;
    cont->nombres = NULL;*/
    return cont;
}

bool contenedor_llenar(contenedor_t *cont, FILE *f){

    char *s;
    uint32_t nn;
    float x ,y;
    size_t longitud;

    int i = 0;
    while(leer_nombre(f, &s)){

        cont->num = (i + 1);

        cont->vectorbezier[i] = bezier_crear(GRADO);
        if(cont->vectorbezier[i] == NULL) return false;

        longitud = strlen(s);

        cont->nombres[i] = malloc(sizeof(char) * (longitud + 1)); 
        if(cont->nombres[i] == NULL) return false;

        memcpy(cont->nombres[i], s, sizeof(char) * (longitud + 1));

        free(s);

        leer_numero_puntos(f, &nn);

        for(int j = 0; j < nn; j++){
            leer_punto(f, &x, &y);
            bezier_agregar_punto(cont->vectorbezier[i], x, y);
        }
        i++;   
    }
    fclose(f);
    return true;
}

void contenedor_destruir(contenedor_t *cont){
    for(size_t i = 0; i < cont->num; i++)
        bezier_destruir(cont->vectorbezier[i]);
    
    for(size_t i = 0; i < cont->num; i++)
        free(cont->nombres[i]);
    
    free(cont);
}

bezier_t *bezier_buscar_contenedor(const contenedor_t *cont, const char *nombre){
    for(size_t i = 0; i < cont->num; i++)
        if(strcmp(cont->nombres[i], nombre) == 0) return cont->vectorbezier[i];
    
    return NULL;
}