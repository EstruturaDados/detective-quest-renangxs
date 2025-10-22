#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura que representa uma sala (nó da árvore binária)
typedef struct Sala {
    char nome[50];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// -----------------------------------------------------------------------------
// Função: criarSala
// Cria dinamicamente uma sala com o nome informado.
// -----------------------------------------------------------------------------
Sala* criarSala(const char* nome) {
    Sala* novaSala = (Sala*) malloc(sizeof(Sala));
    if (novaSala == NULL) {
        printf("Erro ao alocar memória.\n");
        exit(1);
    }
    strcpy(novaSala->nome, nome);
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    return novaSala;
}

// -----------------------------------------------------------------------------
// Função: explorarSalas
// Permite a navegação interativa pelas salas da mansão.
// -----------------------------------------------------------------------------
void explorarSalas(Sala* atual) {
    char escolha;

    while (1) {
        printf("\nVocê está em: %s\n", atual->nome);

        // Se não há caminhos, chegou ao fim
        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("Fim da exploração. Não há mais caminhos.\n");
            break;
        }

        // Opções de navegação
        printf("Escolha um caminho:\n");
        if (atual->esquerda != NULL)
            printf("  (e) Ir para %s\n", atual->esquerda->nome);
        if (atual->direita != NULL)
            printf("  (d) Ir para %s\n", atual->direita->nome);
        printf("  (s) Sair\n");
        printf("Opção: ");
        scanf(" %c", &escolha);

        // Movimentação
        if (escolha == 'e' && atual->esquerda != NULL) {
            atual = atual->esquerda;
        } else if (escolha == 'd' && atual->direita != NULL) {
            atual = atual->direita;
        } else if (escolha == 's') {
            printf("Exploração encerrada.\n");
            break;
        } else {
            printf("Opção inválida.\n");
        }
    }
}

// -----------------------------------------------------------------------------
// Função: main
// Monta o mapa da mansão e inicia a exploração.
// -----------------------------------------------------------------------------
int main() {
    // Criação automática das salas
    Sala* hall = criarSala("Hall de Entrada");
    Sala* salaEstar = criarSala("Sala de Estar");
    Sala* cozinha = criarSala("Cozinha");
    Sala* biblioteca = criarSala("Biblioteca");
    Sala* jardim = criarSala("Jardim");
    Sala* escritorio = criarSala("Escritório");
    Sala* porao = criarSala("Porão");

    // Estrutura da árvore binária
    hall->esquerda = salaEstar;
    hall->direita = cozinha;
    salaEstar->esquerda = biblioteca;
    salaEstar->direita = jardim;
    cozinha->esquerda = escritorio;
    cozinha->direita = porao;

    printf("=== Detective Quest: Exploração da Mansão ===\n");
    explorarSalas(hall);

    // Libera memória
    free(biblioteca);
    free(jardim);
    free(salaEstar);
    free(escritorio);
    free(porao);
    free(cozinha);
    free(hall);

    return 0;
}
