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

//------- Constantes globais---------
const int MAX_TERRITORIOS = 5;

//-------- Structs --------
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

//-----------Protótipos----------
void imprimirStringUTF8Alinhada(const char *str, int largura);
Territorio *alocarMapa(); 
void liberarMemoria(Territorio *mapa);
void inicializarTerritorios(Territorio *mapa, int *contTerritorios);
void exibirMapa(const Territorio *mapa, int contTerritorios);
int exibirMenuPrincipal(int contTerritorios);
Missao sortearMissao();
int verificarVitoria(const Territorio *mapa, int contTerritorios,
                     Missao missao);
void faseDeAtaque(Territorio *mapa, int contTerritorios);
void simularAtaque(Territorio *atacante, Territorio *defensor);

int main() {
  SetConsoleOutputCP(CP_UTF8);
  SetConsoleCP(CP_UTF8);
  srand(time(NULL));

  printf("\nBem-vindo ao War Estruturado! \n");

  Territorio *mapa = alocarMapa();
  if (mapa == NULL) {
    printf("\x1b[31mErro ao alocar memoria para o mapa.\x1b[0m\n");
    return 1;
  }

  int contTerritorios = 0;

  inicializarTerritorios(mapa, &contTerritorios);

  if (contTerritorios < 2) {
    liberarMemoria(mapa);
    return 0;
  }

  Missao missao = sortearMissao();
  printf("\n\x1b[32mSua missao secreta:\x1b[0m %s\n", missao.descricao);

  int opcaoJogo = 0;
  do {
    exibirMapa(mapa, contTerritorios);

    printf("\n════════════════ O QUE DESEJA FAZER? ════════════════\n");
    printf(" 1 - Atacar um territorio\n");
    printf(" 2 - Verificar se venci\n");
    printf(" 0 - Sair do jogo\n");
    printf(" ╚══> Escolha: ");

    if (scanf("%d", &opcaoJogo) != 1) {
      while (getchar() != '\n')
        ;
      continue;
    }
    while (getchar() != '\n')
      ;

    switch (opcaoJogo) {
    case 1:
      faseDeAtaque(mapa, contTerritorios);
      break;
    case 2:
      if (verificarVitoria(mapa, contTerritorios, missao)) {
        printf("\n\x1b[32m====================================================="
               "\x1b[0m\n");
        printf("\x1b[32m  PARABENS! VOCE CUMPRIU SUA MISSAO E VENCEU O JOGO! "
               "\x1b[0m\n");
        printf("\x1b[32m====================================================="
               "\x1b[0m\n");
        opcaoJogo = 0;
      } else {
        printf("\n\x1b[33mAinda nao... Continue conquistando!\x1b[0m\n");
      }
      break;
    case 0:
      printf("\nFim de jogo.\n");
      break;
    default:
      printf("\nOpcao invalida!\n");
    }
  } while (opcaoJogo != 0);

  liberarMemoria(mapa);

  return 0;
}

// --- Implementação das Funções ---

Territorio *alocarMapa() {
  Territorio *mapa = (Territorio *)calloc(MAX_TERRITORIOS, sizeof(Territorio));
  return mapa;
}

void liberarMemoria(Territorio *mapa) { free(mapa); }

int exibirMenuPrincipal(int contTerritorios) {
  int opcao = 0;
  printf("\n══════════════════════════════════════════════════════════\n");
  printf("               M E N U   P R I N C I P A L               \n");
  printf("══════════════════════════════════════════════════════════\n");
  printf(" 1 - Cadastrar territorio (%d/%d)            \n", contTerritorios,
         MAX_TERRITORIOS);
  printf(" 2 - Listar territorios                          \n");
  printf(" 3 - Iniciar jogo               \n");
  printf(" 0 - Sair                       \n");
  printf(" ║   \n");
  printf(" ╚══> Escolha uma opcao acima: ");

  if (scanf("%d", &opcao) != 1) {
    opcao = -1;
  }
  while (getchar() != '\n')
    ;
  return opcao;
}

void inicializarTerritorios(Territorio *mapa, int *contTerritorios) {
  int opcao = 0;
  do {
    opcao = exibirMenuPrincipal(*contTerritorios);

    switch (opcao) {
    case 1:
      if (*contTerritorios < MAX_TERRITORIOS) {
        int territoriosDisponiveis = MAX_TERRITORIOS - *contTerritorios;
        int qtdParaCadastrar = 0;

        printf("\nTerritorios disponiveis: %d/%d\n", territoriosDisponiveis,
               MAX_TERRITORIOS);
        printf("Quantos territorios deseja cadastrar agora? ");
        if (scanf("%d", &qtdParaCadastrar) != 1)
          qtdParaCadastrar = 0;
        while (getchar() != '\n')
          ;

        if (qtdParaCadastrar <= 0 ||
            qtdParaCadastrar > territoriosDisponiveis) {
          printf("\x1b[33m[AVISO] Quantidade invalida! Digite um valor entre 1 "
                 "e %d.\x1b[0m\n",
                 territoriosDisponiveis);
          break;
        }

        int limite = *contTerritorios + qtdParaCadastrar;
        int contBatch = 1;
        while (*contTerritorios < limite) {
          printf("\n--- Cadastrando Territorios %d de %d ---\n", contBatch,
                 qtdParaCadastrar);

          int nomeRepetido;
          do {
            nomeRepetido = 0;
            printf("Nome: ");
            fgets(mapa[*contTerritorios].nome,
                  sizeof(mapa[*contTerritorios].nome), stdin);
            mapa[*contTerritorios]
                .nome[strcspn(mapa[*contTerritorios].nome, "\n")] = 0;

            for (int i = 0; i < *contTerritorios; i++) {
              if (strcmp(mapa[i].nome, mapa[*contTerritorios].nome) == 0) {
                nomeRepetido = 1;
                break;
              }
            }
            if (nomeRepetido) {
              printf("\x1b[33m[AVISO] O territorio \"%s\" já existe! Digite "
                     "outro nome.\x1b[0m\n",
                     mapa[*contTerritorios].nome);
            }
          } while (nomeRepetido);

          printf("Cor do exercito: ");
          fgets(mapa[*contTerritorios].corExercito,
                sizeof(mapa[*contTerritorios].corExercito), stdin);
          mapa[*contTerritorios]
              .corExercito[strcspn(mapa[*contTerritorios].corExercito, "\n")] =
              0;

          printf("Numero de tropas: ");
          if (scanf("%d", &mapa[*contTerritorios].numTropas) != 1)
            mapa[*contTerritorios].numTropas = 1;
          while (getchar() != '\n')
            ;

          (*contTerritorios)++;
          contBatch++;
          printf("Territorio cadastrado com sucesso!\n");
        }
        printf("\nCadastro concluido! Total de territorios: %d/%d\n",
               *contTerritorios, MAX_TERRITORIOS);
      } else {
        printf("\n\x1b[33m[AVISO] Limite de %d territorios atingido!\x1b[0m\n",
               MAX_TERRITORIOS);
      }
      break;

    case 2:
      exibirMapa(mapa, *contTerritorios);
      break;

    case 3:
      if (*contTerritorios < 2) {
        printf("\n\x1b[33m[AVISO] Para iniciar o jogo, é necessário cadastrar "
               "no mínimo 2 territórios!\x1b[0m\n");
        opcao = -1;
      } else {
        printf("\nIniciando jogo...\n");
      }
      break;

    case 0:
      printf("\nSaindo...\n");
      break;

    default:
      printf("\nOpcao invalida!\n");
      break;
    }
  } while (opcao != 0 && opcao != 3);
}

void exibirMapa(const Territorio *mapa, int contTerritorios) {
  if (contTerritorios == 0) {
    printf("\nNenhum territorio cadastrado ainda.\n");
    return;
  }
  printf("\n══════════════════ Lista de Territorios ══════════════════\n");
  for (int i = 0; i < contTerritorios; i++) {
    printf("|%d| Nome: ", i + 1);
    imprimirStringUTF8Alinhada(mapa[i].nome, 15);
    printf(" | Cor: ");
    imprimirStringUTF8Alinhada(mapa[i].corExercito, 12);
    printf(" | Tropas: %2d\n", mapa[i].numTropas);
  }
  printf("══════════════════════════════════════════════════════════\n");
}

Missao sortearMissao() {
  Missao missoes[2] = {
      {1, "Conquistar 1 territorio com pelo menos 5 tropas nele", 5},
      {2, "Acumular um total de 15 tropas espalhadas no mundo", 15}};
  int sorteio = rand() % 2;
  return missoes[sorteio];
}

int verificarVitoria(const Territorio *mapa, int contTerritorios,
                     Missao missao) {
  if (missao.id == 1) {
    // Conquistar 1 territorio com pelo menos 5 tropas
    for (int i = 0; i < contTerritorios; i++) {
      if (mapa[i].numTropas >= missao.quantidadeObjetivo) {
        return 1;
      }
    }
  } else if (missao.id == 2) {
    int somaTropas = 0;
    for (int i = 0; i < contTerritorios; i++) {
      somaTropas += mapa[i].numTropas;
    }
    if (somaTropas >= missao.quantidadeObjetivo) {
      return 1;
    }
  }
  return 0;
}

void faseDeAtaque(Territorio *mapa, int contTerritorios) {
  if (contTerritorios < 2)
    return;

  int idOrigem, idDestino;
  printf("\n--- FASE DE ATAQUE ---\n");
  exibirMapa(mapa, contTerritorios);

  printf("Digite o numero do seu territorio (Origem): ");
  scanf("%d", &idOrigem);
  printf("Digite o numero do territorio alvo (Destino): ");
  scanf("%d", &idDestino);
  while (getchar() != '\n')
    ;

  if (idOrigem < 1 || idOrigem > contTerritorios || idDestino < 1 ||
      idDestino > contTerritorios) {
    printf("\x1b[31m[ERRO] Territorios invalidos!\x1b[0m\n");
    return;
  }
  if (idOrigem == idDestino) {
    printf("\x1b[31m[ERRO] Nao pode atacar a si mesmo!\x1b[0m\n");
    return;
  }

  Territorio *atacante = &mapa[idOrigem - 1];
  Territorio *defensor = &mapa[idDestino - 1];

  if (atacante->numTropas <= 1) {
    printf("\x1b[33m[AVISO] Voce precisa de pelo menos 2 tropas para poder "
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
    defensor->numTropas--;
  } else {
    printf("\x1b[31m>>> Defensor VENCEU a rodada! <<<\x1b[0m\n");
    atacante->numTropas--;
  }

  if (defensor->numTropas <= 0) {
    printf("\n\x1b[32m*** TERRITORIO CONQUISTADO! ***\x1b[0m\n");
    printf("O exercito %s dominou %s!\n", atacante->corExercito,
           defensor->nome);
    strcpy(defensor->corExercito, atacante->corExercito);
    defensor->numTropas = 1;
    atacante->numTropas--;
  }
}

void imprimirStringUTF8Alinhada(const char *str, int largura) {
  int len_visual = 0;
  for (int i = 0; str[i] != '\0'; i++) {
    if ((str[i] & 0xC0) != 0x80) {
      len_visual++;
    }
  }
  printf("%s", str);
  for (int i = len_visual; i < largura; i++) {
    printf(" ");
  }
}
