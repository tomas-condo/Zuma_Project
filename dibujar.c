#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h> 

#include "dibujar.h"
#include "bezier.h"
#include "config.h"
#include "bola.h"
#include "gameplay.h"
#include "lista.h"
#include "diccionariocurvas.h"


static char *coloresnombre[] = {
    [ROJO] = "BOLAROJA",
    [VERDE] = "BOLAVERDE",
    [AZUL] = "BOLAAZUL",
    [AMARILLO] = "BOLAAMARILLA",
};

void mysetcolor(SDL_Renderer *renderer, color_t colorset){
    if(colorset == 0) SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0x00);
    else if(colorset == 1) SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0x00);
    else if(colorset == 2) SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0x00);
    else if(colorset == 3) SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0x00, 0x00);
}

void dibujar_bezier(bezier_t *clon, float dx, float dy, double angulo, SDL_Renderer *renderer){

    //size_t tmax = bezier_cantidad_tramos(clon);
    rotar(clon, angulo);
    trasladar(clon, dx, dy);

    for(double t = 0; t <= bezier_cantidad_tramos(clon); t += PASO){
        double x, y;
        bezier_evaluar(clon, t, &x, &y);
        SDL_RenderDrawPoint(renderer, x, y);
    }


}

void dibujar_bola(ball_t *bola, SDL_Renderer *renderer, contenedor_t *cont){
    
    mysetcolor(renderer, devolver_color(bola));

    char *boladibujar;

    if(devolver_normal(bola)){
        boladibujar = coloresnombre[devolver_color(bola)];        
    }

    else{
        boladibujar = "BOLABOMBA";
    }

    bezier_t *clonbezier = bezier_clonar(bezier_buscar_contenedor(cont, boladibujar));
    //ROTAR

    float x, y;
    coordenadas_devolver(bola, &x, &y);

    dibujar_bezier(clonbezier, x, y, 0, renderer);

    bezier_destruir(clonbezier);

}