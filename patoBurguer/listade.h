#ifndef LISTADE_H
#define LISTADE_H

#include <stdio.h>
#include <stdlib.h>
#include "ingredientes.h"

/* Definimos o tipo dos dados armazenados na lista.
   Aqui é int, mas poderia ser float, struct etc. */
typedef ingrediente tp_item;

/* 
   Estrutura do nó da lista duplamente encadeada
   Cada nó tem:
   - ant: ponteiro para o nó anterior
   - info: valor armazenado
   - prox: ponteiro para o próximo nó
*/
typedef struct tp_no_aux {
    struct tp_no_aux *ant;   // nó anterior
    tp_item info;            // valor do nó
    struct tp_no_aux *prox;  // nó seguinte
} tp_no;

/* 
   Estrutura do descritor da lista
   Ele guarda ponteiros para o início e o fim da lista
*/
typedef struct {
    tp_no *ini;  // ponteiro para o primeiro nó
    tp_no *fim;  // ponteiro para o último nó
    // int tamanho; // opcional, para guardar o tamanho da lista
} tp_listad;

/* 
   Inicializa a lista
   - Aloca memória para o descritor
   - Inicializa ini e fim como NULL (lista vazia)
   - Retorna o ponteiro para o descritor
*/
tp_listad *inicializa_listad() {
    tp_listad *lista = (tp_listad*) malloc(sizeof(tp_listad)); // aloca memória
    if (!lista) return NULL;   // checa se a alocação falhou
    lista->ini = NULL;         // lista começa vazia
    lista->fim = NULL;
    // lista->tamanho = 0;      // opcional
    return lista;              // retorna ponteiro para o descritor
}

/* 
   Verifica se a lista está vazia
   Retorna 1 se estiver vazia ou não inicializada, 0 caso contrário
*/
int listad_vazia(tp_listad *lista) {
    if (lista == NULL) return 1;        // lista não inicializada → vazia
    if (lista->ini == NULL) return 1;   // sem nós → vazia
    return 0;
}

/* 
   Aloca um nó da lista
   Retorna o ponteiro para o nó alocado
*/
tp_no *aloca() {
    tp_no* pt = (tp_no*) malloc(sizeof(tp_no)); // aloca memória
    return pt;  // retorna ponteiro
}

/* 
   Insere um nó no FIM da lista
   Parâmetros:
   - lista: ponteiro para o descritor
   - e: valor a ser inserido
   Retorna 1 se inseriu, 0 se falhou
*/
int insere_listad_no_fim(tp_listad *lista, tp_item e) {
    if (lista == NULL) return 0;     // não inicializada
    tp_no *novo = aloca();           // aloca memória para novo nó
    if (!novo) return 0;             // falha na alocação
    novo->info = e;                  // guarda o valor

    if (listad_vazia(lista)) {       // se a lista estiver vazia
        novo->prox = NULL;           // próximo é NULL (último nó)
        novo->ant = NULL;            // anterior é NULL (primeiro nó)
        lista->ini = lista->fim = novo; // primeiro e último nó são o mesmo
    } else {                         // lista já tem elementos
        novo->prox = NULL;           // novo será o último → prox = NULL
        novo->ant = lista->fim;      // anterior aponta para antigo último nó
        lista->fim->prox = novo;    // antigo último nó aponta para o novo
        lista->fim = novo;           // atualiza ponteiro do fim da lista
    }
    // lista->tamanho++;              // opcional
    return 1;                        // sucesso
}

/* 
   Imprime a lista
   Parâmetros:
   - lista: ponteiro para o descritor
   - ordem: 1 = do início ao fim, 2 = do fim ao início
*/
void imprime_listad(tp_listad *lista, int ordem) {
    if (lista == NULL) {
        printf("Lista nao inicializada\n");
        return;
    }

    tp_no *atu;
    switch (ordem) {
        case 1:
            atu = lista->ini;
            while (atu != NULL) {
                printf("%s (Qtd: %d, Preço: %.2f)\n",
                       atu->info.nome, atu->info.quantEstoque, atu->info.preco);
                atu = atu->prox;
            }
            break;
        case 2:
            atu = lista->fim;
            while (atu != NULL) {
                printf("%s (Qtd: %d, Preço: %.2f)\n",
                       atu->info.nome, atu->info.quantEstoque, atu->info.preco);
                atu = atu->ant;
            }
            break;
        default:
            printf("codigo invalido");
    }
    printf("\n");
}

/* 
   Remove um nó com valor 'e'
   Retorna 1 se removido, 0 se não encontrado
*/
int remove_listad(tp_listad *lista, tp_item e) {
    if (lista == NULL) return 0;
    tp_no *atu = lista->ini;
    while ((atu != NULL) && (strcmp(atu->info.nome, e.nome) != 0)) {
        atu = atu->prox;
    }
    if (atu == NULL) return 0;

    if (lista->ini == lista->fim) {
        lista->ini = lista->fim = NULL;
    } else if (lista->ini == atu) {
        lista->ini = atu->prox;
        lista->ini->ant = NULL;
    } else if (lista->fim == atu) {
        lista->fim = atu->ant;
        lista->fim->prox = NULL;
    } else {
        atu->prox->ant = atu->ant;
        atu->ant->prox = atu->prox;
    }

    free(atu);
    return 1;
}


/* 
   Busca um nó com valor 'e'
   Retorna ponteiro para o nó encontrado ou NULL se não existir
*/
tp_no *busca_listade(tp_listad *lista, tp_item e) {
    if (lista == NULL) return NULL;
    tp_no *atu = lista->ini;
    while ((atu != NULL) && (strcmp(atu->info.nome, e.nome) != 0)) {
        atu = atu->prox;
    }
    return atu;
}

/* 
   Destroi a lista: libera todos os nós e o descritor
   Retorna NULL
*/
tp_listad *destroi_listad(tp_listad *lista) {
    if (lista == NULL) return NULL;
    tp_no *atu = lista->ini;
    tp_no *tmp;
    while (atu != NULL) {
        tmp = atu->prox; // guarda próximo nó antes de liberar
        free(atu);       // libera memória do nó atual
        atu = tmp;       // vai para o próximo
    }
    free(lista);         // libera o descritor
    return NULL;         // retorna NULL para indicar lista destruída
}

#endif
