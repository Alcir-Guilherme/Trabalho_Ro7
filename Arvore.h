#ifndef TRABALHO_RO7_ARVORE_H
#define TRABALHO_RO7_ARVORE_H

#define ARQ_INDEX "index.dat"
#define ARQ_DADOS "dados.dat"
#define ARQ_RAIZ "raiz.dat"
#define TAM_NOME 100
#define TAM_NOME_FILME      200
#define TAM_SUBTITULO_FILME 400
#define TAM_NOME_PESSOA     100
#define TAM_ROLE            100

#include <stdio.h>

typedef struct {
    int t;
    long offset_raiz;
}CABECALHO;

typedef struct chaves {
    char id_no[TAM_NOME];
    long offset_dados;
}CHAVE;

typedef struct arvore{
    int nchaves;
    int eh_folha;
    CHAVE *chaves;
    long *offset_filho;
    long offset_prox;
}ARVORE;




ARVORE *cria_no(int t);
ARVORE *TARVBM_busca(ARVORE *no, char *filme);
ARVORE *arvore_retira(ARVORE* arv, int k, int t);

void inicializa_arquivos(char *index, char *dados, char *raiz, int t);
void arvore_inicializa(char *index, char *dados,char *raiz,int t);
void arvore_insere(char *index, char *raiz, CHAVE *chave);
void arvore_remover(char *index, char *raiz, char *nome)
void arvore_libera(ARVORE *a,long offset_no);
void arvore_imprime_chaves(ARVORE *a);
void carrega_raiz(FILE *raiz,CABECALHO *cab);
void arvore_divisao(ARVORE *x,int i,ARVORE *y,FILE *fp_index,int t);
void arvore_imprime(char *index,char *raiz);
void libera_no(ARVORE *no);
void carrega_no(ARVORE *no,FILE *index,long offset_no, int t);
void salva_no(ARVORE *no, FILE *index, long offset_no,int t);
long fim_arquivo(FILE *arquivo); //passar para a manipula
#endif //TRABALHO_RO7_ARVORE_H
