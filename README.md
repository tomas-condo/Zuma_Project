# Rezta - Clon de Zuma (Trabajo Práctico Final)

![Language](https://img.shields.io/badge/language-C-blue.svg)
![Library](https://img.shields.io/badge/library-SDL2-orange.svg)
![Status](https://img.shields.io/badge/status-Completed-green.svg)

## 📋 Introducción

**Rezta** es una implementación simplificada del clásico juego *Zuma*, desarrollada en lenguaje C utilizando la biblioteca gráfica **SDL2**.

Este proyecto constituye el Trabajo Práctico Integrador de la cursada, enfocándose en el diseño de software, la correcta implementación de Tipos de Datos Abstractos (TDAs), el manejo de memoria dinámica y la separación entre la lógica del juego y la representación gráfica.

## 🚀 Objetivos y Alcance

El objetivo principal es resolver un problema complejo mediante la modularización y la reutilización de código. Los pilares técnicos del proyecto incluyen:

* **Encapsulamiento y Abstracción:** Diseño de TDAs opacos (`bezier`, `lista`, etc.).
* **Estructuras de Datos:** Uso de listas doblemente enlazadas y tablas de búsqueda (diccionarios).
* **Matemática:** Cálculo de curvas de Bézier y movimiento a velocidad constante mediante búsqueda binaria.
* **Archivos:** Carga de niveles y curvas desde archivos binarios/texto.

## 🛠️ Arquitectura y Diseño

El diseño de la aplicación sigue el modelo de separación de responsabilidades (Modelo-Vista), donde la lógica del juego es independiente de SDL2.

### 1. TDA Bézier (Curvas)
Se implementó un TDA capaz de manejar curvas multitramo de grado arbitrario.
* **Primitiva destacada:** `bezier_avanzar`. Dado que el parámetro $t$ en una curva de Bézier no es lineal respecto a la distancia, se implementó un algoritmo de **búsqueda binaria** para calcular el siguiente punto a una distancia constante (velocidad uniforme de las bolas).

### 2. TDA Lista de Bolas
Se utiliza una **lista doblemente enlazada** para gestionar la fila de bolas.
* **Motivo:** Las interacciones del juego (inserción por disparos, eliminación por combos, retrocesos) requieren acceso constante tanto al nodo siguiente como al anterior para detectar agrupaciones de colores y colapsar huecos eficientemente.

### 3. Diccionario de Curvas
Un contenedor que carga y almacena las curvas predefinidas (niveles, trayectorias de disparo) permitiendo su recuperación eficiente mediante un identificador de texto (ej: "NIVEL1").

### 4. Lógica de Juego vs Gráficos
* **Lógica:** Gestiona colisiones, estados de las bolas, puntuación, niveles y combos.
* **Vista (SDL2):** Se encarga exclusivamente de renderizar el estado actual y capturar eventos (mouse/teclado).

## 🎮 Características del Juego

* **Mecánicas:** Disparo de bolas, inserción en la fila, eliminación de 3+ colores iguales.
* **Combos:** Sistema de puntuación multiplicativo ($4^n$) al encadenar eliminaciones y colapsar huecos.
* **Bolas Especiales:** Implementación de bolas con efectos (Pausa, Retroceso o Explosión).
* **Niveles:** Sistema escalable de 4 niveles con dificultad progresiva y curvas distintas.
* **Sapo:** El lanzador rota siguiendo el mouse y permite intercambiar la bola actual con la siguiente.

## ⚙️ Instalación, Compilación y Ejecución

### Requisitos previos
* Compilador GCC o Clang.
* Biblioteca `libsdl2-dev`.
* Herramienta `make`.
* Ejecución `./rezta`.
* Valgrind.

### Compilación
El proyecto incluye un `Makefile` para automatizar la construcción. Ejecute:

```bash
make
