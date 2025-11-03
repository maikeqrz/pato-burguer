#ifndef PILHA_H
#define PILHA_H

#include <stdio.h>
#include "cardapio.h"

#define MAX 100

// Tipo de item da pilha
typedef ingrediente* tp_item_pilha;

// Estrutura da pilha
typedef struct {
    int topo;
    tp_item_pilha item[MAX];
} tp_pilha;

// Inicializa a pilha
void inicializa_pilha(tp_pilha *p) {
    p->topo = -1;
}

// Verifica se a pilha está vazia
int pilhaVazia(tp_pilha *p) {
    return p->topo == -1;
}

// Verifica se a pilha está cheia
int pilhaCheia(tp_pilha *p) {
    return p->topo == MAX - 1;
}

// Empilha um elemento
int push(tp_pilha *p, tp_item_pilha e) {
    if (pilhaCheia(p)) {
        return 0;  // Falha ao empilhar (pilha cheia)
    }
    p->topo++;
    p->item[p->topo] = e;
    return 1;  // Sucesso
}

// Desempilha um elemento
int pop(tp_pilha *p, tp_item_pilha *e) {
    if (pilhaVazia(p)) {
        return 0;  // Falha ao desempilhar (pilha vazia)
    }
    *e = p->item[p->topo];
    p->topo--;
    return 1;  // Sucesso
}

int alturaPilha(tp_pilha *p) {
    return p->topo + 1;
}

void imprimePilha(tp_pilha p) {
    tp_item_pilha e;
    printf("\n");
    while (!pilhaVazia(&p)) {
        pop(&p, &e);
        printf("%s\n", e);
    }
}

#endif  // PILHA_H
