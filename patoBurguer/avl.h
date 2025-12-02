#ifndef AVL_H
#define AVL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct NoAVL {
    char nome[30];
    int quantidade;
    int altura;
    struct NoAVL *esq, *dir;
} NoAVL;

int alturaAVL(NoAVL *n) {
    return n ? n->altura : 0;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

NoAVL *novoNoAVL(char *nome, int qtd) {
    NoAVL *no = (NoAVL*) malloc(sizeof(NoAVL));
    strcpy(no->nome, nome);
    no->quantidade = qtd;
    no->altura = 1;
    no->esq = no->dir = NULL;
    return no;
}

NoAVL *rotDir(NoAVL *y) {
    NoAVL *x = y->esq;
    NoAVL *T2 = x->dir;

    x->dir = y;
    y->esq = T2;

    y->altura = max(alturaAVL(y->esq), alturaAVL(y->dir)) + 1;
    x->altura = max(alturaAVL(x->esq), alturaAVL(x->dir)) + 1;

    return x;
}

NoAVL *rotEsq(NoAVL *x) {
    NoAVL *y = x->dir;
    NoAVL *T2 = y->esq;

    y->esq = x;
    x->dir = T2;

    x->altura = max(alturaAVL(x->esq), alturaAVL(x->dir)) + 1;
    y->altura = max(alturaAVL(y->esq), alturaAVL(y->dir)) + 1;

    return y;
}

int fatorBalance(NoAVL *n) {
    return n ? alturaAVL(n->esq) - alturaAVL(n->dir) : 0;
}

NoAVL *inserirAVL(NoAVL *no, char *nome, int qtd) {
    if (!no) return novoNoAVL(nome, qtd);

    int cmp = strcmp(nome, no->nome);

    if (cmp < 0)
        no->esq = inserirAVL(no->esq, nome, qtd);
    else if (cmp > 0)
        no->dir = inserirAVL(no->dir, nome, qtd);
    else {
        no->quantidade += qtd;
        return no;
    }

    no->altura = 1 + max(alturaAVL(no->esq), alturaAVL(no->dir));

    int fb = fatorBalance(no);

    if (fb > 1 && strcmp(nome, no->esq->nome) < 0)
        return rotDir(no);

    if (fb < -1 && strcmp(nome, no->dir->nome) > 0)
        return rotEsq(no);

    if (fb > 1 && strcmp(nome, no->esq->nome) > 0) {
        no->esq = rotEsq(no->esq);
        return rotDir(no);
    }

    if (fb < -1 && strcmp(nome, no->dir->nome) < 0) {
        no->dir = rotDir(no->dir);
        return rotEsq(no);
    }

    return no;
}

void salvarConsumo(NoAVL *raiz, FILE *arquivo) {
    if (!raiz) return;

    salvarConsumo(raiz->esq, arquivo);
    fprintf(arquivo, "%s %d\n", raiz->nome, raiz->quantidade);
    salvarConsumo(raiz->dir, arquivo);
}
NoAVL *consumoDiario = NULL;

#endif
