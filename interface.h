#ifndef INTERFACE_H
#define INTERFACE_H

#include "estruturas.h"

void imprimirStringUTF8Alinhada(const char *str, int largura);
int exibirMenuPrincipal(int contTerritorios);
int inicializarTerritorios(Territorio *mapa, int *contTerritorios);
void exibirMapa(const Territorio *mapa, int contTerritorios);
int lerInteiro(int *destino);

#endif
