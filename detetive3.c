#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NOME 64
#define MAX_PISTA 128
#define HASH_SIZE 101   

typedef struct Sala {
    char nome[MAX_NOME];
    char pista[MAX_PISTA]; 
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;
typedef struct PistaNode {
    char pista[MAX_PISTA];
    struct PistaNode *esquerda;
    struct PistaNode *direita;
} PistaNode;

typedef struct HashEntry {
    char pista[MAX_PISTA];         
    char suspeito[MAX_NOME];       
    struct HashEntry *proximo;
} HashEntry;

typedef struct HashTable {
    HashEntry* buckets[HASH_SIZE];
} HashTable;

Sala* criarSala(const char* nome, const char* pista);

void explorarSalas(Sala* atual, PistaNode** arvorePistas, HashTable* hash);

PistaNode* inserirPista(PistaNode* raiz, const char* pista);

int existePista(PistaNode* raiz, const char* pista);

void inserirNaHash(HashTable* ht, const char* pista, const char* suspeito);

const char* encontrarSuspeito(HashTable* ht, const char* pista);

void exibirPistas(PistaNode* raiz);

int contarPistasParaSuspeito(PistaNode* raiz, HashTable* ht, const char* suspeito);

void verificarSuspeitoFinal(PistaNode* arvorePistas, HashTable* ht, const char* acusado);

unsigned long hash_djb2(const char *str);
HashTable* criarHashTable();
void liberarHashTable(HashTable* ht);
void liberarPistas(PistaNode* raiz);
void liberarSalas(Sala* raiz); 


Sala* criarSala(const char* nome, const char* pista) {
    Sala* s = (Sala*) malloc(sizeof(Sala));
    if (!s) {
        fprintf(stderr, "Erro de alocação ao criar sala.\n");
        exit(1);
    }
    strncpy(s->nome, nome, MAX_NOME-1);
    s->nome[MAX_NOME-1] = '\0';
    if (pista != NULL) {
        strncpy(s->pista, pista, MAX_PISTA-1);
        s->pista[MAX_PISTA-1] = '\0';
    } else {
        s->pista[0] = '\0';
    }
    s->esquerda = s->direita = NULL;
    return s;
}

PistaNode* criarPistaNode(const char* pista) {
    PistaNode* n = (PistaNode*) malloc(sizeof(PistaNode));
    if (!n) {
        fprintf(stderr, "Erro de alocação ao criar nó de pista.\n");
        exit(1);
    }
    strncpy(n->pista, pista, MAX_PISTA-1);
    n->pista[MAX_PISTA-1] = '\0';
    n->esquerda = n->direita = NULL;
    return n;
}

int existePista(PistaNode* raiz, const char* pista) {
    if (raiz == NULL) return 0;
    int cmp = strcmp(pista, raiz->pista);
    if (cmp == 0) return 1;
    if (cmp < 0) return existePista(raiz->esquerda, pista);
    else return existePista(raiz->direita, pista);
}


PistaNode* inserirPista(PistaNode* raiz, const char* pista) {
    if (raiz == NULL) {
        return criarPistaNode(pista);
    }
    int cmp = strcmp(pista, raiz->pista);
    if (cmp < 0) {
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    } else if (cmp > 0) {
        raiz->direita = inserirPista(raiz->direita, pista);
    } 
    return raiz;
}

void exibirPistas(PistaNode* raiz) {
    if (raiz == NULL) return;
    exibirPistas(raiz->esquerda);
    printf("- %s\n", raiz->pista);
    exibirPistas(raiz->direita);
}

/* Hash: djb2 */
unsigned long hash_djb2(const char *str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + (unsigned char)c; 
    return hash;
}

HashTable* criarHashTable() {
    HashTable* ht = (HashTable*) malloc(sizeof(HashTable));
    if (!ht) {
        fprintf(stderr, "Erro ao criar tabela hash.\n");
        exit(1);
    }
    for (int i = 0; i < HASH_SIZE; ++i) ht->buckets[i] = NULL;
    return ht;
}

void inserirNaHash(HashTable* ht, const char* pista, const char* suspeito) {
    unsigned long h = hash_djb2(pista) % HASH_SIZE;
    HashEntry* cur = ht->buckets[h];
    while (cur != NULL) {
        if (strcmp(cur->pista, pista) == 0) {
           
            strncpy(cur->suspeito, suspeito, MAX_NOME-1);
            cur->suspeito[MAX_NOME-1] = '\0';
            return;
        }
        cur = cur->proximo;
    }

    HashEntry* novo = (HashEntry*) malloc(sizeof(HashEntry));
    if (!novo) {
        fprintf(stderr, "Erro de alocação na hash.\n");
        exit(1);
    }
    strncpy(novo->pista, pista, MAX_PISTA-1);
    novo->pista[MAX_PISTA-1] = '\0';
    strncpy(novo->suspeito, suspeito, MAX_NOME-1);
    novo->suspeito[MAX_NOME-1] = '\0';
    novo->proximo = ht->buckets[h];
    ht->buckets[h] = novo;
}

const char* encontrarSuspeito(HashTable* ht, const char* pista) {
    unsigned long h = hash_djb2(pista) % HASH_SIZE;
    HashEntry* cur = ht->buckets[h];
    while (cur != NULL) {
        if (strcmp(cur->pista, pista) == 0) {
            return cur->suspeito;
        }
        cur = cur->proximo;
    }
    return NULL;
}

void explorarSalas(Sala* atual, PistaNode** arvorePistas, HashTable* hash) {
    char escolha;
    while (1) {
        printf("\nVocê está em: %s\n", atual->nome);

        if (strlen(atual->pista) > 0) {
            printf("Pista encontrada: \"%s\"\n", atual->pista);
            if (!existePista(*arvorePistas, atual->pista)) {
                *arvorePistas = inserirPista(*arvorePistas, atual->pista);
                printf("Pista adicionada ao caderno.\n");
            } else {
                printf("Você já havia coletado essa pista antes.\n");
            }

        } else {
            printf("Nenhuma pista nesta sala.\n");
        }

        printf("\nEscolha um caminho:\n");
        if (atual->esquerda) printf("  (e) Ir para %s\n", atual->esquerda->nome);
        if (atual->direita) printf("  (d) Ir para %s\n", atual->direita->nome);
        printf("  (s) Sair da exploração\n");
        printf("Opção: ");
        if (scanf(" %c", &escolha) != 1) {
         
            int c; while ((c = getchar()) != '\n' && c != EOF);
            printf("Entrada inválida. Tente novamente.\n");
            continue;
        }

        if (escolha == 'e' && atual->esquerda) {
            atual = atual->esquerda;
        } else if (escolha == 'd' && atual->direita) {
            atual = atual->direita;
        } else if (escolha == 's') {
            printf("Exploração encerrada pelo jogador.\n");
            break;
        } else {
            printf("Opção inválida ou caminho inexistente.\n");
        }
    }
}

int contarPistasParaSuspeito_rec(PistaNode* raiz, HashTable* ht, const char* suspeito) {
    if (raiz == NULL) return 0;
    int count = 0;
    count += contarPistasParaSuspeito_rec(raiz->esquerda, ht, suspeito);
    const char* s = encontrarSuspeito(ht, raiz->pista);
    if (s != NULL && strcmp(s, suspeito) == 0) count++;
    count += contarPistasParaSuspeito_rec(raiz->direita, ht, suspeito);
    return count;
}

int contarPistasParaSuspeito(PistaNode* raiz, HashTable* ht, const char* suspeito) {
    return contarPistasParaSuspeito_rec(raiz, ht, suspeito);
}

void verificarSuspeitoFinal(PistaNode* arvorePistas, HashTable* ht, const char* acusado) {
    int total = contarPistasParaSuspeito(arvorePistas, ht, acusado);
    printf("\nVocê acusou: %s\n", acusado);
    printf("Pistas que apontam para %s: %d\n", acusado, total);
    if (total >= 2) {
        printf("Veredito: Há evidências suficientes. %s é o culpado!\n", acusado);
    } else {
        printf("Veredito: Evidências insuficientes para condenar %s. Continue investigando.\n", acusado);
    }
}

void liberarPistas(PistaNode* raiz) {
    if (raiz == NULL) return;
    liberarPistas(raiz->esquerda);
    liberarPistas(raiz->direita);
    free(raiz);
}

void liberarHashTable(HashTable* ht) {
    if (!ht) return;
    for (int i = 0; i < HASH_SIZE; ++i) {
        HashEntry* cur = ht->buckets[i];
        while (cur) {
            HashEntry* prox = cur->proximo;
            free(cur);
            cur = prox;
        }
    }
    free(ht);
}

void liberarSalas(Sala* raiz) {
    if (raiz == NULL) return;
    liberarSalas(raiz->esquerda);
    liberarSalas(raiz->direita);
    free(raiz);
}


int main() {

    Sala* hall = criarSala("Hall de Entrada", "Pegada de sapato molhado");
    Sala* salaEstar = criarSala("Sala de Estar", "Relógio parado às 10h15");
    Sala* cozinha = criarSala("Cozinha", "Copo quebrado no chão");
    Sala* biblioteca = criarSala("Biblioteca", "Livro aberto na página 42");
    Sala* jardim = criarSala("Jardim", "Fios de seda rasgados");
    Sala* escritorio = criarSala("Escritório", "Carta rasgada sobre a mesa");
    Sala* porao = criarSala("Porão", "Chave antiga enferrujada");
    Sala* quarto = criarSala("Quarto", "Maquiagem com marcas vermelhas");


    hall->esquerda = salaEstar;
    hall->direita = cozinha;

    salaEstar->esquerda = biblioteca;
    salaEstar->direita = jardim;

    cozinha->esquerda = escritorio;
    cozinha->direita = porao;

    biblioteca->esquerda = quarto; 

    PistaNode* arvorePistas = NULL;
    HashTable* ht = criarHashTable();

    inserirNaHash(ht, "Pegada de sapato molhado", "Sr. Verde");
    inserirNaHash(ht, "Relógio parado às 10h15", "Sra. Rosa");
    inserirNaHash(ht, "Copo quebrado no chão", "Sr. Verde");
    inserirNaHash(ht, "Livro aberto na página 42", "Prof. Azul");
    inserirNaHash(ht, "Fios de seda rasgados", "Sra. Rosa");
    inserirNaHash(ht, "Carta rasgada sobre a mesa", "Prof. Azul");
    inserirNaHash(ht, "Chave antiga enferrujada", "Sr. Marrom");
    inserirNaHash(ht, "Maquiagem com marcas vermelhas", "Sra. Rosa");

    printf("=== Detective Quest: Julgamento Final ===\n");
    printf("Explore a mansão e colete pistas. Escolha 'e' (esquerda), 'd' (direita) ou 's' (sair).\n");

    explorarSalas(hall, &arvorePistas, ht);

    printf("\n=== Pistas Coletadas (em ordem) ===\n");
    if (arvorePistas == NULL) {
        printf("Nenhuma pista foi coletada.\n");
    } else {
        exibirPistas(arvorePistas);
    }

    char acusado[MAX_NOME];
    printf("\nInforme o nome do suspeito que você deseja acusar (ex: \"Sr. Verde\"): ");
    
    int c;
    while ((c = getchar()) != '\n' && c != EOF); 
    if (fgets(acusado, MAX_NOME, stdin) != NULL) {
        
        size_t ln = strlen(acusado);
        if (ln > 0 && acusado[ln-1] == '\n') acusado[ln-1] = '\0';
        if (strlen(acusado) == 0) {
            printf("Nenhum acusado informado. Encerrando sem julgamento.\n");
        } else {
            verificarSuspeitoFinal(arvorePistas, ht, acusado);
        }
    } else {
        printf("Entrada falhou. Encerrando.\n");
    }

    /* Limpeza de memória */
    liberarPistas(arvorePistas);
    liberarHashTable(ht);
    liberarSalas(hall); 

    printf("\nFim do programa. Obrigado por jogar.\n");
    return 0;
}
