#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "cardapio.h"
#include "fila.h"
#include "ingredientes.h" 


typedef struct NoIngred {
    char nome[50];
    int totalConsumido;
    struct NoIngred *prox;
} NoIngred;

NoIngred* criaNo(const char *nome, int qtd) {
    NoIngred* novo = (NoIngred*) malloc(sizeof(NoIngred));
    strcpy(novo->nome, nome);
    novo->totalConsumido = qtd;
    novo->prox = NULL;
    return novo;
}

void inserirConsumido(NoIngred **lista, const char *nome, int qtd) {
    NoIngred *atual = *lista;
    NoIngred *anterior = NULL;

    while (atual != NULL) {
        if (strcasecmp(atual->nome, nome) == 0) {
            atual->totalConsumido += qtd;
            return;
        }
        anterior = atual;
        atual = atual->prox;
    }

    NoIngred *novo = criaNo(nome, qtd);
    if (anterior == NULL) {
        *lista = novo;
    } else {
        anterior->prox = novo;
    }
}

void imprimirConsumidos(NoIngred *lista) {
    NoIngred *atual = lista;
    while (atual != NULL) {
        printf("%s : %d\n", atual->nome, atual->totalConsumido);
        atual = atual->prox;
    }
}

void liberarLista(NoIngred *lista) {
    NoIngred *atual = lista;
    while (atual != NULL) {
        NoIngred *temp = atual;
        atual = atual->prox;
        free(temp);
    }
}



int buscaIngredienteIndex(const char *nome) {
    for (int i = 0; i < numIngredientes; i++) {
        if (strcasecmp(ingredientes[i].nome, nome) == 0)
            return i;
    }
    return -1;
}

int consumirIngrediente(const char *nome, int qtd) {
    int idx = buscaIngredienteIndex(nome);
    if (idx == -1) return 0;
    if (ingredientes[idx].quantidade < qtd) return 0;
    ingredientes[idx].quantidade -= qtd;
    return 1;
}



void atendimento_cliente(hamburguer *h) {
    printf("\noii, tudo bem? Eu desejo um %s!\n", h->nome);
}

void montar_hamburguer_manual(hamburguer *h, NoIngred **lista_consumo) {
    atendimento_cliente(h);

    printf("\n-- Montando: %s --\n", h->nome);
    printf("siga a ordem exata dos ingredientes.\n\n");

    printf("receita (%d itens):\n", h->qntIngredientes);
    for (int i = 0; i < h->qntIngredientes; i++) {
        printf(" %d) %s\n", i + 1, h->ingredientes[i]);
    }

    for (int i = 0; i < h->qntIngredientes; i++) {
        char entrada[64];
        printf("\ningrediente %d: ", i + 1);
        scanf(" %63[^\n]", entrada);

        if (strcasecmp(entrada, h->ingredientes[i]) != 0) {
            printf("-> errado! esperado: %s\n", h->ingredientes[i]);
        }

        if (!consumirIngrediente(h->ingredientes[i], 1)) {
            printf("-> falta %s no estoque! pedido cancelado.\n", h->ingredientes[i]);
            return;
        }

        inserirConsumido(lista_consumo, h->ingredientes[i], 1);
    }

    printf("\nmontagem concluída!\n");
}

 

void atendimento_por_dias(hamburguer cardapio[], struct Fila *fila, int dias) {
    NoIngred *lista_consumo = NULL;
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

            montar_hamburguer_manual(&cardapio[idx], &lista_consumo);
            free(p);
        }

        printf("\ndia %d encerrado. pressione ENTER para continuar...\n", dia);
        getchar();
    }

    printf("\n=== Ingredientes consumidos ===\n");
    imprimirConsumidos(lista_consumo);
    liberarLista(lista_consumo);
}
