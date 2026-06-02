//
// Created by luiz-p on 25/05/2026.
//

#ifndef TRABALHO_RO7_ARVORE_H
#define TRABALHO_RO7_ARVORE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TAM_NOME_FILME 200
#define TAM_SUBTITULO_FILME 400
#define TAM_NOME_PESSOA 100
#define TAM_FUNCAO 9
#define TAM_ROLE 100
#define INDEX "Index.dat"

typedef struct {
    int t;
    long offset_raiz;
}CABECALHO;

typedef struct chaves {
    char id_no[TAM_NOME_FILME];
    long offset_dados;
}CHAVE;

typedef struct arvore{
    int nchaves;
    int eh_folha;
    CHAVE *chaves; //mudar para chaves*
    long *offset_filho;
    long offset_prox;
}ARVORE;


ARVORE *cria(int t);
ARVORE *TARVBM_inicializa(void);
ARVORE *TARVBM_busca(ARVORE *no, char *filme);
ARVORE *TARVBM_insere(ARVORE *T, int mat, int t);
ARVORE* TARVBM_retira(ARVORE* arv, int k, int t);
void TARVBM_libera(ARVORE *a,long offset_no);
void TARVBM_imprime(ARVORE *a);
void TARVBM_imprime_chaves(ARVORE *a);
#endif //TRABALHO_RO7_ARVORE_H
