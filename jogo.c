#include "jogo.h"
#include "interface.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Territorio *alocarMapa() {
  Territorio *mapa = (Territorio *)calloc(MAX_TERRITORIOS, sizeof(Territorio));
  return mapa;
}

void liberarMemoria(Territorio **mapa) {
  if (*mapa != NULL) {
    free(*mapa);
    *mapa = NULL;
  }
}

Missao sortearMissao() {
  Missao missoes[2] = {
      {1, "Conquistar 1 território com pelo menos 5 tropas nele", 5},
      {2, "Acumular um total de 15 tropas espalhadas no mundo", 15}};
  int sorteio = rand() % 2;
  return missoes[sorteio];
}

int verificarVitoria(const Territorio *mapa, int contTerritorios, Missao missao,
                     const char *corJogador) {
  if (missao.id == 1) {
    // Conquistar 1 territorio com pelo menos 5 tropas
    for (int i = 0; i < contTerritorios; i++) {
      if (strcmp(mapa[i].corExercito, corJogador) == 0 &&
          mapa[i].numTropas >= missao.quantidadeObjetivo) {
        return 1;
      }
    }
  } else if (missao.id == 2) {
    int somaTropas = 0;
    for (int i = 0; i < contTerritorios; i++) {
      if (strcmp(mapa[i].corExercito, corJogador) == 0) {
        somaTropas += mapa[i].numTropas;
      }
    }
    if (somaTropas >= missao.quantidadeObjetivo) {
      return 1;
    }
  }
  return 0;
}

void faseDeAtaque(Territorio *mapa, int contTerritorios,
                  const char *corJogador) {
  if (contTerritorios < 2)
    return;

  int idOrigem = 0, idDestino = 0;
  printf("\n--- FASE DE ATAQUE ---\n");
  exibirMapa(mapa, contTerritorios);
  printf("Digite o número do seu território (Atacante): ");
  lerInteiro(&idOrigem);
  printf("Digite o número do território alvo (Defensor): ");
  lerInteiro(&idDestino);

  if (idOrigem < 1 || idOrigem > contTerritorios || idDestino < 1 ||
      idDestino > contTerritorios) {
    printf("\x1b[31m[ERRO] Territórios inválidos!\x1b[0m\n");
    return;
  }
  if (idOrigem == idDestino) {
    printf("\x1b[31m[ERRO] Não pode atacar a si mesmo!\x1b[0m\n");
    return;
  }

  Territorio *atacante = &mapa[idOrigem - 1];
  Territorio *defensor = &mapa[idDestino - 1];

  if (strcmp(atacante->corExercito, corJogador) != 0) {
    printf("\x1b[31m[ERRO] Você só pode atacar a partir de um território da "
           "sua cor (%s)!\x1b[0m\n",
           corJogador);
    return;
  }

  if (atacante->numTropas <= 1) {
    printf("\x1b[33m[AVISO] Você precisa de pelo menos 2 tropas para poder "
           "atacar!\x1b[0m\n");
    return;
  }

  simularAtaque(atacante, defensor);
}

void simularAtaque(Territorio *atacante, Territorio *defensor) {
  printf("\nBatalha: %s (Atacante) vs %s (Defensor)\n", atacante->nome,
         defensor->nome);
  printf("Rolando dados virtualmente...\n");

  int dadoAtaque = (rand() % 6) + 1;
  int dadoDefesa = (rand() % 6) + 1;

  printf("Atacante tirou: %d\n", dadoAtaque);
  printf("Defensor tirou: %d\n", dadoDefesa);

  if (dadoAtaque > dadoDefesa) {
    printf("\x1b[32m>>> Atacante VENCEU a rodada! <<<\x1b[0m\n");
    printf("\x1b[32m>>> Defensor PERDEU 1 tropa! <<<\x1b[0m\n");
    defensor->numTropas--;
  } else {
    printf("\x1b[31m>>> Defensor VENCEU a rodada! <<<\x1b[0m\n");
    printf("\x1b[31m>>> Atacante PERDEU 1 tropa! <<<\x1b[0m\n");
    atacante->numTropas--;
  }

  if (defensor->numTropas <= 0) {
    printf("\n\x1b[32m*** TERRITÓRIO CONQUISTADO! ***\x1b[0m\n");
    printf("O exército %s dominou %s e moveu 1 tropa para ocupá-lo!\n",
           atacante->corExercito, defensor->nome);
    strncpy(defensor->corExercito, atacante->corExercito,
            sizeof(defensor->corExercito) - 1);
    defensor->corExercito[sizeof(defensor->corExercito) - 1] = '\0';
    defensor->numTropas = 1;
    atacante->numTropas--;
  }
}
