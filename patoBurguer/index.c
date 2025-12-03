#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include "cardapio.h"
#include "fila.h"
#include "listade.h"
#include "pilha.h"
#include "AVL.h"

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
/* depois de: #include "ingredientes_diarios.h" */
NoAVL *consumoDiario = NULL;


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

void consertaEspaco(char *str) {
    if (str == NULL) return;
    
    int len = strlen(str);
    while (len > 0 && isspace((unsigned char)str[len - 1])) {
        len--;
    }
    str[len] = '\0';
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

int conta_estoque(tp_listad *lista, ingrediente item);

void imprime_listad2salva(tp_listad *lista, FILE *data2, int dindin) {
    
    freopen("logy.txt", "w+", data2); 
    
    fprintf(data2, "%d\n", dindin);

    for (int i = 0; i < 15; i++) {
        int qtd = conta_estoque(lista, estoque[i]);
        fprintf(data2, "%d\n", qtd);
    }
    fflush(data2); 
}

int lerArquivoss(FILE *data2, int linha){
    rewind(data2);
    int numFindedi;
    char buffeIngrid[400];
    int linhaAtual = 1;
    while (fgets(buffeIngrid, sizeof(buffeIngrid), data2) != NULL) {
        if (linhaAtual == linha) {
            if (sscanf(buffeIngrid, "%d", &numFindedi) == 1) {
                return numFindedi;
            }
            break; 
        }
        
        linhaAtual++;
    }
    return -1;
}

void inputinhoTXT(tipomatrix *matrix, int x, int y, char formatacao[], char varzinha[])
{
    printf(formatacao);
    cursor(x+1, y);
    scanf(" %[^\n]s", varzinha);
}

void pausar(tipomatrix *matrix, char back[]) {
    escreverCentro(matrix, forcoelba, 25, "Pressione ENTER para continuar...", back);
    showtime(*matrix);
    char pausa[5];
    inputinhoTXT(matrix, 25, 49, forpedro, pausa);
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
        char ingredientes_str[200] = ""; 

        for (int j = 0; j < cardapio[ll].qntIngredientes; j++) {
            if (j > 0) { 
                strcat(ingredientes_str, ", "); 
            }
            strcat(ingredientes_str, cardapio[ll].ingredientes[j]);
        }
        escreverCentro(matrix, forcoelba, 12, ingredientes_str, backode);

        char xx5[50];
        sprintf(xx5, "Preco: R$ %.2f", cardapio[ll].preco);
        escreverCentro(matrix, forcoelba, 14, xx5, backode);

        escreverCentro(matrix, forcoelba, 18, "Digite 'p' (proximo), 'a' (anterior) ou 's' (sair)", backode);
 

        escreverCentro(matrix, forcoelba, 21, "Digite aqui:              ", backode);
        if (a3 == 1){
            escreverCentro(matrix, AVISO, 25, "Digite algo válido", backode);
        }
        a3 = 0;
        showtime(*matrix);
        char categoria[12];
        inputinhoTXT(matrix, 21, 49, forpedro, categoria);
        int i = 0;
        while (categoria[i] != '\0') {
            categoria[i] = tolower(categoria[i]);
            i++;
        }

        if(strcmp(categoria, "s") == 0){
            return;  // <-- AQUI ELE SAI DO CLIENTE E VOLTA PARA O MENU
        }
        else if(strcmp(categoria, "a") == 0){
            if (ll > 0) {
                ll = ll - 2; 
            } else {
                ll = -1; 
        }
        }
        else if (strcmp(categoria, "p") != 0){
            a3 = 1;
            ll--;
        }

    }
}

void COZINHEIRO(tipomatrix *matrix, hamburguer cardapio[10], tp_pilha *pilhaa, FILE *data2, int *dinheiro, tp_listad *lista){

    for (int xc = 0; xc < 5; xc++) {

        // LIMPA A TELA E MOSTRA O TÍTULO DA NOITE
        printf(resetar);
        printf(apagar);
        cursor(0, 0);
        preencher(matrix, backardapio);

        char dia[50];
        sprintf(dia, "Noite %d ", (xc + 1));
        escreverCentro(matrix, forardapio, 2, dia, backardapio);

        // QUANTIDADE DE HAMBÚRGUERES A SEREM FEITOS
        int quantidadePedidos = (rand() % 4) + 2; // 2 a 5 hambúrgueres
        char msgQtd[100];
        sprintf(msgQtd, "Voce deve montar %d hamburgueres nesta noite!", quantidadePedidos);
        escreverCentro(matrix, forcoelba, 5, msgQtd, backardapio);

        showtime(*matrix);
        sleep(1);

        // ==============================
        //   LOOP DOS PEDIDOS DA NOITE
        // ==============================
        for (int pedidoIndex = 0; pedidoIndex < quantidadePedidos; pedidoIndex++) {

            printf(resetar);
            printf(apagar);
            cursor(0, 0);
            preencher(matrix, backardapio);

            // mostra estoque à direita
            imprimirEstoqueDireita(matrix, lista, backardapio);

            // sorteia um hamburger
            int pedidool = rand() % 10;

            char tituloPedido[150];
            sprintf(tituloPedido, "Pedido %d/%d: %s",
                    pedidoIndex + 1, quantidadePedidos, cardapio[pedidool].nome);

            escreverCentro(matrix, forcoelba, 7, tituloPedido, backardapio);

            int praqtantavar = 0;

            // ==============================
            //     LOOP DE MONTAGEM
            // ==============================
            while (praqtantavar == 0) {

                escreverCentro(matrix, forcoelba, 10, "Digite os ingredientes na ordem correta!", backardapio);
                escreverCentro(matrix, forcoelba, 11, "Ingrediente: ", backardapio);
                

                // Mostra a pilha (hamburguer sendo montado)
                if (!pilhaVazia(pilhaa)) {
                    int linha = 15;
                    int topo = alturaPilha(pilhaa) - 1;

                    for (int k = topo; k >= 0; k--) {
                        escreverCentro(matrix, forcoelba, linha, pilhaa->item[k]->nome, backardapio);
                        linha++;
                    }
                }

                // Estoque novamente à direita
                imprimirEstoqueDireita(matrix, lista, backardapio);

                showtime(*matrix);

                char INGed[40];
                inputinhoTXT(matrix, 11, 60, forpedro, INGed);

                // limpa tela
                printf(resetar);
                printf(apagar);
                cursor(0,0);

                // minúsculo
                for (int i = 0; INGed[i] != '\0'; i++)
                    INGed[i] = tolower(INGed[i]);

                int ingredienteValido = 0;
                int semEstoque = 0;

                // CHECA INGREDIENTE
                for (int c = 0; c < 15; c++) {

                    char nomeEstoqueMinusculo[50];
                    strcpy(nomeEstoqueMinusculo, estoque[c].nome);
                    for (int k = 0; nomeEstoqueMinusculo[k] != '\0'; k++)
                        nomeEstoqueMinusculo[k] = tolower(nomeEstoqueMinusculo[k]);

                    if (strcmp(INGed, nomeEstoqueMinusculo) == 0) {

                        ingredienteValido = 1;

                        tp_no *no = busca_listade(lista, estoque[c]);
                        if (no != NULL) {
                            push(pilhaa, &estoque[c]);
                            remove_listad(lista, estoque[c]);
                        } else {
                            semEstoque = 1;
                        }
                        break;
                    }
                }

                // tratativas
                if (semEstoque == 1) {
                    escreverCentro(matrix, AVISO, 24, "Sem estoque desse ingrediente!", backardapio);
                }
                else if (ingredienteValido == 0) {
                    escreverCentro(matrix, AVISO, 24, "Ingrediente invalido!", backardapio);
                }
                else {
                    if (alturaPilha(pilhaa) == cardapio[pedidool].qntIngredientes) {
                        praqtantavar = 1;
                    }
                }
            }

            // ====================================
            //      VERIFICAÇÃO DOS ERROS
            // ====================================
            int erros = 0;
            for (int i = 0; i < alturaPilha(pilhaa); i++) {
                if (strcmp(pilhaa->item[i]->nome, cardapio[pedidool].ingredientes[i]) != 0)
                    erros++;
            }

            int ganho = (cardapio[pedidool].preco / (erros + 1));
            *dinheiro += ganho;

            rewind(data2);
            imprime_listad2salva(lista, data2, *dinheiro);
            fflush(data2);

            char msgFinal[150];
            sprintf(msgFinal,
                    "Pedido %d finalizado! Erros: %d | Dinheiro ganho: %d",
                    pedidoIndex + 1, erros, ganho);

            escreverCentro(matrix, forcoelba, 24, msgFinal, backardapio);

            showtime(*matrix);

            char pausa[10];
            inputinhoTXT(matrix, 25, 75, forpedro, pausa);

            inicializa_pilha(pilhaa);

       
        }
    }
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
        /*
        while (aux != NULL) {
            char cansei2[60];
            sprintf(cansei2, "Pedido n%d: %s", aux->id, cardapio[aux->idHamburguer - 1].nome);
            escreverCentro(matrix, forcoelba, (7 + SEILA), cansei2, backardapio);
            SEILA++;
            aux = aux->prox;
        }
        */
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
    escreverCentro(matrix, AVISO, 3, "CAIXA", backluz);
    escreverCentro(matrix, forcoelba, 7, "EM ANDAMENTO", backluz);
    escreverCentro(matrix, forcoelba, 15, "Por favor, informe sua senha: ", backluz);
    escreverCentro(matrix, forcoelba, 19, "Digite aqui:              ", backluz);
    showtime(*matrix);
    char senha[12];
    inputinhoTXT(matrix, 19, 49, forpedro, senha);
}

int conta_estoque(tp_listad *lista, ingrediente item) {
    if (listad_vazia(lista)) return 0;
    int count = 0;
    tp_no *atu = lista->ini;
    while (atu != NULL) {
        if (strcmp(atu->info.nome, item.nome) == 0) {
            count++;
        }
        atu = atu->prox;
    }
    return count;
}


void imprimirEstoqueDireita(tipomatrix *matrix, tp_listad *lista, char back[]) {
    int linha = 3;
    int coluna = largura - 25; // 25 colunas da borda direita

    // Título
    strcpy(matrix->espaco[linha][coluna], forcoelba);
    strcat(matrix->espaco[linha][coluna], "ESTOQUE");
    strcat(matrix->espaco[linha][coluna], resetar);
    linha += 2;

    // Ingredientes
    for (int i = 0; i < 15; i++) {
        int qtd = conta_estoque(lista, estoque[i]);

        char texto[60];
        sprintf(texto, "%-12s %3d", estoque[i].nome, qtd);

        int k = 0;
        while (texto[k] != '\0') {
            char c[2] = {texto[k], '\0'};
            strcpy(matrix->espaco[linha][coluna + k], c);
            k++;
        }

        linha++;
    }
}



void ADM_comprar(tipomatrix *matrix, tp_listad *lista, int *dinheiro, FILE *data2) {
    int pagina = 0;              // controla a página de itens (0 ou 1)
    int sair_compra = 0;         // flag para sair do menu
    char escolha[10];            // guarda a entrada do usuário
    char msg_erro[100] = "";     // mensagens de erro ou confirmação

    while (!sair_compra) {
        // limpa a tela e desenha o fundo
        printf(resetar);
        printf(apagar);
        cursor(0, 0);
        preencher(matrix, backozinha);

        escreverCentro(matrix, forozinha, 2, "--- COMPRAR INGREDIENTES ---", backozinha);

        // Mostra o saldo atual do jogador
        char dindin_str[50];
        sprintf(dindin_str, "Patocoins: %d", *dinheiro);
        escreverCentro(matrix, forcoelba, 4, dindin_str, backozinha);

        // Exibe de 8 em 8 ingredientes (paginação)
        int start = pagina * 8; 
        int end = (pagina == 0) ? 8 : 15;
        int linha_item = 6;

        for (int i = start; i < end; i++) {
            // Exibe o ID, nome e preço de cada ingrediente do estoque global
            char item_str[100];
            sprintf(item_str, "%d. %-20s - Preco: R$ %.2f", i, estoque[i].nome, estoque[i].preco);
            escreverCentro(matrix, forcoelba, linha_item, item_str, backozinha);
            linha_item++;
        }

        // Opções para o jogador
        escreverCentro(matrix, forcoelba, 20, "Digite ID (comprar), 'p' (prox), 'a' (ant), 's' (sair)", backozinha);
        escreverCentro(matrix, forcoelba, 22, "Digite aqui:              ", backozinha);
        
        // Exibe mensagem de erro se existir
        if (strlen(msg_erro) > 0) {
            escreverCentro(matrix, AVISO, 25, msg_erro, backozinha);
            strcpy(msg_erro, ""); 
        }

        showtime(*matrix); // mostra a interface atualizada
        inputinhoTXT(matrix, 22, 49, forpedro, escolha);

        // Converte tudo para minúsculo para evitar erro de digitação
        for(int i = 0; escolha[i]; i++) escolha[i] = tolower(escolha[i]);

        // --- Comandos de navegação ---
        if (strcmp(escolha, "s") == 0) { sair_compra = 1; continue; }
        else if (strcmp(escolha, "p") == 0) { pagina = 1; continue; }
        else if (strcmp(escolha, "a") == 0) { pagina = 0; continue; }

        // Converte o ID digitado em número
        int id = atoi(escolha);
        if (id < 0 || id > 14 || (id == 0 && strcmp(escolha, "0") != 0)) {
            sprintf(msg_erro, "ID '%s' invalido. Tente novamente.", escolha);
            continue;
        }

        // Pede a quantidade desejada
        escreverCentro(matrix, forcoelba, 22, "Quantidade:               ", backozinha);
        showtime(*matrix);
        char qtd_str[10];
        inputinhoTXT(matrix, 22, 49, forpedro, qtd_str);
        int qtd = atoi(qtd_str);

        if (qtd <= 0) {
            sprintf(msg_erro, "Quantidade '%s' invalida.", qtd_str);
            continue;
        }

        // Calcula o custo total da compra
        float custo = estoque[id].preco * qtd;
        if (*dinheiro < custo) {
            sprintf(msg_erro, "Dinheiro insuficiente! Custo: %.2f", custo);
            continue;
        }

        // --- ALOCAÇÃO DINÂMICA AQUI ---
        // Cada ingrediente comprado é adicionado dinamicamente à lista.
        // Isso significa que a função insere_listad_no_fim() faz malloc()
        // e adiciona um novo nó no final da lista encadeada.
        *dinheiro -= (int)custo; 
        for (int i = 0; i < qtd; i++) {
            insere_listad_no_fim(lista, estoque[id]);  // aloca e adiciona
        }

        // Atualiza o arquivo de log com o novo estoque e dinheiro
        rewind(data2);
        imprime_listad2salva(lista, data2, *dinheiro);
        fflush(data2);

        sprintf(msg_erro, "Compra de %d %s(s) realizada!", qtd, estoque[id].nome);
    }
}

void ADM_vender(tipomatrix *matrix, tp_listad *lista, int *dinheiro, FILE *data2) {
    int pagina = 0;
    int sair_venda = 0;
    char escolha[10];
    char msg_erro[100] = "";
    float multiplicador_venda = 0.5; // vende pela metade do preço original

    while (!sair_venda) {
        printf(resetar);
        printf(apagar);
        cursor(0, 0);
        preencher(matrix, backozinha);

        escreverCentro(matrix, forozinha, 2, "--- VENDER INGREDIENTES ---", backozinha);

        // Mostra o saldo atual antes de vender
        char dindin_str[50];
        sprintf(dindin_str, "Patocoins: %d", *dinheiro);
        escreverCentro(matrix, forcoelba, 4, dindin_str, backozinha);

        int start = pagina * 8;
        int end = (pagina == 0) ? 8 : 15;
        int linha_item = 6;

        // Mostra ingredientes e a quantidade atual de cada um
        for (int i = start; i < end; i++) {
            char item_str[100];
            int qtd_atual = conta_estoque(lista, estoque[i]);
            float preco_venda = estoque[i].preco * multiplicador_venda;
            sprintf(item_str, "%d. %-20s (Qtd: %d) - Vende por: R$ %.2f",
                    i, estoque[i].nome, qtd_atual, preco_venda);
            escreverCentro(matrix, forcoelba, linha_item, item_str, backozinha);
            linha_item++;
        }

        escreverCentro(matrix, forcoelba, 20, "Digite ID (vender), 'p' (prox), 'a' (ant), 's' (sair)", backozinha);
        escreverCentro(matrix, forcoelba, 22, "Digite aqui:              ", backozinha);
        
        if (strlen(msg_erro) > 0) {
            escreverCentro(matrix, AVISO, 25, msg_erro, backozinha);
            strcpy(msg_erro, "");
        }

        showtime(*matrix);
        inputinhoTXT(matrix, 22, 49, forpedro, escolha);

        for(int i = 0; escolha[i]; i++) escolha[i] = tolower(escolha[i]);

        // Navegação
        if (strcmp(escolha, "s") == 0) { sair_venda = 1; continue; }
        else if (strcmp(escolha, "p") == 0) { pagina = 1; continue; }
        else if (strcmp(escolha, "a") == 0) { pagina = 0; continue; }

        int id = atoi(escolha);
        if (id < 0 || id > 14 || (id == 0 && strcmp(escolha, "0") != 0)) {
            sprintf(msg_erro, "ID '%s' invalido. Tente novamente.", escolha);
            continue;
        }

        // Pergunta a quantidade que deseja vender
        escreverCentro(matrix, forcoelba, 22, "Quantidade:               ", backozinha);
        showtime(*matrix);
        char qtd_str[10];
        inputinhoTXT(matrix, 22, 49, forpedro, qtd_str);
        int qtd = atoi(qtd_str);

        if (qtd <= 0) {
            sprintf(msg_erro, "Quantidade '%s' invalida.", qtd_str);
            continue;
        }

        // Verifica se o jogador tem o suficiente no estoque
        int qtd_estoque = conta_estoque(lista, estoque[id]);
        if (qtd > qtd_estoque) {
            sprintf(msg_erro, "Estoque insuficiente! Você so tem %d.", qtd_estoque);
            continue;
        }

        // --- ALOCAÇÃO DINÂMICA REVERSA ---
        // Aqui é feita a remoção de nós da lista encadeada.
        // Cada chamada de remove_listad() dá free() em um nó da lista.
        int lucro = (int)(estoque[id].preco * multiplicador_venda * qtd);
        *dinheiro += lucro;
        for (int i = 0; i < qtd; i++) {
            remove_listad(lista, estoque[id]);  // libera o nó da lista
        }

        // Atualiza o arquivo com as novas informações
        rewind(data2);
        imprime_listad2salva(lista, data2, *dinheiro);
        fflush(data2);

        sprintf(msg_erro, "Venda de %d %s(s) realizada! Lucro: %d", qtd, estoque[id].nome, lucro);
    }
}

void ADM_ver_estoque(tipomatrix *matrix, tp_listad *lista, int *dinheiro) {
    printf(resetar);
    printf(apagar);
    cursor(0, 0);
    preencher(matrix, backozinha);
    escreverCentro(matrix, forozinha, 2, "--- ESTOQUE ATUAL ---", backozinha);
    char dindin_str[50];
    sprintf(dindin_str, "Patocoins Atuais: %d", *dinheiro);
    escreverCentro(matrix, forcoelba, 4, dindin_str, backozinha);

    int linha_atual = 6;
    for (int i = 0; i < 15; i++) {
        int qtd = conta_estoque(lista, estoque[i]);
        char item_str[100];
        sprintf(item_str, "%-20s: %d unidades", estoque[i].nome, qtd);
        escreverCentro(matrix, forcoelba, linha_atual, item_str, backozinha);
        linha_atual++;
    }
    
    // Espera ENTER pra voltar
    pausar(matrix, backozinha);
}

void ADM(tipomatrix *matrix, tp_listad *lista, int *dinheiro, FILE *data2){
    
    int sair_adm = 0;
    char escolha[12];
    char msg_erro[100] = "";

    while (!sair_adm)
    {
        printf(resetar);
        printf(apagar);
        cursor(0, 0);
        preencher(matrix, backozinha);
        escreverCentro(matrix, forozinha, 3, "SENAI CIMATEC", backozinha);
        escreverCentro(matrix, AVISO, 4, "PAINEL ADMINISTRATIVO", backozinha);
        
        char dindin_str[50];
        sprintf(dindin_str, "Patocoins Atuais: %d", *dinheiro);
        escreverCentro(matrix, forcoelba, 7, dindin_str, backozinha);

        escreverCentro(matrix, forcoelba, 10, "Bem-vindo a loja, Sanval/Soussa!", backozinha);
        
        escreverCentro(matrix, forcoelba, 14, "1. Comprar Ingredientes", backozinha);
        escreverCentro(matrix, forcoelba, 15, "2. Vender Ingredientes", backozinha);
        escreverCentro(matrix, forcoelba, 16, "3. Ver Estoque Atual", backozinha);
        escreverCentro(matrix, forcoelba, 18, "4. Sair do Painel", backozinha);
        
        escreverCentro(matrix, forcoelba, 20, "Digite aqui:              ", backozinha);

        if (strlen(msg_erro) > 0) {
            escreverCentro(matrix, AVISO, 25, msg_erro, backozinha);
            strcpy(msg_erro, "");
        }

        showtime(*matrix);
        inputinhoTXT(matrix, 20, 49, forpedro, escolha);

        if (strcmp(escolha, "1") == 0) {
            ADM_comprar(matrix, lista, dinheiro, data2);
        }
        else if (strcmp(escolha, "2") == 0) {
            ADM_vender(matrix, lista, dinheiro, data2);
        }
        else if (strcmp(escolha, "3") == 0) {
            ADM_ver_estoque(matrix, lista, dinheiro);
        }
        else if (strcmp(strcmp(escolha, "4") == 0 ? "sair" : escolha, "sair") == 0) {
            sair_adm = 1;
        }
        else {
            sprintf(msg_erro, "Opcao '%s' invalida!", escolha);
        }
    }
}

int main()
{

    struct Fila fila;
    inicializaFila(&fila);

    tp_pilha PILHA;
    inicializa_pilha(&PILHA);
    
    srand(time(NULL));

    hamburguer cardapio[10];
    hamburguers(cardapio);

    tp_listad *armazen;
    armazen = inicializa_listad();
    int dinheiro = 1000;

    FILE *data2;
    data2 = fopen("logy.txt", "w+");



    //O jogador vai começar com: 
    //1000 Patocoins
    //80 pães
    //20 carnes
    //30 baconzitos
    //40 queijos
    //60 alfaces
    //50 tomates
    //50 molhos especiais
    //60 cebolinhas
    //70 picles
    //50 aios
    //25 galinhas
    //55 coisas
    //10 shoyou
    //75 pau mofado
    //Sem fruta dos Deuses

    //Inserir na listade todos os ingredientes que o jogador começa
    for (int ingrid = 0; ingrid<80; ingrid++){
        insere_listad_no_fim(armazen, estoque[0]);
    }
    for (int ingrid = 0; ingrid<20; ingrid++){
        insere_listad_no_fim(armazen, estoque[1]);
    }
    for (int ingrid = 0; ingrid<30; ingrid++){
        insere_listad_no_fim(armazen, estoque[2]);
    }
    for (int ingrid = 0; ingrid<40; ingrid++){
        insere_listad_no_fim(armazen, estoque[3]);
    }
    for (int ingrid = 0; ingrid<60; ingrid++){
        insere_listad_no_fim(armazen, estoque[4]);
    }
    for (int ingrid = 0; ingrid<50; ingrid++){
        insere_listad_no_fim(armazen, estoque[5]);
    }
    for (int ingrid = 0; ingrid<50; ingrid++){
        insere_listad_no_fim(armazen, estoque[6]);
    }
    for (int ingrid = 0; ingrid<60; ingrid++){
        insere_listad_no_fim(armazen, estoque[7]);
    }
    for (int ingrid = 0; ingrid<70; ingrid++){
        insere_listad_no_fim(armazen, estoque[8]);
    }
    for (int ingrid = 0; ingrid<50; ingrid++){
        insere_listad_no_fim(armazen, estoque[9]);
    }
    for (int ingrid = 0; ingrid<25; ingrid++){
        insere_listad_no_fim(armazen, estoque[10]);
    }
    for (int ingrid = 0; ingrid<55; ingrid++){
        insere_listad_no_fim(armazen, estoque[11]);
    }
    for (int ingrid = 0; ingrid<10; ingrid++){
        insere_listad_no_fim(armazen, estoque[12]);
    }
    for (int ingrid = 0; ingrid<75; ingrid++){
        insere_listad_no_fim(armazen, estoque[13]);
    }
    for (int ingrid = 0; ingrid<100; ingrid++){
        insere_listad_no_fim(armazen, estoque[14]);
    }

    imprime_listad2salva(armazen, data2, dinheiro);

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
    escreverCentro(&comeco, forcoelba, 15, "--> cozinha <--", backode);
    escreverCentro(&comeco, forcoelba, 16, "--> caixa <--", backode);
    escreverCentro(&comeco, forcoelba, 17, "--> admin <--", backode);
    escreverCentro(&comeco, forcoelba, 21, "Digite aqui:              ", backode);
    showtime(comeco);
    char categoria[12];
    inputinhoTXT(&comeco, 21, 49, forpedro, categoria);
    int i = 0;
    while (categoria[i] != '\0') {
        categoria[i] = tolower(categoria[i]);
        i++;
    }

    FILE *data;
    data = fopen("cadastro.txt", "r");

    int b = 0;
    int socorro = 0;
    while (b == 0)
    {
        if (strcmp(categoria, "cliente") == 0)
        {
            CLIENTE(&comeco, cardapio);
            socorro = 1;
        }
        else if (strcmp(categoria, "cozinha") == 0)
{
    COZINHEIRO(&comeco, cardapio, &PILHA, data2, &dinheiro, armazen);
    socorro = 1;
}
        else if (strcmp(categoria, "caixa") == 0)
        {
            GERENTE(&comeco);
            socorro = 1;
        }
        else if (strcmp(categoria, "admin") == 0)
        {
            ADM(&comeco, armazen, &dinheiro, data2);
            socorro = 1;
        }
        printf(resetar);
        printf(apagar);
        cursor(0, 0);

        preencher(&comeco, backode);
        escreverCentro(&comeco, AVISO, 3, "Bem vindo ao Patoburger!", backode);
        escreverCentro(&comeco, forcoelba, 7, "Eu sou o P.A.T.O", backode);
        escreverCentro(&comeco, forcoelba, 8, "Programa Autonomo de Tutoria Orientada", backode);
        escreverCentro(&comeco, forcoelba, 10, "Hoje estarei te auxiliando nesta filial.", backode);
        escreverCentro(&comeco, forcoelba, 12, "Por favor, informe sua categoria de ingresso: ", backode); 
        escreverCentro(&comeco, forcoelba, 14, "--> cliente <--", backode);
        escreverCentro(&comeco, forcoelba, 15, "--> cozinha <--", backode);
        escreverCentro(&comeco, forcoelba, 16, "--> caixa <--", backode);
        escreverCentro(&comeco, forcoelba, 17, "--> admin <--", backode);
        escreverCentro(&comeco, forcoelba, 21, "Digite aqui:              ", backode);

        if (socorro == 0){
            escreverCentro(&comeco, AVISO, 25, "Por favor, informe uma categoria valida.", backode);
        }
        socorro = 0;

        showtime(comeco);
        inputinhoTXT(&comeco, 21, 49, forpedro, categoria);
        int i = 0;
        while (categoria[i] != '\0') {
            categoria[i] = tolower(categoria[i]);
            i++;
        }
    }

    fclose(data);
    fclose(data2);
    destroi_listad(armazen);

    FILE *consumoFile = fopen("consumo_diario.txt", "w");
    salvarConsumo(consumoDiario, consumoFile);
    fclose(consumoFile);


    return 0;
}
