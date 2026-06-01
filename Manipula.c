//
// Created by alcir on 29/05/2026.
//
#include <stdio.h>
#include "Arvore.h"
#include "HASH_PESSOAS.h"
#include "HASH_PESSOAS.c"

typedef struct {
    char nome[TAM_NOME_PESSOA];
    char funcao[TAM_FUNCAO];
    char personagem[TAM_ROLE];
    char filme[TAM_NOME_FILME];
}RELACAO_TEMP;


typedef struct {
    char titulo[TAM_NOME_FILME];
    int  ano;
    char subtitulo[TAM_SUBTITULO_FILME];
}FILME_TEMP;

typedef struct {
    char nome[TAM_NOME_PESSOA];
    int  data_nascimento;
} PESSOA_TEMP;

// funcoes para ler no do arquivo arvore

ARVORE *le_no(FILE* index,long offset_no,int t) {
    fseek(index,offset_no,SEEK_CUR);
    ARVORE *aux = TARVBM_cria(t);
    fread(&aux->nchaves,sizeof(int),1,index);
    fread(&aux->folha,sizeof(int),1,index);
    for (int i=0; i<aux->nchaves; i++) {
        aux->chaves[i] = (CHAVE*)malloc(sizeof(CHAVE));
        fread(&aux->chaves[i],sizeof(int),1,index);
    }
    aux->offset_filho =(long*)malloc(sizeof(long)*2*t);
    for (int i=0; i<(2*t);i++) {
        fread(&aux->offset_filho[i],sizeof(long),1,index);
    }
    fseek(index,((2*t-1)-aux->nchaves)*sizeof(CHAVE),SEEK_CUR);
    fread(&aux->offset_prox,sizeof(long),1,index);
    return aux;
}

void escreve_no(FILE *index,ARVORE *no,long offset_no,int t) { //escreve um no no arquivo de index
    fseek(index,offset_no,SEEK_SET);
    fwrite(&no->nchaves,sizeof(int),1,index);
    fwrite(&no->folha,sizeof(int),1,index);
    for (int i=0; i<no->nchaves;i++){
        fwrite(no->chaves[i],sizeof(CHAVE),1,index);
    }
    char place_holder[sizeof(CHAVE)*((2*t-1)-no->nchaves)];
    fwrite(place_holder,sizeof(CHAVE),1,index);

    for (int i= 0;i<(2*t);i++) {
        fwrite(&no->offset_filho[i],sizeof(long),1,index);
    }
    fwrite(&no->offset_prox,sizeof(long),1,index);
}

// achei meio desnecessaria melhor tratar dentro do proprio funcao de ler pessoa e filme do arquivo nnode
// void le_ate_encontrar_barra(char *linha,char* info,int *i) {
//     int j = 0;
//     while (linha[*i] != '|') {
//         if (linha[*i] != ' ') {
//             info[j] = linha[*i];
//             j++;
//         }
//         *(i++);
//     }
//
//     info[j++] = '\0';
// }


// funcoes para ler do arq nodes e  relationship
void trim(char *s) {

    int j = strlen(s) - 1;
    while (j >= 0 && (s[j] == ' ' )) {
        s[j--] = '\0';
    }

    int i = 0;
    while (s[i] && (s[i] == ' ')) {
        i++;
    }

    if (i > 0) {
        int k = 0;
        while (s[i]) {
            s[k++] = s[i++];
        }
        s[k] = '\0';
    }
}
void le_pessoa(char *linha,PESSOA_TEMP *p){
    char tipo[20];
    char ano_str[20];

    sscanf(linha, "%[^|] | %[^|] | %[^\n]", tipo, p->nome, ano_str);

    trim(p->nome);
    trim(ano_str);

    if (ano_str[0] == '(')
        p->data_nascimento = -1;
    else
        p->data_nascimento = atoi(ano_str);
}

void le_filme(char *linha,FILME_TEMP *filme){
    char tipo[20];
    char ano_str[20];
    sscanf(linha, "%[^|] | %[^|] | %[^|] | %[^|\n]",tipo,filme->titulo,ano_str,filme->subtitulo);

    trim(filme->titulo);
    trim(ano_str);
    trim(filme->subtitulo);

    filme->ano = atoi(ano_str);
    if (filme->subtitulo[0] == '(') {
        filme->subtitulo[0] = '\0';
    }
}

void le_relacao(char *linha,RELACAO_TEMP *relacao) {
    int lidos = sscanf(linha,"*%[^|] | %[^|] | %[^|] | *%[^|] | %[^|] | %[^\n]",relacao->nome,relacao->funcao,relacao->filme,relacao->personagem);
    if (lidos < 4 ) {
        relacao->personagem[0] = '\0';
    }
    trim(relacao->nome);
    trim(relacao->funcao);
    trim(relacao->filme);
    trim(relacao->personagem);
}

//salvar no, carregar no arquivo de dados
//caminhar pela arvore ate achar o filme desejado na folha estara o offset achou a chave va para a folha pega o offset e vai no arq de dados,
//acessar  um no da arvore carrega pra memoria, salvar um filho colocar no final do arquivo,
//sempre que for escrever um novo no no arq index retornar onde foi escrito
