#ifndef FILA_H
#define FILA_H
#include <stdio.h>
#include <stdlib.h>

struct Pedido {
    int id;
    int idHamburguer;
    struct Pedido* prox; 
};

struct Fila {
    struct Pedido* inicio;
    struct Pedido* fim;
};

void inicializaFila(struct Fila* f) {
    f->inicio = NULL;
    f->fim = NULL;
}

void enfileirar(struct Fila* f, int id, int idHamburguer) {
    struct Pedido* novo = malloc(sizeof(struct Pedido));
    novo->id = id;
    novo->idHamburguer = idHamburguer;
    novo->prox = NULL;

    if (f->fim == NULL) { 
        f->inicio = novo;
        f->fim = novo;
    } else {
        f->fim->prox = novo;
        f->fim = novo;
    }
}

struct Pedido* desenfileirar(struct Fila* f) {
    if (f->inicio == NULL) return NULL; 

    struct Pedido* removido = f->inicio;
    f->inicio = f->inicio->prox;
    if (f->inicio == NULL) f->fim = NULL; 

    return removido;
}

void mostrarFila(struct Fila* f) {
    struct Pedido* aux = f->inicio;
    while (aux != NULL) {
        printf("Pedido %d (Hamburguer %d)\n", aux->id, aux->idHamburguer);
        aux = aux->prox;
    }
}

/* int main() {
    struct Fila fila;
    inicializaFila(&fila);

    enfileirar(&fila, 1, 2);
    enfileirar(&fila, 2, 4);
    enfileirar(&fila, 3, 1);

    printf("Fila atual:\n");
    mostrarFila(&fila);

    struct Pedido* p = desenfileirar(&fila);
    if (p != NULL) {
        printf("\nRemovido: Pedido %d (Hamburguer %d)\n", p->id, p->idHamburguer);
        free(p); 
    }

    printf("\nFila após remoção:\n");
    mostrarFila(&fila);

    return 0;
} */

#endif