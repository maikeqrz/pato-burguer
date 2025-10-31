#include <stdio.h>
#include <string.h>
#include "pilha.h"

char ingredientes[15][20] = {
    "Pao", "Carne", "Bacon", "Queijo", "Alface", "Tomate",
    "Molho Especial", "Cebola", "Picles", "Cebola Crispy",
    "Frango", "Onion Rings", "Carne de soja", "Pao integral", "Abacate"
};

typedef struct {
    int ID;
    char nome[50];
    char *ingredientes[20];
    int qntIngredientes;
    double preco;
} hamburguer;

hamburguer cardapio[10];

void cria_cardapio() {
    cardapio[0] = (hamburguer){1, "Bit and Bacon", {ingredientes[0], ingredientes[2], ingredientes[3], ingredientes[1], ingredientes[0]}, 5, 18.90};
    cardapio[1] = (hamburguer){2, "Duck Cheese", {ingredientes[0], ingredientes[4], ingredientes[5], ingredientes[3], ingredientes[0]}, 5, 15.90};
    cardapio[2] = (hamburguer){3, "Quackteirao", {ingredientes[0], ingredientes[4], ingredientes[3], ingredientes[1], ingredientes[3], ingredientes[0]}, 6, 16.90};
    cardapio[3] = (hamburguer){4, "Big Pato", {ingredientes[0], ingredientes[1], ingredientes[4], ingredientes[0], ingredientes[1], ingredientes[3], ingredientes[4], ingredientes[0]}, 8, 18.90};
    cardapio[4] = (hamburguer){5, "Brabo Patissimo", {ingredientes[0], ingredientes[6], ingredientes[4], ingredientes[1], ingredientes[1], ingredientes[3], ingredientes[9], ingredientes[0]}, 8, 19.90};
    cardapio[5] = (hamburguer){6, "Tasty Donald", {ingredientes[0], ingredientes[6], ingredientes[7], ingredientes[4], ingredientes[5], ingredientes[3], ingredientes[1], ingredientes[3], ingredientes[0]}, 9, 20.50};
    cardapio[6] = (hamburguer){7, "Patolino Burguer", {ingredientes[0], ingredientes[10], ingredientes[3], ingredientes[4], ingredientes[6], ingredientes[7], ingredientes[8], ingredientes[3], ingredientes[0]}, 9, 20.00};
    cardapio[7] = (hamburguer){8, "Cheddar Quack Melt", {ingredientes[0], ingredientes[1], ingredientes[3], ingredientes[3], ingredientes[7], ingredientes[0]}, 6, 17.50};
    cardapio[8] = (hamburguer){9, "Patao", {ingredientes[0], ingredientes[1], ingredientes[1], ingredientes[3], ingredientes[6], ingredientes[2], ingredientes[11], ingredientes[0]}, 8, 22.00};
    cardapio[9] = (hamburguer){10, "Veganito do Pato", {ingredientes[13], ingredientes[12], ingredientes[5], ingredientes[7], ingredientes[4], ingredientes[14], ingredientes[13]}, 7, 22.50};
}

void monta_hamburguer(hamburguer h) {
    tp_pilha p;
    inicializa_pilha(&p);

    printf("--- %s ---\n", h.nome);

    for (int i = 0; i < h.qntIngredientes; i++) {
        push(&p, h.ingredientes[i]);
    }

    for (int i = h.qntIngredientes - 1; i >= 0; i--) {
        printf("%s\n", h.ingredientes[i]);
    }
}
