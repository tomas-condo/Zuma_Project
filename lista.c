#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <math.h>
#include <string.h>
#include <time.h>

#include "lista.h"
#include "bezier.h"
#include "bola.h"
#include "config.h"
#include "gameplay.h"



struct nodo{
    struct nodo *ant;
    struct nodo *sig;
    ball_t *dato;
};

struct listofballs{
    nodo_t *prim; //prim->ant == NULL
};

nodo_t *nodo_crear(ball_t *b){
    nodo_t *n = malloc(sizeof(nodo_t));
    if(n == NULL) return NULL;
    n->ant = NULL;
    n->sig = NULL;
    n->dato = b;
    return n;
}

void nodo_destruir(lista_t *l, nodo_t *n){
    if(n == NULL) return;

    if((n->sig == NULL) && (n->ant == NULL)){
        l->prim = NULL;
        bola_destruir(n->dato);
        free(n);
        return;
    }

    if(n->ant == NULL){
        l->prim = n->sig;
        l->prim->ant = NULL;
        bola_destruir(n->dato);
        free(n);
        return;
    }

    if(n->sig != NULL)
    n->sig->ant = n->ant;

    if(n->ant != NULL)
    n->ant->sig = n->sig;

    bola_destruir(n->dato);

    free(n);
}

ball_t *devolver_dato(nodo_t *n){
    return n->dato;
}

lista_t *lista_crear(){
    lista_t *l = malloc(sizeof(lista_t));
    if(l == NULL) return NULL;
    l->prim = NULL;
    return l;
}

void lista_destruir(lista_t *l){
    nodo_t *actual = l->prim;
    while(actual != NULL){
        nodo_t *siguiente = actual->sig;
        nodo_destruir(l, actual);
        actual = siguiente;

    }
    free(l);
}

bool bola_insertar(lista_t *l, ball_t *bola){
    nodo_t *n = nodo_crear(bola);
    if(lista_es_vacia(l)){
        l->prim = n;
        return true;
    }
    if(n == NULL) return false;
    n->ant = NULL;
    n->sig = l->prim;
    n->sig->ant = n;
    l->prim = n;
    return true; 
}

bool bola_insertar_comienzo(lista_t *l, bezier_t *curvanivel){

    double curvax0, curvay0;

    curvax0 = 0;
    curvay0 = 0;

    if(curvanivel != NULL)
        bezier_evaluar(curvanivel, 0, &curvax0, &curvay0);


    color_t c = (rand()%4);
    bool norm = ((rand()%20)==1)?false:true;
    ball_t *nueva = bola_crear(c, curvax0, curvay0, norm);


    if(lista_es_vacia(l)){

        bola_insertar(l, nueva);

        return true;
    }

    static float x, y; 
    coordenadas_devolver(l->prim->dato, &x, &y);

    if(dist(x, y, curvax0, curvay0) >= DIAMETRO){

        bola_insertar(l, nueva);
        return true;  
    }

    else return false;

}

bool lista_es_vacia(const lista_t *l){
    return l->prim == NULL;
}

//Una bola disparada solo entra a la lista si toca a otra bola.
//lista buscar deberia devolver el nodo que toque la bola disparada.

nodo_t *bola_buscar(lista_t *l, ball_t *boladisparada){
    //habria que recorrer la lista para ver que bola toca
    if(lista_es_vacia(l)) return NULL;

    nodo_t *actual = l->prim;
    float x, y, xx, yy;
    coordenadas_devolver(boladisparada, &xx, &yy);
    coordenadas_devolver(actual->dato, &x, &y);

    if(dist(x, y, xx, yy) <= DIAMETRO){
        return actual;
    }

    while(actual->sig != NULL){
        coordenadas_devolver(actual->sig->dato, &x, &y);
        if(dist(x, y, xx, yy) > DIAMETRO){
            actual = actual->sig;
        }
        else{
            return actual->sig;
        }

    }

    return NULL;
}

//Si listabuscar != NULL, bola insertar disparo deberia insertar la bola disparada como sig de la bola devuelta por bolabuscar
//en su misma posicion y luego ser empujada.

//podria recibir nodo y empujar desde ahi

void bolas_empujar(lista_t *l, bezier_t *curvanivel){
    //if(lista_es_vacia(l) == true) break;  
    nodo_t *actual = l->prim;

    if(actual->sig == NULL) return;

    float x, y, xx, yy;
    double xxx, yyy;

    while(actual->sig != NULL){

        actual = actual->sig;  //porque a prim no deberia empujarlo nada
        coordenadas_devolver(actual->dato, &x, &y);
        coordenadas_devolver(actual->ant->dato, &xx, &yy);

        if(dist(x, y, (double)xx, (double)yy) <= DIAMETRO){
            
            double t_inicial = devolver_te(actual->ant->dato);

            double t_mover = bezier_avanzar(curvanivel, t_inicial, DIAMETRO);
            bezier_evaluar(curvanivel, t_mover, &xxx, &yyy);
            cambiar_coordenadas(actual->dato, (float)xxx, (float)yyy);
            
            cambiar_te(actual->dato, t_mover);
        }
    }
}

//Si listabuscar != NULL, bola insertar disparo deberia insertar la bola disparada como sig de la bola devuelta por listabuscar
//en su misma posicion y luego ser empujada.

int bola_insertar_disparo(lista_t *l, nodo_t *bolabuscada, ball_t *disparo, bezier_t *curvanivel){

    //metemos el disparo a la lista
    
    nodo_t *nodo_insertar = nodo_crear(disparo);
    nodo_insertar->sig = bolabuscada->sig;
    bolabuscada->sig = nodo_insertar;    
    nodo_insertar->ant = bolabuscada;
    if(nodo_insertar->sig != NULL){
        nodo_insertar->sig->ant = nodo_insertar;      
    }


    float x, y;
    coordenadas_devolver(bolabuscada->dato, &x, &y);

    cambiar_coordenadas(nodo_insertar->dato, x, y);

    bolas_empujar(l, curvanivel);


    nodo_t *actual = nodo_insertar;
    nodo_t *aux;

    int puntaje = 0;

    int exp = 0; //exponencial

    bool uno = false;
    bool dos = false;
    bool tres = false;

    while((actual == nodo_insertar)||(devolver_color(actual->dato) == devolver_color(actual->ant->dato))){


        if(actual->ant != NULL){
            if(actual->ant->ant != NULL){
                if((devolver_color(actual->dato) == devolver_color(actual->ant->dato)) && (devolver_color(actual->dato) == devolver_color(actual->ant->ant->dato))){
                    uno = true;
                }
                else uno = false;            
            }
            else uno = false;
        }

        if(actual->sig != NULL){
            if(actual->ant != NULL){
                if((devolver_color(actual->dato) == devolver_color(actual->ant->dato)) && (devolver_color(actual->dato) == devolver_color(actual->sig->dato))){
                    dos = true;
                }
                else dos = false;
            }
            else dos = false;
        }


        if(actual->sig != NULL){
            if(actual->sig->sig != NULL){
                if((devolver_color(actual->dato) == devolver_color(actual->sig->dato)) && (devolver_color(actual->dato) == devolver_color(actual->sig->sig->dato))){                   
                    tres = true;
                }
                else tres = false;
            }
            else tres = false;

        }

        if((uno || dos) || tres){
            //actual == disparo en primer vuelta
            aux = actual;

            while(aux->sig != NULL){ 
                if((devolver_color(aux->sig->dato) == devolver_color(aux->dato))){
                    if(!devolver_normal(aux->sig->dato)){
                            aux = aux->sig;
                            actual = aux;
                            eliminar_especial(l, aux);
                            cambiar_normal(actual->dato);
                            puntaje += pow(2, exp) * PUNTOS_POR_BOLA * 4;
                    }
                    else{
                        aux = aux->sig;
                    }
                }
                else break;

            }
            if(actual!=aux){
                
                while((actual->sig != aux)){
                    nodo_destruir(l, actual->sig);
                    puntaje += pow(2, exp) * PUNTOS_POR_BOLA;
                }
                nodo_destruir(l, aux);
                puntaje += pow(2, exp) * PUNTOS_POR_BOLA;
                aux = actual;

            }  


            while(aux->ant != NULL){
                if((devolver_color(aux->ant->dato) == devolver_color(aux->dato))){
                    if(!devolver_normal(aux->ant->dato)){
                            aux = aux->ant;
                            actual = aux;
                            eliminar_especial(l, aux);
                            cambiar_normal(actual->dato);
                            puntaje += pow(2, exp) * PUNTOS_POR_BOLA * 4;
                    }
                    else{
                        aux = aux->ant;

                    }
                }
                else break;

            }
            if(actual!=aux){
                while((actual->ant != aux)){
                    nodo_destruir(l, actual->ant);
                    puntaje += pow(2, exp) * PUNTOS_POR_BOLA;
                }
                nodo_destruir(l, aux);
                puntaje += pow(2, exp) * PUNTOS_POR_BOLA;
                aux = actual;
            } 

            //no hay agujero
            if((actual->ant == NULL) || (actual->sig == NULL)){
                nodo_destruir(l, actual);
                return (puntaje + pow(2, exp) * PUNTOS_POR_BOLA);
            }

            //vuelve a preguntar el while
            else{
                aux = actual->sig;
                nodo_destruir(l, actual);
                puntaje = (puntaje + pow(2, exp) * PUNTOS_POR_BOLA);
                actual = aux; //devolver
                exp++;
                if(exp == 1) exp = 2;

            }

        }

        //primera vuelta no hay explosion ni combo
        else break;    

    }

    //si es <=2 no hubo combo 
    
    if((exp > 2) && (!lista_es_vacia(l))){
        //bolas_retroceder(actual->ant);
        bolas_empujar(l, curvanivel);
        //EMPUJAR AFUERA
    }//cuando ya se elimino el combo.

    return puntaje; 

}

void eliminar_especial(lista_t *l, nodo_t *n){
    if(n->sig!=NULL){
        if(n->sig->sig != NULL){
            nodo_destruir(l, n->sig->sig);
        }
        nodo_destruir(l, n->sig);
    }
    if(n->ant!=NULL){
        if(n->ant->ant!=NULL){
            nodo_destruir(l, n->ant->ant);
        }
        nodo_destruir(l, n->ant);
    }

}

void bolas_retroceder(nodo_t *n){

    nodo_t *actual = n;
    float x, y;
    coordenadas_devolver(actual->dato, &x, &y);

    while(actual->sig != NULL){

        cambiar_coordenadas(actual->sig->dato, x, y);
        actual = actual->sig;

    }

}

void mover_bola_bezier(lista_t *l, bezier_t *curvanivel, size_t velocidad){

    nodo_t *actual = l->prim;

    size_t pixeles_avanzar = velocidad / JUEGO_FPS;

    double t_inicial = devolver_te(actual->dato);
    double t_final = bezier_avanzar(curvanivel, t_inicial, pixeles_avanzar);

    double xx, yy;
    bezier_evaluar(curvanivel, t_final, &xx, &yy);

    cambiar_coordenadas(actual->dato, (float)xx, (float)yy);
    cambiar_te(actual->dato, t_final);

    bolas_empujar(l, curvanivel);

}

nodo_t *ultima_bola(lista_t *l){
    nodo_t *actual = l->prim;
    while(actual->sig != NULL){
        actual = actual->sig;
    }
    return actual;
}

ball_t **lista_a_vector(lista_t *l, size_t *n){
    *n = 0;    
    if(lista_es_vacia(l)){
        return NULL;  
    }

    ball_t **vectorbolas = NULL;

     
    nodo_t *actual = l->prim;
    while(actual->sig != NULL){
        ball_t **aux = realloc(vectorbolas, sizeof(ball_t *) * ++(*n));
        vectorbolas = aux;
        vectorbolas[(*n)-1] = actual->dato;
        actual = actual->sig;
    }
    
    ball_t **aux = realloc(vectorbolas, sizeof(ball_t *) * ++(*n));
    vectorbolas = aux;
    vectorbolas[(*n)-1] = actual->dato; //no se puede hacer memcpy porque no sabemos sizeof ball_t

    return vectorbolas;
}

void imprimir_lista_adelante(lista_t *l){
    nodo_t *actual = l->prim;
    int i = 1;

    if(lista_es_vacia(l)){
        printf("vacia\n");
        return;
    }

    while(actual->sig != NULL){
        printf("color%d = %d\n", i++,  devolver_color(actual->dato));
        actual = actual->sig;
    }
    printf("color%d = %d\n", i++,  devolver_color(actual->dato));

}

void imprimir_lista_atras(lista_t *l){
    nodo_t *actual = l->prim;
    int i = 1;

    
    if(lista_es_vacia(l)){
        printf("vacia\n");
        return;
    }

    while(actual->sig != NULL){
        actual = actual->sig;
    }

    while(actual->ant != NULL){
        printf("color%d = %d\n", i++,  devolver_color(actual->dato));
        actual = actual->ant;
    }

    printf("color%d = %d\n", i++,  devolver_color(actual->dato));

}