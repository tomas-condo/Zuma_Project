#include <SDL2/SDL.h>

#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#include "diccionariocurvas.h"
#include "config.h"
#include "bezier.h"
#include "dibujar.h"
#include "gameplay.h"
#include "lista.h"
#include "bola.h"

#ifdef TTF
#include <SDL2/SDL_ttf.h>

void escribir_texto(SDL_Renderer *renderer, TTF_Font *font, const char *s, int x, int y) {
    SDL_Color color = {255, 255, 255};  // Estaría bueno si la función recibe un enumerativo con el color, ¿no?
    SDL_Surface *surface = TTF_RenderText_Solid(font, s, color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = surface->w;
    rect.h = surface->h;

    SDL_RenderCopy(renderer, texture, NULL, &rect);

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}
#endif


int main(int argc, char *argv[]) { 
    SDL_Init(SDL_INIT_VIDEO);

#ifdef TTF
    TTF_Init();
    TTF_Font* font = TTF_OpenFont("FreeSansBold.ttf", 24);
    if(font == NULL) {
        fprintf(stderr, "No pudo cargarse la tipografía\n");
        return 1;
    }
#endif

    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event event;

    SDL_CreateWindowAndRenderer(VENTANA_ANCHO, VENTANA_ALTO, 0, &window, &renderer);
    SDL_SetWindowTitle(window, "Rezta");

    int dormir = 0;

    // BEGIN código del alumno
    //SE INICIALIZA EL JUEGO

    nivel_t lista_niveles[] = {
    [NIVEL1] = {"NIVEL1", 241, 248, 40, 0.5},
    [NIVEL2] = {"NIVEL2", 334, 247, 100, 2},
    [NIVEL3] = {"NIVEL3", 305, 337, 100, 2},
    [NIVEL4] = {"NIVEL4", 326, 113, 100, 2},
    };

    static char *coloresnombre[] = {
    [ROJO] = "BOLAROJA",
    [VERDE] = "BOLAVERDE",
    [AZUL] = "BOLAAZUL",
    [AMARILLO] = "BOLAAMARILLA",
    };

    //leer curvas, generamos tdabezier
    FILE *entrada = fopen("20232_tp1.bin", "rb");
    
    contenedor_t *contenedor = contenedor_crear();
    contenedor_llenar(contenedor, entrada);

    char *nombresapo = "SAPO";
    char *nombrecalavera = "CALAVERA";

    char *nombreproxima = "BOLAPROXIMA";

    enum niveles actual = 0; //cambiar para q cambie el nivel
    char *nombrenivel = lista_niveles[actual].curvanombre; 

    int framespasasdas = 0;

    float angulo;
    int coordx = 0, coordy = 0;

    //para insertar la cant de bolas que pide el nivel
    int bolasinsertadas = 0;

    color_t colorant = (rand()%4); //RANDOM 
    color_t proxima = (rand()%4);//boca
    color_t coloraux;
    //color_t colordisparo;


    //para almacenar disparos
    float angulodisparo1;
    ball_t **bolasdisparadas = malloc(sizeof(ball_t *)*100);
    if(bolasdisparadas == NULL) return 1;
    size_t cantdisparos = 0;

    int puntajex = 0;
    int velocidadactual = VELOCIDAD_INICIAL;

    bool perdiste = false;

    //cambia semilla de generacion de randoms
    time_t tiempo;
    srand((unsigned) time(&tiempo));

    lista_t *l = lista_crear();


    //END código del alumno


    unsigned int ticks = SDL_GetTicks();
    while(1) { //por cada frame

        if(SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                break;

            // BEGIN código del alumno
            //cada vez q hay una interaccion con el usuario

            else if(event.type == SDL_MOUSEMOTION) {
                coordx = event.motion.x;
                coordy = event.motion.y;
                angulo = atan2(coordy - lista_niveles[actual].y, coordx - lista_niveles[actual].x);

            }//devolver
            
            if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {

                angulodisparo1 = angulo;

                ball_t *disparoprueba = bola_crear(proxima, lista_niveles[actual].x + cos(angulodisparo1)*30, lista_niveles[actual].y + sin(angulodisparo1)*30, true);


                if(cantdisparos == 100) cantdisparos = 0;
                bolasdisparadas[cantdisparos++] = disparoprueba;

                angulodisparo1 = angulo;

            } //devolver

            else if(event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT) {
                
                proxima = colorant;
                colorant = (rand()%4);

            } //devolver            

            if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_RIGHT) {

            } //devolver

            else if(event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_RIGHT) {

                coloraux = proxima;
                proxima = colorant;
                colorant = coloraux;

            } //devolver  

            // END código del alumno

            continue;
        }
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0x00);

        // BEGIN código del alumno
        //lo que se va a tener que recargar siempre

        framespasasdas++;
        if(((framespasasdas/JUEGO_FPS) >= lista_niveles[actual].segsinicial) && !perdiste) velocidadactual = VELOCIDAD_JUEGO;



#ifdef TTF
        escribir_texto(renderer, font, "Rezta", 100, 20);
        char aux[100];
        sprintf(aux, "%03d, %03d", coordx, coordy);
        escribir_texto(renderer, font, aux, VENTANA_ANCHO - 100, VENTANA_ALTO - 34);
        char auxpuntaje[100];
        sprintf(auxpuntaje, "puntaje : %03d", puntajex);
        escribir_texto(renderer, font, auxpuntaje, 70, 440);
        if(perdiste) escribir_texto(renderer, font, "PERDISTE", 320, 240);
        if(((lista_es_vacia(l) && (bolasinsertadas == lista_niveles[actual].cant))) && (actual == 3)){
            char auxganaste[100];
            sprintf(auxganaste, "GANASTE : %03d", puntajex);
            escribir_texto(renderer, font, auxpuntaje, 320, 240);            
        }  

#endif

        bezier_t *clonsapo = bezier_clonar(bezier_buscar_contenedor(contenedor, nombresapo));
        bezier_t *clonnivel = bezier_clonar(bezier_buscar_contenedor(contenedor, nombrenivel));
        bezier_t *cloncalavera = bezier_clonar(bezier_buscar_contenedor(contenedor, nombrecalavera));
        bezier_t *clonanterior = bezier_clonar(bezier_buscar_contenedor(contenedor, nombreproxima));
        bezier_t *clonbocasapo = bezier_clonar(bezier_buscar_contenedor(contenedor, coloresnombre[proxima]));

        //coordenadas para la calavera
        double cax, cay;
        bezier_evaluar(clonnivel, bezier_cantidad_tramos(clonnivel), &cax, &cay);

        //nivel
        dibujar_bezier(clonnivel, 0, 0, 0, renderer);
        //calavera
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0x00, 0x00);
        dibujar_bezier(cloncalavera, (float)cax, (float)cay, 0, renderer);
        //sapo
        SDL_SetRenderDrawColor(renderer, 0x72, 0xB0, 0x74, 0x00); //#72b074
        dibujar_bezier(clonsapo, lista_niveles[actual].x, lista_niveles[actual].y, angulo, renderer);
        //bolalomo
        mysetcolor(renderer, colorant);
        dibujar_bezier(clonanterior, lista_niveles[actual].x, lista_niveles[actual].y, angulo, renderer);
        //bolaboca
        mysetcolor(renderer, proxima);
        trasladar(clonbocasapo, 30, 0);
        dibujar_bezier(clonbocasapo, lista_niveles[actual].x, lista_niveles[actual].y, angulo, renderer);


        //DISPAROS
        for(size_t d = 0; d < cantdisparos; d++){

            nodo_t *ins = bola_buscar(l, bolasdisparadas[d]);

            
            //para insertar a  la lista
            if(ins != NULL){
                puntajex += bola_insertar_disparo(l, ins, bolasdisparadas[d], clonnivel);

                for(size_t h = d; h < (cantdisparos-1); h++){
                    bolasdisparadas[h] = bolasdisparadas[h+1];
                }
                cantdisparos--;

            }
            //si no toca la lista
            else if(ins == NULL){
                dibujar_bola(bolasdisparadas[d], renderer, contenedor);
                if(!mover_bola_linea(bolasdisparadas[d], lista_niveles[actual].x, lista_niveles[actual].y)){
                    for(size_t h = d; h < (cantdisparos-1); h++){
                        bolasdisparadas[h] = bolasdisparadas[h+1];
                    }
                    cantdisparos--;
                };                
            }
        }
        

        bezier_destruir(clonsapo);
        bezier_destruir(cloncalavera);
        bezier_destruir(clonanterior);
        bezier_destruir(clonbocasapo);


        if(bolasinsertadas < lista_niveles[actual].cant){
            if(bola_insertar_comienzo(l, clonnivel)) bolasinsertadas++;

        }
        
        if(!lista_es_vacia(l)) mover_bola_bezier(l, clonnivel, velocidadactual);

        size_t tamanio;
        ball_t **vectorlista = lista_a_vector(l, &tamanio);
        if(vectorlista!=NULL){
            for(size_t k = 0; k < tamanio; k++){
                dibujar_bola(vectorlista[k], renderer, contenedor);
            }
        }

        free(vectorlista);

        if(lista_es_vacia(l) && (bolasinsertadas == lista_niveles[actual].cant)){
            if(actual < 3){
                actual++;
                bolasinsertadas = 0;
                nombrenivel = lista_niveles[actual].curvanombre; 
                framespasasdas = 0;     
            }

        }

        if(bolasinsertadas == lista_niveles[actual].cant){
            if(bezier_cantidad_tramos(clonnivel) == devolver_te(devolver_dato(ultima_bola(l)))){
                velocidadactual = VELOCIDAD_FINAL;
                perdiste = true;                
            }
        }

        bezier_destruir(clonnivel);

        // END código del alumno

        SDL_RenderPresent(renderer);
        ticks = SDL_GetTicks() - ticks;
        if(dormir) {
            SDL_Delay(dormir);
            dormir = 0;
        }
        else if(ticks < 1000 / JUEGO_FPS)
            SDL_Delay(1000 / JUEGO_FPS - ticks);
        else
            printf("Perdiendo cuadros\n");
        ticks = SDL_GetTicks();
    }

    // BEGIN código del alumno

    lista_destruir(l);
    free(bolasdisparadas);
    contenedor_destruir(contenedor);
 
    // END código del alumno

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

#ifdef TTF
    TTF_CloseFont(font);
    TTF_Quit();
#endif
    SDL_Quit();
    return 0;
}