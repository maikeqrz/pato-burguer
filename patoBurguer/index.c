#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "cardapio.h"
#include "fila.h"
#include "loja.h"


#define backardapio "\033[48;2;0;0;0m" //dark
#define forardapio "\033[1;38;2;255;146;1m" //laranja pato 
#define backozinha "\033[48;2;255;255;255m" //cegante
#define forozinha "\033[3;38;2;22;65;148m" //azul senai
#define AVISO "\033[4;38;2;232;73;16m" //vermeio da universidade
#define resetar "\033[0m" //reinicia o back e o fore
#define apagar "\033[2J" //apaga a tela
#define tela "\033[?1049h" //obrigatorio
#define backode "\033[48;2;0;68;34m" //verdin de hacker
#define backluz "\033[48;2;68;0;34m" //rosa bem escuro
#define forcoelba "\033[2;38;2;255;255;0m" //amarelo luz 
#define forpedro "\033[5;38;2;170;34;204m" //violeta

// Tela padrao = 96x27

#define altura 27
#define largura 96

typedef struct {
    char espaco[altura][largura + 1][100];
} tipomatrix;

void cursor(int cx, int cy) 
{
    printf("\033[%d;%dH", cx, cy);
}

void showtime(tipomatrix matrix)
{
    for(int i = 0; i < altura; i++)
    {
        for (int j = 0; j < largura; j++)
        {
            printf("%s", matrix.espaco[i][j]);
        }
        printf("\n");
    }
}

void preencher(tipomatrix *matrix, char back[])
{
    strcpy(matrix->espaco[0][0], back);
    for(int i = 0; i < altura; i++)
    {
        for (int j = 0; j < largura; j++)
        {
            if (i == 0 && j == 0)
            {
                strcat(matrix->espaco[i][j], " ");
            }
            else
            {
                strcpy(matrix->espaco[i][j], " ");
            }
        }
    }
}

void escreverCentro(tipomatrix *matrix, char formatacao[], int linha, char frase[], char back[])
{
    int len = strlen(frase);
    int k = (largura - len) / 2; 
    strcpy(matrix->espaco[linha][k - 1], formatacao);
    strcat(matrix->espaco[linha][k - 1], " "); 
    for (int i = 0; i < len; i++)
    {
        char eita[2] = {frase[i], '\0'};
        strcpy(matrix->espaco[linha][k + i], eita);
    }
    strcpy(matrix->espaco[linha][k + len], resetar);
    strcat(matrix->espaco[linha][k + len], back);
    strcat(matrix->espaco[linha][k + len], " ");
}

void inputinhoTXT(tipomatrix *matrix, int x, int y, char formatacao[], char varzinha[])
{
    printf(formatacao);
    cursor(x+1, y);
    scanf(" %[^\n]s", varzinha);
}

void CLIENTE(tipomatrix *matrix, hamburguer cardapio[10]){

    int a3 = 0;

    for (int ll = 0; ll < 10; ll++){
        printf(resetar);
        printf(apagar);
        cursor(0, 0);

        preencher(matrix, backode);
        escreverCentro(matrix, AVISO, 3, "=========== CARDAPIO ===========", backode);
        int n = 0; 
        char xx2[50];
        sprintf(xx2, "%d. %s", cardapio[ll].ID, cardapio[ll].nome);
        escreverCentro(matrix, forcoelba, 7, xx2, backode);
        escreverCentro(matrix, forcoelba, 10, "Ingredientes: ", backode);
        char ingredientes_str[200] = ""; // String para armazenar os ingredientes
        int primeiro = 1;

        for (int j = 0; j < cardapio[ll].qntIngredientes; j++) {
            int repetido = 0;
            for (int k = 0; k < j; k++) {
                if (strcmp(cardapio[ll].ingredientes[j], cardapio[ll].ingredientes[k]) == 0) {
                    repetido = 1;
                    break;
                }
            }
        
            if (repetido == 0) {
                if (!primeiro) {
                    strcat(ingredientes_str, ", "); 
                }
                strcat(ingredientes_str, cardapio[ll].ingredientes[j]);
                primeiro = 0;
            }
        }
        escreverCentro(matrix, forcoelba, 12, ingredientes_str, backode);

        char xx5[50];
        sprintf(xx5, "Preco: R$ %.2f", cardapio[ll].preco);
        escreverCentro(matrix, forcoelba, 14, xx5, backode);

        escreverCentro(matrix, forcoelba, 18, "Por favor, digite 'proximo' ou 'anterior'.", backode); 

        escreverCentro(matrix, forcoelba, 21, "Digite aqui:              ", backode);
        if (a3 == 1){
            escreverCentro(matrix, AVISO, 25, "Digite algo válido", backode);
        }
        a3 = 0;
        showtime(*matrix);
        char categoria[12];
        inputinhoTXT(matrix, 21, 49, forpedro, categoria);
        strlwr(categoria);

        if(strcmp(categoria, "anterior") == 0){
            ll = ll - 2;
        }
        else if (strcmp(categoria, "proximo") != 0){
            a3 = 1;
            ll--;
        }
    }
}

void COZINHEIRO(tipomatrix *matrix){
    printf(resetar);
    printf(apagar);
    cursor(0, 0);
    preencher(matrix, backardapio);
    escreverCentro(matrix, forardapio, 3, "CLUBE DE PROGRAMACAO", backardapio);
    escreverCentro(matrix, forcoelba, 7, "EM ANDAMENTO", backardapio);
    escreverCentro(matrix, forcoelba, 15, "Por favor, informe sua senha: ", backardapio);
    escreverCentro(matrix, forcoelba, 19, "Digite aqui:              ", backardapio);
    showtime(*matrix);
    char senha[12];
    inputinhoTXT(matrix, 19, 49, forpedro, senha);
}

void GARCOM(tipomatrix *matrix, hamburguer cardapio[10], struct Fila* f){

    for (int xc = 0; xc < 5; xc++){
        int numeroP = rand() % 4 + 1;
        printf(resetar);
        printf(apagar);
        cursor(0, 0);
        preencher(matrix, backardapio);
        char dia[50];
        sprintf(dia, "Noite %d ", (xc+1));
        escreverCentro(matrix, forardapio, 2, dia, backardapio);
        escreverCentro(matrix, forcoelba, 5, "Pedidos: ", backardapio);
        struct Pedido* aux = f->inicio;
        int SEILA = 0;
        while (aux != NULL) {
            char cansei2[60];
            sprintf(cansei2, "Pedido n%d: %s", aux->id, cardapio[aux->idHamburguer - 1].nome);
            escreverCentro(matrix, forcoelba, (7 + SEILA), cansei2, backardapio);
            SEILA++;
            aux = aux->prox;
        }
        for (int v = 0; v < numeroP; v++){
            int pedidool = rand() % 10;
            enfileirar(f, v + SEILA + 1, cardapio[pedidool].ID);
            char cansei[60];
            sprintf(cansei, "Pedido n%d: %s", v + SEILA + 1, cardapio[pedidool].nome);
            escreverCentro(matrix, forcoelba, (7 + v + SEILA), cansei, backardapio);
        }
        escreverCentro(matrix, forcoelba, 25, "Digite algo e aperte ENTER para avancar a madrugada: ", backardapio);
        showtime(*matrix);
        char senha[12];
        inputinhoTXT(matrix, 25, 75, forpedro, senha);
    }
    struct Pedido* aux = f->inicio;
    while (aux != NULL) {
        aux = aux->prox;
        struct Pedido* p = desenfileirar(f);
        if (p != NULL) {
            free(p); 
        }
    }
}

void GERENTE(tipomatrix *matrix){
    printf(resetar);
    printf(apagar);
    cursor(0, 0);
    preencher(matrix, backluz);
    escreverCentro(matrix, AVISO, 3, "GERENCIA", backluz);
    escreverCentro(matrix, forcoelba, 7, "EM ANDAMENTO", backluz);
    escreverCentro(matrix, forcoelba, 15, "Por favor, informe sua senha: ", backluz);
    escreverCentro(matrix, forcoelba, 19, "Digite aqui:              ", backluz);
    showtime(*matrix);
    char senha[12];
    inputinhoTXT(matrix, 19, 49, forpedro, senha);
}

void ADM(tipomatrix *matrix){
    printf(resetar);
    printf(apagar);
    cursor(0, 0);
    preencher(matrix, backozinha);
    escreverCentro(matrix, forozinha, 3, "SENAI CIMATEC", backozinha);
    escreverCentro(matrix, AVISO, 4, "UNIVERSIDADE", backozinha);
    escreverCentro(matrix, forcoelba, 7, "EM ANDAMENTO", backozinha);
    escreverCentro(matrix, forcoelba, 8, "O que traz o senhor a nossa humilde filial?", backozinha);
    escreverCentro(matrix, forcoelba, 16, "Por favor, informe sua senha: ", backozinha);
    escreverCentro(matrix, forcoelba, 20, "Digite aqui:              ", backozinha);
    showtime(*matrix);
    char senha[12];
    inputinhoTXT(matrix, 20, 49, forpedro, senha);
}

int main()
{
    struct Fila fila;
    inicializaFila(&fila);

    tp_listad *loja = inicializa_estoque_loja(); // cria o estoque da loja (mantido durante o jogo)

    
    srand(time(NULL));

    hamburguer cardapio[10];

    hamburguers(cardapio);

    printf(tela);
    printf(resetar);
    printf(apagar);
    cursor(0, 0);

    tipomatrix comeco;
    preencher(&comeco, backode);
    escreverCentro(&comeco, AVISO, 3, "Bem vindo ao Patoburger!", backode);
    escreverCentro(&comeco, forcoelba, 7, "Eu sou o P.A.T.O", backode);
    escreverCentro(&comeco, forcoelba, 8, "Programa Autonomo de Tutoria Orientada", backode);
    escreverCentro(&comeco, forcoelba, 10, "Hoje estarei te auxiliando nesta filial.", backode);
    escreverCentro(&comeco, forcoelba, 12, "Por favor, informe sua categoria de ingresso: ", backode); 
    escreverCentro(&comeco, forcoelba, 14, "--> cliente <--", backode);
    escreverCentro(&comeco, forcoelba, 15, "EM MANUTENCAO --> cozinheiro <-- EM MANUTENCAO", backode);
    escreverCentro(&comeco, forcoelba, 16, "--> garcom <--", backode);
    escreverCentro(&comeco, forcoelba, 17, "EM MANUTENCAO --> gerente <-- EM MANUTENCAO", backode);
    escreverCentro(&comeco, forcoelba, 18, "EM MANUTENCAO --> admin <-- EM MANUTENCAO", backode);
    escreverCentro(&comeco, forcoelba, 19, "--> loja <--", backode);
    escreverCentro(&comeco, forcoelba, 21, "Digite aqui:              ", backode);
    showtime(comeco);
    char categoria[12];
    inputinhoTXT(&comeco, 21, 49, forpedro, categoria);
    strlwr(categoria);

    FILE *data;
    data = fopen("cadastros.txt", "r");

    int b = 0;
    int socorro = 0;
    while (b == 0)
    {
        if (strcmp(categoria, "cliente") == 0)
        {
            CLIENTE(&comeco, cardapio);
            socorro = 1;
        }
        else if (strcmp(categoria, "cozinheiro") == 0)
        {
            int a = 0;
            int a1 = 0;
            while (a == 0)
            {
                printf(resetar);
                printf(apagar);
                cursor(0, 0);
                preencher(&comeco, backardapio);
                escreverCentro(&comeco, forardapio, 3, "CLUBE DE PROGRAMACAO", backardapio);
                escreverCentro(&comeco, forcoelba, 7, "O expediente inicia agora.", backardapio);
                escreverCentro(&comeco, forcoelba, 15, "Por favor, informe sua senha: ", backardapio);
                escreverCentro(&comeco, forcoelba, 19, "Digite aqui:              ", backardapio);
                if (a1 == 1){
                    escreverCentro(&comeco, AVISO, 25, "Senha Incorreta!", backardapio);
                }
                showtime(comeco);
                char senha[12];
                inputinhoTXT(&comeco, 19, 49, forpedro, senha);
                rewind(data);

                char validacao[14];

                fgets(validacao, 14, data);
                validacao[strcspn(validacao, "\n")] = '\0';

                if (strcmp(validacao, senha) == 0){
                    a = 1;
                }
                else{
                    a1 = 1;
                }
            }
            COZINHEIRO(&comeco);
            socorro = 1;
        }
        else if (strcmp(categoria, "garcom") == 0)
        {
            int a = 0;
            int a1 = 0;
            while (a == 0)
            {
                printf(resetar);
                printf(apagar);
                cursor(0, 0);
                preencher(&comeco, backardapio);
                escreverCentro(&comeco, forardapio, 3, "CLUBE DE PROGRAMACAO", backardapio);
                escreverCentro(&comeco, forcoelba, 7, "anote os pedidos UwU", backardapio);
                escreverCentro(&comeco, forcoelba, 15, "Por favor, informe sua senha: ", backardapio);
                escreverCentro(&comeco, forcoelba, 19, "Digite aqui:              ", backardapio);
                if (a1 == 1){
                    escreverCentro(&comeco, AVISO, 25, "Senha Incorreta!", backardapio);
                }
                showtime(comeco);
                char senha[12];
                inputinhoTXT(&comeco, 19, 49, forpedro, senha);
                rewind(data);

                char lixo1[14];
                char lixo2[14];
                char lixo3[14];
                char validacao[14];

                fgets(lixo1, 14, data);
                fgets(lixo2, 14, data);
                fgets(lixo3, 14, data);
                fgets(validacao, 14, data);
                validacao[strcspn(validacao, "\n")] = '\0';

                if (strcmp(validacao, senha) == 0){
                    a = 1;
                }
                else{
                    a1 = 1;
                }
            }
            GARCOM(&comeco, cardapio, &fila);
            socorro = 1;
        }
        else if (strcmp(categoria, "gerente") == 0)
        {
            int a = 0;
            int a1 = 0;
            while (a == 0)
            {
                printf(resetar);
                printf(apagar);
                cursor(0, 0);
                preencher(&comeco, backluz);
                escreverCentro(&comeco, AVISO, 3, "GERENCIA", backluz);
                escreverCentro(&comeco, forcoelba, 7, "Relatorio pendente.", backluz);
                escreverCentro(&comeco, forcoelba, 15, "Por favor, informe sua senha: ", backluz);
                escreverCentro(&comeco, forcoelba, 19, "Digite aqui:              ", backluz);
                if (a1 == 1){
                    escreverCentro(&comeco, AVISO, 25, "Senha Incorreta!", backluz);
                }
                showtime(comeco);
                char senha[12];
                inputinhoTXT(&comeco, 19, 49, forpedro, senha);
                rewind(data);

                char lixo1[14];
                char validacao[14];

                fgets(lixo1, 14, data);
                fgets(validacao, 14, data);
                validacao[strcspn(validacao, "\n")] = '\0';

                if (strcmp(validacao, senha) == 0){
                    a = 1;
                }
                else{
                    a1 = 1;
                }
            }
            GERENTE(&comeco);
            socorro = 1;
        }
        else if (strcmp(categoria, "admin") == 0)
        {
            int a = 0;
            int a1 = 0;
            while (a == 0)
            {
                printf(resetar);
                printf(apagar);
                cursor(0, 0);
                preencher(&comeco, backozinha);
                escreverCentro(&comeco, forozinha, 3, "SENAI CIMATEC", backozinha);
                escreverCentro(&comeco, AVISO, 4, "UNIVERSIDADE", backozinha);
                escreverCentro(&comeco, forcoelba, 7, "Saudacoes Sanval/Soussa!", backozinha);
                escreverCentro(&comeco, forcoelba, 8, "O que traz o senhor a nossa humilde filial?", backozinha);
                escreverCentro(&comeco, forcoelba, 16, "Por favor, informe sua senha: ", backozinha);
                escreverCentro(&comeco, forcoelba, 20, "Digite aqui:              ", backozinha);
                if (a1 == 1){
                    escreverCentro(&comeco, AVISO, 25, "Senha Incorreta!", backozinha);
                }
                showtime(comeco);
                char senha[12];
                inputinhoTXT(&comeco, 20, 49, forpedro, senha);
                rewind(data);

                char lixo1[14];
                char lixo2[14];
                char validacao[14];

                fgets(lixo1, 14, data);
                fgets(lixo2, 14, data);
                fgets(validacao, 14, data);
                validacao[strcspn(validacao, "\n")] = '\0';

                if (strcmp(validacao, senha) == 0){
                    a = 1;
                }
                else{
                    a1 = 1;
                }
            }
            ADM(&comeco);
            socorro = 1;
        }

        else if (strcmp(categoria, "loja") == 0) {
    // Sai do modo de tela alternativa antes de abrir a loja
    printf("\033[?1049l");
    printf(resetar);
    printf(apagar);

    menu_loja(loja);  // usa o mesmo estoque existente

    // Volta para o modo de tela do jogo
    printf(tela);
    printf(resetar);
    printf(apagar);
    cursor(0, 0);

    socorro = 1;
}

        printf(resetar);
        printf(apagar);
        cursor(0, 0);

        tipomatrix comeco;
        preencher(&comeco, backode);
        escreverCentro(&comeco, AVISO, 3, "Bem vindo ao Patoburger!", backode);
        escreverCentro(&comeco, forcoelba, 7, "Eu sou o P.A.T.O", backode);
        escreverCentro(&comeco, forcoelba, 8, "Programa Autonomo de Tutoria Orientada", backode);
        escreverCentro(&comeco, forcoelba, 10, "Hoje estarei te auxiliando nesta filial.", backode);
        escreverCentro(&comeco, forcoelba, 12, "Por favor, informe sua categoria de ingresso: ", backode); 
        escreverCentro(&comeco, forcoelba, 14, "--> cliente <--", backode);
        escreverCentro(&comeco, forcoelba, 15, "EM MANUTENCAO --> cozinheiro <-- EM MANUTENCAO", backode);
        escreverCentro(&comeco, forcoelba, 16, "--> garcom <--", backode);
        escreverCentro(&comeco, forcoelba, 17, "EM MANUTENCAO --> gerente <-- EM MANUTENCAO", backode);
        escreverCentro(&comeco, forcoelba, 18, "EM MANUTENCAO --> admin <-- EM MANUTENCAO", backode);
        escreverCentro(&comeco, forcoelba, 21, "Digite aqui:              ", backode);

        if (socorro == 0){
            escreverCentro(&comeco, AVISO, 25, "Por favor, informe uma categoria valida.", backode);
        }
        socorro = 0;

        showtime(comeco);
        inputinhoTXT(&comeco, 21, 49, forpedro, categoria);
        strlwr(categoria);
    }

    fclose(data);

    return 0;
}
