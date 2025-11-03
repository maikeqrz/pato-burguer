#ifndef INGREDIENTES_H
#define INGREDIENTES_H
#include <stdio.h>
#include <string.h>

char i[15][20] = {"Pao", "Carne", "Bacon", "Queijo",  "Alface", "Tomate", "Molho Especial", 
    "Cebola", "Picles", "Ceboola Crispy", "Frango", "Onion Rings", "Carne de soja", "Pao integral", "Abacate"};

typedef struct {

    char *nome;
    int quantEstoque;
    float preco;

} ingrediente;

ingrediente estoque[15] = {
    
    /* ingredientes obrigatórios */
    {i[0], 50, 0.80},
    {i[1], 30, 3.00},
    {i[2], 20, 2.50},
    {i[3], 40, 1.80},
    {i[4], 25, 0.70},
    {i[5], 25, 0.80},
    
    /* Ingredientes criados por nós */
    {i[6], 15, 1.00},
    {i[7], 20, 0.60},
    {i[8], 10, 0.90},
    {i[9], 12, 1.50},
    {i[10], 18, 2.80},
    {i[11], 14, 1.20},
    {i[12], 8, 5.00},
    {i[13], 6, 3.00},
    {i[14], 4, 6.00}
};

#endif