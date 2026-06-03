#ifndef TRABALHO_RO7_ARVORE_H
#define TRABALHO_RO7_ARVORE_H

#define ARQ_INDEX "index.dat"
#define ARQ_DADOS "dados.dat"
#define ARQ_RAIZ "raiz.dat"
#define TAM_NOME 100



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

ARVORE *cria(int t);
void arvore_inicializa(char *index, char *dados,char *raiz,int t);
ARVORE *TARVBM_busca(ARVORE *no, char *filme);
void arvore_insere(ARVORE *no, char *index, char *raiz, CHAVE *chave);
ARVORE *arvore_retira(ARVORE* arv, int k, int t);
void arvore_libera(ARVORE *a,long offset_no);
void arvoreimprime(ARVORE *a);
void arvore_imprime_chaves(ARVORE *a);
#endif //TRABALHO_RO7_ARVORE_H
