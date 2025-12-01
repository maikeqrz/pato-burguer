

#include <stdio.h>
#include "vendas.h"

int main(void) {

    int dinheiro = 0; 


    if (dinheiro <= 0) {
        printf("\n[Patoburger] Falência detectada. Gerando relatório de vendas...\n");
        mostrar_vendas_falencia(LOG_PATH); // LOG_PATH definido em vendas.h como "logy.txt"
    }



    return 0;
}