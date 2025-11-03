#ifndef CARDAPIO_H
#define CARDAPIO_H
#include <stdio.h>
#include <string.h>
#include "ingredientes.h"


typedef struct
{
    int ID;
    char nome[50];
    char *ingredientes[20];
    int qntIngredientes;
    double preco;

} hamburguer;

 
void hamburguers (hamburguer cardapio[10]) {

    cardapio[0] = (hamburguer){1, "Bit and Bacon", {i[0], i[2], i[3], i[1], i[0]}, 5, 18.90};
    cardapio[1] = (hamburguer){2, "Duck Cheese", {i[0], i[4], i[5], i[3], i[0]}, 5, 15.90};
    cardapio[2] = (hamburguer){3, "Quackteirao", {i[0], i[4], i[3], i[1], i[3], i[0]}, 6, 16.90};
    cardapio[3] = (hamburguer){4, "Big Pato", {i[0], i[1], i[4], i[0], i[1], i[3], i[4], i[0]}, 8, 18.90};
    cardapio[4] = (hamburguer){5, "Brabo Patissimo", {i[0], i[6], i[4], i[1], i[1], i[3], i[9], i[0]}, 8, 19.90};
    cardapio[5] = (hamburguer){6, "Tasty Donald", {i[0], i[6], i[7], i[4], i[5], i[3], i[1], i[3], i[0]}, 9, 20.50};
    cardapio[6] = (hamburguer){7, "Patolino Burguer", {i[0], i[10], i[3], i[4], i[6], i[7], i[8], i[3], i[0]}, 9, 20.00};
    cardapio[7] = (hamburguer){8, "Cheddar Quack Melt", {i[0], i[1], i[3], i[3], i[7], i[0]}, 6, 17.50};
    cardapio[8] = (hamburguer){9, "Patao", {i[0], i[1], i[1], i[3], i[6], i[2], i[11], i[0]}, 8, 22.00};
    cardapio[9] = (hamburguer){10, "Veganito do Pato", {i[13], i[12], i[5], i[7], i[4], i[14], i[13]}, 7, 22.50};
};

#endif