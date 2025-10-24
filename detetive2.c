#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Sala {
    char nome[50];
    char pista[100];          
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

typedef struct PistaNode {
    char pista[100];
    struct PistaNode *esquerda;
    struct PistaNode *direita;
} PistaNode;

Sala* criarSala(const char* nome, const char* pista) {
    Sala* novaSala = (Sala*) malloc(sizeof(Sala));
    if (novaSala == NULL) {
        printf("Erro ao alocar memória para a sala.\n");
        exit(1);
    }
    strcpy(novaSala->nome, nome);
    strcpy(novaSala->pista, pista);
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    return novaSala;
}

PistaNode* criarPistaNode(const char* pista) {
    PistaNode* novo = (PistaNode*) malloc(sizeof(PistaNode));
    if (novo == NULL) {
        printf("Erro ao alocar memória para pista.\n");
        exit(1);
    }
    strcpy(novo->pista, pista);
    novo->esquerda = NULL;
    novo->direita = NULL;
    return novo;
}

PistaNode* inserirPista(PistaNode* raiz, const char* pista) {
    if (raiz == NULL) {
        return criarPistaNode(pista);
    }
    if (strcmp(pista, raiz->pista) < 0)
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    else if (strcmp(pista, raiz->pista) > 0)
        raiz->direita = inserirPista(raiz->direita, pista);
    // se for igual, ignora (não insere duplicada)
    return raiz;
}

void exibirPistas(PistaNode* raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esquerda);
        printf("- %s\n", raiz->pista);
        exibirPistas(raiz->direita);
    }
}


void explorarSalasComPistas(Sala* atual, PistaNode** arvorePistas) {
    char escolha;

    while (1) {
        printf("\nVocê está em: %s\n", atual->nome);

        // Coleta automática da pista, se houver
        if (strlen(atual->pista) > 0) {
            printf("Você encontrou uma pista: \"%s\"\n", atual->pista);
            *arvorePistas = inserirPista(*arvorePistas, atual->pista);
        } else {
            printf("Nenhuma pista aqui.\n");
        }

        // Mostra opções de navegação
        printf("\nEscolha um caminho:\n");
        if (atual->esquerda != NULL)
            printf("  (e) Ir para %s\n", atual->esquerda->nome);
        if (atual->direita != NULL)
            printf("  (d) Ir para %s\n", atual->direita->nome);
        printf("  (s) Sair da exploração\n");
        printf("Opção: ");
        scanf(" %c", &escolha);

        if (escolha == 'e' && atual->esquerda != NULL) {
            atual = atual->esquerda;
        } else if (escolha == 'd' && atual->direita != NULL) {
            atual = atual->direita;
        } else if (escolha == 's') {
            printf("\nVocê decidiu encerrar a exploração.\n");
            break;
        } else {
            printf("Opção inválida.\n");
        }
    }
}


int main() {
    // Criação das salas com pistas associadas
    Sala* hall = criarSala("Hall de Entrada", "Pegada de sapato molhado");
    Sala* salaEstar = criarSala("Sala de Estar", "Relógio parado às 10h15");
    Sala* cozinha = criarSala("Cozinha", "Copo quebrado no chão");
    Sala* biblioteca = criarSala("Biblioteca", "Livro aberto na página 42");
    Sala* jardim = criarSala("Jardim", "Pegadas que levam à cerca");
    Sala* escritorio = criarSala("Escritório", "Carta rasgada sobre a mesa");
    Sala* porao = criarSala("Porão", "Chave antiga enferrujada");


    hall->esquerda = salaEstar;
    hall->direita = cozinha;
    salaEstar->esquerda = biblioteca;
    salaEstar->direita = jardim;
    cozinha->esquerda = escritorio;
    cozinha->direita = porao;

    // Árvore de pistas inicialmente vazia
    PistaNode* arvorePistas = NULL;

    printf("=== Detective Quest: Coleta de Pistas ===\n");
    explorarSalasComPistas(hall, &arvorePistas);

    // Exibe as pistas coletadas
    printf("\n=== Pistas Coletadas ===\n");
    if (arvorePistas == NULL)
        printf("Nenhuma pista foi coletada.\n");
    else
        exibirPistas(arvorePistas);

    // Libera memória (simplificado)
    free(biblioteca);
    free(jardim);
    free(salaEstar);
    free(escritorio);
    free(porao);
    free(cozinha);
    free(hall);

    printf("\nFim da investigação. Até a próxima, detetive!\n");
    return 0;
}
