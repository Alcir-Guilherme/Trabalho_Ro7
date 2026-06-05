#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Arvore.h"
int main() {
    int t;
    printf("Digite qual será o t: ");
    scanf("%d", &t);
    inicializa_arquivos("../index.dat","../dados.dat","../raiz.dat",t);
    //FILE *fp = fopen("raiz.dat","rb");
    CHAVE c;
    for(int i=0;i<30;i++) {
        c.id_no[0] = i + 65;
        c.id_no[1] = '\0';
        c.offset_dados =99;
        arvore_insere("../index.dat","../raiz.dat",&c);
    }
    arvore_imprime("../index.dat","../raiz.dat");

    // unsigned char c;
    //
    // printf("Bytes do arquivo:\n");
    //
    // while(fread(&c,1,1,fp) == 1)
    //     printf("%02X ", c);
    //
    // printf("\n");
    //
    // fclose(fp);
    // FILE *raiz;
    // raiz=fopen("raiz.dat","rb");
    // printf("sizeof(int) = %zu\n", sizeof(int));
    // printf("sizeof(long) = %zu\n", sizeof(long));
    // printf("sizeof(CABECALHO) = %zu\n", sizeof(CABECALHO));
    // CABECALHO cab;
    // carrega_raiz(raiz,&cab);
    // printf("t = %d\n", cab.t);
    // printf("offset = %ld\n", cab.offset_raiz);
    //
    //
    // printf("\n");
    // printf("%d\n",cab.t);
    // printf("%ld",cab.offset_raiz);
    // fclose(raiz);
    // ARVORE *folha = cria_no(t);
    //
    // folha->nchaves = 3;
    // folha->eh_folha = 1;
    //
    // strcpy(folha->chaves[0].id_no,"Apollo 13");
    // folha->chaves[0].offset_dados = 100;
    //
    // strcpy(folha->chaves[1].id_no,"Cast Away");
    // folha->chaves[1].offset_dados = 200;
    //
    // strcpy(folha->chaves[2].id_no,"Top Gun");
    // folha->chaves[2].offset_dados = 300;
    //
    // printf("\nNO CRIADO EM MEMORIA\n");
    //
    // for(int i=0;i<folha->nchaves;i++){
    //     printf(
    //         "%s -> %ld\n",
    //         folha->chaves[i].id_no,
    //         folha->chaves[i].offset_dados
    //     );
    // }
    //
    // //--------------------------
    // // grava no indice
    // //--------------------------
    //
    // FILE *fp_index = fopen("index.dat","rb+");
    //
    // long offset = fim_arquivo(fp_index);
    //
    // printf("\nGravando no offset %ld\n",offset);
    //
    // salva_no(
    //     folha,
    //     fp_index,
    //     offset,
    //     t
    // );
    //
    // fclose(fp_index);
    //
    // libera_no(folha);
    //
    // //--------------------------
    // // recarrega do disco
    // //--------------------------
    //
    // fp_index = fopen("index.dat","rb");
    //
    // ARVORE *teste = cria_no(t);
    //
    // carrega_no(
    //     teste,
    //     fp_index,
    //     offset,
    //     t
    // );
    //
    // printf("\nNO LIDO DO DISCO\n");
    //
    // printf("nchaves = %d\n",teste->nchaves);
    // printf("eh_folha = %d\n",teste->eh_folha);
    //
    // for(int i=0;i<teste->nchaves;i++){
    //     printf(
    //         "%s -> %ld\n",
    //         teste->chaves[i].id_no,
    //         teste->chaves[i].offset_dados
    //     );
    // }
    //
    // fclose(fp_index);
    //
    // libera_no(teste);
    //
    // printf("\nTeste concluido.\n");

    return 0;

}