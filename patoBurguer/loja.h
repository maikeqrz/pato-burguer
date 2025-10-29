#ifndef LOJA_H
#define LOJA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ingredientes.h"
#include "listade.h"

// Códigos de cores para manter o visual do jogo
#define backode "\033[48;2;0;68;34m"   // Cor de fundo verde
#define forcoelba "\033[2;38;2;255;255;0m" // Cor de texto amarela
#define resetar "\033[0m"              // Reseta cores
#define apagar "\033[2J"               // Limpa a tela

// Inicializa o estoque base da loja com os 15 ingredientes definidos
tp_listad* inicializa_estoque_loja() {
    tp_listad *loja = inicializa_listad(); // Cria uma lista duplamente encadeada
    if (loja == NULL) {                    // Verifica se a memória foi alocada corretamente
        printf("Erro ao inicializar a loja!\n");
        exit(1);
    }

    // Insere todos os ingredientes padrão no início do jogo
    for (int j = 0; j < 15; j++) {
        insere_listad_no_fim(loja, estoque[j]);
    }

    return loja; // Retorna o ponteiro da loja criada
}

// Exibe todos os ingredientes cadastrados na loja
void mostrar_loja(tp_listad *loja) {
    printf(resetar);    // Reseta formatação anterior
    printf(apagar);     // Limpa a tela
    printf(backode);    // Aplica fundo verde
    printf("\n=====  LOJA PATÔNICA =====\n");

    // Caso o estoque esteja vazio, exibe aviso
    if (listad_vazia(loja)) {
        printf("\n A loja está vazia!\n");
        printf(resetar);
        return;
    }

    // Percorre toda a lista de ingredientes e imprime seus dados
    tp_no *atu = loja->ini;
    while (atu != NULL) {
        printf("%s-> %s | Quantidade: %d | Preço: R$ %.2f%s\n",
               forcoelba, atu->info.nome, atu->info.quantEstoque, atu->info.preco, resetar);
        atu = atu->prox; // Passa para o próximo item
    }

    // Linha de fechamento visual
    printf("%s============================\n%s", forcoelba, resetar);
}

// Permite comprar novos ingredientes ou adicionar mais unidades dos já existentes
void comprar_ingrediente(tp_listad *loja) {
    ingrediente novo;
    char nome[50];

    // Exibe o estoque atual antes de iniciar a compra
    printf("\n Estoque atual antes da compra:\n");
    mostrar_loja(loja);

    // Recebe o nome do ingrediente que será comprado
    printf("\n Digite o nome do ingrediente a comprar: ");
    scanf(" %[^\n]", nome);

    // Cria dinamicamente uma string para armazenar o nome do novo ingrediente
    novo.nome = malloc(strlen(nome) + 1);
    strcpy(novo.nome, nome);

    // Solicita a quantidade e o preço unitário
    printf("Quantidade comprada: ");
    scanf("%d", &novo.quantEstoque);

    printf("Preço de compra (por unidade): ");
    scanf("%f", &novo.preco);

    // Verifica se o ingrediente já existe na lista
    tp_no *atu = loja->ini;
    while (atu != NULL && strcasecmp(atu->info.nome, nome) != 0) {
        atu = atu->prox;
    }

    // Se o ingrediente já existir, apenas atualiza quantidade e preço
    if (atu != NULL) {
        atu->info.quantEstoque += novo.quantEstoque;
        atu->info.preco = novo.preco; 
        printf("\n Estoque de %s atualizado. Total: %d unidades.\n",
               nome, atu->info.quantEstoque);
        free(novo.nome); // Libera memória usada temporariamente
    } 
    // Caso contrário, insere como novo item no final da lista
    else {
        insere_listad_no_fim(loja, novo);
        printf("\n %d unidades de %s adicionadas ao estoque.\n",
               novo.quantEstoque, novo.nome);
    }

    // Exibe o estoque atualizado após a operação
    printf("\n Estoque atualizado após compra:\n");
    mostrar_loja(loja);
}

// Permite vender ingredientes, reduzindo suas quantidades ou removendo do estoque
void vender_ingrediente(tp_listad *loja) {
    // Se o estoque estiver vazio, não há o que vender
    if (listad_vazia(loja)) {
        printf("\n Nenhum ingrediente disponível para venda.\n");
        return;
    }

    // Exibe o estoque atual antes da venda
    printf("\n Estoque atual antes da venda:\n");
    mostrar_loja(loja);

    char nome[50];
    int qtd;

    // Pede o nome e a quantidade a serem vendidos
    printf("\n Digite o nome do ingrediente a vender: ");
    scanf(" %[^\n]", nome);
    printf("Quantidade a vender: ");
    scanf("%d", &qtd);

    // Procura o ingrediente na lista
    tp_no *atu = loja->ini;
    while (atu != NULL && strcasecmp(atu->info.nome, nome) != 0) {
        atu = atu->prox;
    }

    // Caso o ingrediente não seja encontrado
    if (atu == NULL) {
        printf("\n Ingrediente %s não encontrado.\n", nome);
        return;
    }

    // Se a quantidade vendida for igual ou maior que o total, remove o item completamente
    if (qtd >= atu->info.quantEstoque) {
        printf("\n Vendido todo o estoque de %s.\n", atu->info.nome);
        remove_listad(loja, atu->info);
    } 
    // Caso contrário, apenas reduz a quantidade disponível
    else {
        atu->info.quantEstoque -= qtd;
        printf("\n Vendido %d unidades de %s. Restam %d.\n",
               qtd, atu->info.nome, atu->info.quantEstoque);
    }

    // Exibe o estoque após a venda
    printf("\n Estoque atualizado após venda:\n");
    mostrar_loja(loja);
}

// Menu principal da loja — interface de navegação das opções
void menu_loja(tp_listad *loja) {
    int opcao;
    do {
        // Configura visual e imprime as opções do menu
        printf(resetar);
        printf(apagar);
        printf(backode);
        printf("\n===== MENU LOJA PATÔNICA =====\n");
        printf("1. Mostrar estoque da loja\n");
        printf("2. Comprar ingrediente novo\n");
        printf("3. Vender ingrediente\n");
        printf("0. Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        // Interpreta a opção digitada
        switch (opcao) {
            case 1:
                mostrar_loja(loja);         // Exibe a lista de ingredientes
                break;
            case 2:
                comprar_ingrediente(loja);  // Chama a função de compra
                break;
            case 3:
                vender_ingrediente(loja);   // Chama a função de venda
                break;
            case 0:
                printf("\n Encerrando a Loja Patônica...\n");
                break;
            default:
                printf("\n Opção inválida!\n");
        }

        // Aguarda o usuário pressionar ENTER antes de voltar ao menu
        if (opcao != 0) {
            printf("\nPressione ENTER para continuar...");
            getchar(); getchar(); // Captura o ENTER
        }

    } while (opcao != 0); // Repete até o usuário escolher sair
}

#endif
