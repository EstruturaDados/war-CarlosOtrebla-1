#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H

extern const int MAX_TERRITORIOS;

typedef struct territorio {
  char nome[50];
  char corExercito[20];
  int numTropas;
} Territorio;

typedef struct missao {
  int id;
  char descricao[100];
  int quantidadeObjetivo;
} Missao;

#endif
