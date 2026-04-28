#ifndef JOGO_H
#define JOGO_H

#include "estruturas.h"

Territorio *alocarMapa();
void liberarMemoria(Territorio **mapa);
Missao sortearMissao();
int verificarVitoria(const Territorio *mapa, int contTerritorios, Missao missao,
                     const char *corJogador);
void faseDeAtaque(Territorio *mapa, int contTerritorios,
                  const char *corJogador);
void simularAtaque(Territorio *atacante, Territorio *defensor);

#endif
