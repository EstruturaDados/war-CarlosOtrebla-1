#include <stdio.h>
#include <string.h>
#include "interface.h"

int exibirMenuPrincipal(int contTerritorios) {
  int opcao = 0;
  printf("\n══════════════════════════════════════════════════════════\n");
  printf("               M E N U   P R I N C I P A L               \n");
  printf("══════════════════════════════════════════════════════════\n");
  printf(" 1 - Cadastrar território (%d/%d)            \n", contTerritorios,
         MAX_TERRITORIOS);
  printf(" 2 - Listar territórios                          \n");
  printf(" 3 - Iniciar jogo               \n");
  printf(" 0 - Sair                       \n");
  printf(" ║   \n");
  printf(" ╚══> Escolha uma opção acima: ");

  if (scanf("%d", &opcao) != 1) {
    opcao = -1;
  }
  while (getchar() != '\n')
    ;
  return opcao;
}

int inicializarTerritorios(Territorio *mapa, int *contTerritorios) {
  int opcao = 0;
  do {
    opcao = exibirMenuPrincipal(*contTerritorios);

    switch (opcao) {
    case 1:
      if (*contTerritorios < MAX_TERRITORIOS) {
        int territoriosDisponiveis = MAX_TERRITORIOS - *contTerritorios;
        int qtdParaCadastrar = 0;

        printf("\nTerritórios disponíveis: %d/%d\n", territoriosDisponiveis,
               MAX_TERRITORIOS);
        printf("Quantos territórios deseja cadastrar agora? ");
        if (scanf("%d", &qtdParaCadastrar) != 1)
          qtdParaCadastrar = 0;
        while (getchar() != '\n')
          ;

        if (qtdParaCadastrar <= 0 ||
            qtdParaCadastrar > territoriosDisponiveis) {
          printf("\x1b[33m[AVISO] Quantidade inválida! Digite um valor entre 1 "
                 "e %d.\x1b[0m\n",
                 territoriosDisponiveis);
          break;
        }

        int limite = *contTerritorios + qtdParaCadastrar;
        int contBatch = 1;
        while (*contTerritorios < limite) {
          printf("\n--- Cadastrando Territórios %d de %d ---\n", contBatch,
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
              printf("\x1b[33m[AVISO] O território \"%s\" já existe! Digite "
                     "outro nome.\x1b[0m\n",
                     mapa[*contTerritorios].nome);
            }
          } while (nomeRepetido);

          printf("Cor do exército: ");
          fgets(mapa[*contTerritorios].corExercito,
                sizeof(mapa[*contTerritorios].corExercito), stdin);
          mapa[*contTerritorios]
              .corExercito[strcspn(mapa[*contTerritorios].corExercito, "\n")] =
              0;

          printf("Número de tropas: ");
          if (scanf("%d", &mapa[*contTerritorios].numTropas) != 1)
            mapa[*contTerritorios].numTropas = 1;
          while (getchar() != '\n')
            ;

          (*contTerritorios)++;
          contBatch++;
          printf("Território cadastrado com sucesso!\n");
        }
        printf("\nCadastro concluído! Total de territórios: %d/%d\n",
               *contTerritorios, MAX_TERRITORIOS);
        exibirMapa(mapa, *contTerritorios);
      } else {
        printf("\n\x1b[33m[AVISO] Limite de %d territórios atingido!\x1b[0m\n",
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
      printf("\nOpção inválida!\n");
      break;
    }
  } while (opcao != 0 && opcao != 3);
  return opcao;
}

void exibirMapa(const Territorio *mapa, int contTerritorios) {
  if (contTerritorios == 0) {
    printf("\n\x1b[33m[AVISO] Nenhum território cadastrado ainda.\x1b[0m\n");
    return;
  }
  printf("\n══════════════ M A P A   D O   M U N D O ══════════════\n");
  printf("%-5s | %-20s | ", "ID", "Nome");
  imprimirStringUTF8Alinhada("Exército", 15);
  printf(" | %-10s\n", "Tropas");
  printf("══════════════════════════════════════════════════════════\n");
  for (int i = 0; i < contTerritorios; i++) {
    printf("%-5d | ", i + 1);
    imprimirStringUTF8Alinhada(mapa[i].nome, 20);
    printf(" | ");
    imprimirStringUTF8Alinhada(mapa[i].corExercito, 15);
    printf(" | %-10d\n", mapa[i].numTropas);
  }
  printf("══════════════════════════════════════════════════════════\n");
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
