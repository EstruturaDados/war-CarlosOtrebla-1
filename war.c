// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO
// ============================================================================
//
// ============================================================================
//
// OBJETIVOS:
// - Modularizar completamente o código em funções especializadas.
// - Implementar um sistema de missões para um jogador.
// - Criar uma função para verificar se a missão foi cumprida.
// - Utilizar passagem por referência (ponteiros) para modificar dados e
//   passagem por valor/referência constante (const) para apenas ler.
// - Foco em: Design de software, modularização, const correctness, lógica de
// jogo.
//
// ============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>

#include "estruturas.h"
#include "interface.h"
#include "jogo.h"

int main() {
  SetConsoleOutputCP(CP_UTF8);
  SetConsoleCP(CP_UTF8);
  srand(time(NULL));

  printf("\nBem-vindo ao War Estruturado! \n");

  Territorio *mapa = alocarMapa();
  if (mapa == NULL) {
    printf("\x1b[31mErro ao alocar memória para o mapa.\x1b[0m\n");
    return 1;
  }

  int contTerritorios = 0;
  int sairPrograma = 0;

  while (!sairPrograma) {
    int opcaoMenu = inicializarTerritorios(mapa, &contTerritorios);

    if (opcaoMenu == 0) {
      sairPrograma = 1;
      continue;
    }

    if (contTerritorios < 2) {
      continue;
    }

    char corJogador[20];
    printf("\nPara qual Exército (Cor) você vai jogar? ");
    fgets(corJogador, sizeof(corJogador), stdin);
    corJogador[strcspn(corJogador, "\n")] = 0;

    Missao missao = sortearMissao();
    printf("\n\x1b[32mSua missão secreta:\x1b[0m %s\n", missao.descricao);

    int opcaoJogo = 0;
    do {
      exibirMapa(mapa, contTerritorios);

      printf("\n════════════════ O QUE DESEJA FAZER? ════════════════\n");
      printf(" 1 - Atacar um território\n");
      printf(" 2 - Verificar se venci\n");
      printf(" 3 - Voltar ao Menu Principal\n");
      printf(" 0 - Sair do jogo\n");
      printf(" ╚══> Escolha: ");

      if (!lerInteiro(&opcaoJogo)) {
        continue;
      }

      switch (opcaoJogo) {
      case 1:
        faseDeAtaque(mapa, contTerritorios, corJogador);
        break;
      case 2:
        if (verificarVitoria(mapa, contTerritorios, missao, corJogador)) {
          printf("\n\x1b[32m==================================================="
                 "==\x1b[0m\n");
          printf("\x1b[32m  PARABÉNS! VOCÊ CUMPRIU SUA MISSÃO E VENCEU O JOGO! "
                 "\x1b[0m\n");
          printf("\x1b[32m====================================================="
                 "\x1b[0m\n");
          opcaoJogo = 3;
        } else {
          printf("\n\x1b[33mAinda não... Continue conquistando!\x1b[0m\n");
        }
        break;
      case 3:
        printf("\nVoltando ao Menu Principal...\n");
        break;
      case 0:
        printf("\nSaindo...\n");
        sairPrograma = 1;
        break;
      default:
        printf("\nOpção inválida!\n");
      }
    } while (opcaoJogo != 0 && opcaoJogo != 3);
  }

  liberarMemoria(&mapa);

  return 0;
}
