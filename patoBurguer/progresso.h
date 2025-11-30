#ifndef PROGRESSO_H
#define PROGRESSO_H

typedef struct {
    int idHamburguerAtual;     // qual hambúrguer está sendo montado
    int etapa;                 // índice do ingrediente atual
    int ativo;                 // 0 = nenhum processo acontecendo, 1 = em progresso
} ProgressoCozinha;

#endif
