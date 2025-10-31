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


void inicializaEstoque(IngredienteEstoque estoque[], int *n) {
    *n = 8;
    strcpy(estoque[0].nome, "Pao");     estoque[0].quantidade = 50;
    strcpy(estoque[1].nome, "Carne");   estoque[1].quantidade = 40;
    strcpy(estoque[2].nome, "Bacon");   estoque[2].quantidade = 30;
    strcpy(estoque[3].nome, "Queijo");  estoque[3].quantidade = 40;
    strcpy(estoque[4].nome, "Alface");  estoque[4].quantidade = 60;
    strcpy(estoque[5].nome, "Tomate");  estoque[5].quantidade = 50;
    strcpy(estoque[6].nome, "Molho");   estoque[6].quantidade = 40;
    strcpy(estoque[7].nome, "Cebola");  estoque[7].quantidade = 30;
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


void atendimento_cliente(hamburguer *h) {
    printf("\nOii, tudo bem? Eu desejo um %s!\n", h->nome);
}

void montar_hamburguer_manual(hamburguer *h, IngredienteEstoque estoque[], int n, NoIngred **lista_consumo) {
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

        inserirConsumido(lista_consumo, h->ingredientes[i], 1);
    }

    printf("\nMontagem concluída!\n");
}



void atendimento_por_dias(hamburguer cardapio[], struct Fila *fila, IngredienteEstoque estoque[], int nEstoque, int dias) {
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

            montar_hamburguer_manual(&cardapio[idx], estoque, nEstoque, &lista_consumo);
            free(p);
        }

        printf("\nDia %d encerrado. Pressione ENTER para continuar...\n", dia);
        getchar();
    }

    printf("\n=== Ingredientes consumidos ===\n");
    imprimirConsumidos(lista_consumo);
    liberarLista(lista_consumo);
}
