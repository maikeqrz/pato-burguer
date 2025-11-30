#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vendas.h"



extern struct ingrediente estoque[]; 


static const int INITIAL_COUNTS[15] = {
    80, 20, 30, 40, 60, 50, 50, 60, 70, 50, 25, 55, 10, 75, 100
};

typedef struct {
    int idx;
    int inicial;
    int atual;
    int vendido;
    const char *nome;
} VendaInfo;

static int cmp_desc_vendido(const void *a, const void *b) {
    const VendaInfo *va = (const VendaInfo*)a;
    const VendaInfo *vb = (const VendaInfo*)b;
    return vb->vendido - va->vendido; 
}

void mostrar_vendas_falencia(const char *log_path) {
    FILE *f = fopen(log_path, "r");
    if (!f) {
        fprintf(stderr, "\n[Patoburger] Aviso: nao foi possivel abrir '%s' para leitura.\n", log_path);
        fprintf(stderr, "[Patoburger] Certifique-se de que o arquivo de log foi gravado pelo jogo.\n\n");
        return;
    }

    int dinheiro = 0;
    if (fscanf(f, "%d", &dinheiro) != 1) {
        fclose(f);
        fprintf(stderr, "\n[Patoburger] Formato invalido em '%s' (esperado dinheiro na 1a linha).\n\n", log_path);
        return;
    }

    VendaInfo infos[15];
    for (int i = 0; i < 15; ++i) {
        int qtd = 0;
        if (fscanf(f, "%d", &qtd) != 1) {
            qtd = 0;
        }
        infos[i].idx = i;
        infos[i].inicial = INITIAL_COUNTS[i];
        infos[i].atual = qtd;
        int vendido = infos[i].inicial - infos[i].atual;
        if (vendido < 0) vendido = 0;
        infos[i].vendido = vendido;

        if (estoque && estoque[i].nome && estoque[i].nome[0] != '\0') {
            infos[i].nome = estoque[i].nome;
        } else {
            infos[i].nome = "(ingrediente sem nome)";
        }
    }
    fclose(f);


    qsort(infos, 15, sizeof(VendaInfo), cmp_desc_vendido);


    printf("\n=============================================================\n");
    printf("         PATO BURGER - RELATÓRIO FINAL (CHEGADA À FALÊNCIA)\n");
    printf("=============================================================\n");
    printf("Caixa final (Patocoins): %d\n", dinheiro);
    printf("-------------------------------------------------------------\n");
    printf("%-3s  %-28s  %8s  %8s\n", "N", "ITEM (Hamburgueria)", "VENDIDOS", "RESTO");
    printf("-------------------------------------------------------------\n");

    int total_vendidos = 0;
    for (int i = 0; i < 15; ++i) {
        printf("%2d.   %-28s   %8d   %8d\n",
               infos[i].idx + 1,
               infos[i].nome ? infos[i].nome : "(sem nome)",
               infos[i].vendido,
               infos[i].atual);
        total_vendidos += infos[i].vendido;
    }

    printf("-------------------------------------------------------------\n");
    printf("TOTAL UNIDADES VENDIDAS (todas as categorias): %d\n", total_vendidos);
    printf("=============================================================\n\n");
}