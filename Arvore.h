//
// Created by luiz-p on 25/05/2026.
//

#ifndef TRABALHO_RO7_ARVORE_H
#define TRABALHO_RO7_ARVORE_H
#include <stdio.h>
#include <stdlib.h>

typedef struct chaves {
    int chave;
    long offset;
}CHAVE;

typedef struct arvore{
    int nchaves, folha;
    CHAVE *chaves;
    struct arvore **filho, *prox;
}ARVORE;


ARVORE *TARVBM_cria(int t);
ARVORE *TARVBM_inicializa(void);
ARVORE *TARVBM_busca(ARVORE *a, int mat);
ARVORE *TARVBM_insere(ARVORE *T, int mat, int t);
ARVORE* TARVBM_retira(ARVORE* arv, int k, int t);
void TARVBM_libera(ARVORE *a);
void TARVBM_imprime(ARVORE *a);
void TARVBM_imprime_chaves(ARVORE *a);
#endif //TRABALHO_RO7_ARVORE_H
