#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "cardapio.h"
#include "fila.h"


typedef struct {
    char nome[50];
    int quantidade;
} IngredienteEstoque;

typedef struct NoIngred {
    char nome[50];
    int totalConsumido;
    struct NoIngred *esq, *dir;
} NoIngred;



void inicializaEstoque(IngredienteEstoque estoque[], int *n) {
    *n = 8;
    strcpy(estoque[0].nome, "Pao");     estoque[0].quantidade = 50; estoque[0].;
    strcpy(estoque[1].nome, "Carne");   estoque[1].quantidade = 40; estoque[1].;
    strcpy(estoque[2].nome, "Bacon");   estoque[2].quantidade = 30; estoque[2].;
    strcpy(estoque[3].nome, "Queijo");  estoque[3].quantidade = 40; estoque[3].;
    strcpy(estoque[4].nome, "Alface");  estoque[4].quantidade = 60; estoque[4].;
    strcpy(estoque[5].nome, "Tomate");  estoque[5].quantidade = 50; estoque[5].;
    strcpy(estoque[6].nome, "Molho");   estoque[6].quantidade = 40; estoque[6].;
    strcpy(estoque[7].nome, "Cebola");  estoque[7].quantidade = 30; estoque[7].;
}

int buscaIngredienteIndex(IngredienteEstoque estoque[], int n, const char *nome) {
    for (int i = 0; i < n; i++) {
        if (strcasecmp(estoque[i].nome, nome) == 0)
            return i;
    }
    return -1;
}

int consumirIngrediente(IngredienteEstoque estoque[], int n, const char *nome, int qtd) {
    int idx = buscaIngredienteIndex(estoque, n, nome);
    if (idx == -1) return 0;
    if (estoque[idx].quantidade < qtd) return 0;
    estoque[idx].quantidade -= qtd;
    return 1;
}



NoIngred* criaNo(const char *nome, int qtd) {
    NoIngred* novo = (NoIngred*) malloc(sizeof(NoIngred));
    strcpy(novo->nome, nome);
    novo->totalConsumido = qtd;
    novo->esq = novo->dir = NULL;
    return novo;
}

NoIngred* inserirNoConsumido(NoIngred *raiz, const char *nome, int qtd) {
    if (raiz == NULL) return criaNo(nome, qtd);
    int cmp = strcasecmp(nome, raiz->nome);
    if (cmp == 0)
        raiz->totalConsumido += qtd;
    else if (cmp < 0)
        raiz->esq = inserirNoConsumido(raiz->esq, nome, qtd);
    else
        raiz->dir = inserirNoConsumido(raiz->dir, nome, qtd);
    return raiz;
}

void imprimirInOrderConsumidos(NoIngred *raiz) {
    if (!raiz) return;
    imprimirInOrderConsumidos(raiz->esq);
    printf("%s : %d\n", raiz->nome, raiz->totalConsumido);
    imprimirInOrderConsumidos(raiz->dir);
}

void liberarArvore(NoIngred *raiz) {
    if (!raiz) return;
    liberarArvore(raiz->esq);
    liberarArvore(raiz->dir);
    free(raiz);
}


void atendimento_cliente(hamburguer *h) {
    printf("\nOii, tudo bem? Eu desejo um %s!\n", h->nome);
}

void montar_hamburguer_manual(hamburguer *h, IngredienteEstoque estoque[], int n, NoIngred **raiz_consumo) {
    atendimento_cliente(h);

    printf("\n-- Montando: %s --\n", h->nome);
    printf("Siga a ordem exata dos ingredientes.\n\n");

    printf("Receita (%d itens):\n", h->qntIngredientes);
    for (int i = 0; i < h->qntIngredientes; i++) {
        printf(" %d) %s\n", i + 1, h->ingredientes[i]);
    }

    for (int i = 0; i < h->qntIngredientes; i++) {
        char entrada[64];
        printf("\nIngrediente %d: ", i + 1);
        scanf(" %63[^\n]", entrada);

        if (strcasecmp(entrada, h->ingredientes[i]) != 0) {
            printf("-> Errado! Esperado: %s\n", h->ingredientes[i]);
        }

        if (!consumirIngrediente(estoque, n, h->ingredientes[i], 1)) {
            printf("-> Falta %s no estoque! Pedido cancelado.\n", h->ingredientes[i]);
            return;
        }

        *raiz_consumo = inserirNoConsumido(*raiz_consumo, h->ingredientes[i], 1);
    }

    printf("\nMontagem concluída!\n");
}



void atendimento_por_dias(hamburguer cardapio[], struct Fila *fila, IngredienteEstoque estoque[], int nEstoque, int dias) {
    NoIngred *raiz_consumo = NULL;
    srand(time(NULL));

    for (int dia = 1; dia <= dias; dia++) {
        printf("\n========== DIA %d ==========\n", dia);

        int novos = rand() % 5 + 1;
        for (int i = 0; i < novos; i++) {
            int idH = rand() % 10;
            enfileirar(fila, rand() % 1000, cardapio[idH].ID);
        }

        struct Pedido *p;
        while ((p = desenfileirar(fila)) != NULL) {
            int idx = p->idHamburguer - 1;
            if (idx < 0 || idx >= 10) {
                free(p);
                continue;
            }

            montar_hamburguer_manual(&cardapio[idx], estoque, nEstoque, &raiz_consumo);
            free(p);
        }

        printf("\nDia %d encerrado. Pressione ENTER para continuar...\n", dia);
        getchar();
    }

    printf("\n=== Ingredientes consumidos ===\n");
    imprimirInOrderConsumidos(raiz_consumo);
    liberarArvore(raiz_consumo);
}

