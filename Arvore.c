#include "Arvore.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void inicializa_arquivos(char *index, char *dados, char *raiz, int t) {
  FILE *fp = fopen(index,"wb");
  if (!fp) exit(EXIT_FAILURE);
  fclose(fp);
  fp = fopen(dados,"wb");
  if (!fp) exit(EXIT_FAILURE);
  fclose(fp);
  fp = fopen(raiz,"wb");
  if(!fp) exit(EXIT_FAILURE);
  CABECALHO cab;
  cab.t = t;
  cab.offset_raiz = -1;
  fwrite(&cab,sizeof(CABECALHO),1,fp);

  fclose(fp);
}
//----------------------------------------------------------------------------------------------------------
void atualiza_raiz(FILE *raiz,CABECALHO *cab) {
  fseek(raiz,0,SEEK_SET);
  fwrite(cab,sizeof(CABECALHO),1,raiz);

}
//----------------------------------------------------------------------------------------------------------

long fim_arquivo(FILE *arquivo) {
  fseek(arquivo,0,SEEK_END);
  return ftell(arquivo);
}
long grava_novo_no(FILE *fp_index,ARVORE *no,int t){
  long offset = fim_arquivo(fp_index);

  salva_no(no, fp_index, offset, t);

  return offset;
}

//----------------------------------------------------------------------------------------------------------
void libera_no(ARVORE *no){
  free(no->chaves);
  free(no->offset_filho);
  free(no);
}
//----------------------------------------------------------------------------------------------------------
void arvore_inicializa(char *index, char *dados,char *raiz,int t) {
  FILE *fp = fopen(index,"wb");
  if(!fp) exit(EXIT_FAILURE);
  fclose(fp);

  fp = fopen(raiz,"wb");
  CABECALHO cab;
  cab.offset_raiz = -1;
  cab.t = t;
  fwrite(&cab,sizeof(CABECALHO),1,fp);
  if(!fp) exit(EXIT_FAILURE);
  fclose(fp);

  fp = fopen(dados,"wb");
  if(!fp) exit(EXIT_FAILURE);
  fclose(fp);
}
//----------------------------------------------------------------------------------------------------------
ARVORE *cria_no(int t){

  ARVORE* novo = (ARVORE*)malloc(sizeof(ARVORE));

  novo->nchaves = 0;
  novo->chaves = (CHAVE*)malloc(sizeof(CHAVE)*((t*2)-1));
  memset(novo->chaves,0,sizeof(CHAVE)*((t*2)-1)); // seta as chaves para string vazia e offset 0
  novo->eh_folha = 1;
  novo->offset_filho = (long*)malloc(sizeof(long)*t*2);
  int i;
  for(i=0; i<(t*2); i++) novo->offset_filho[i] = -1;
  novo->offset_prox = -1;
  return novo;
}
//----------------------------------------------------------------------------------------------------------
void carrega_no(ARVORE *no,FILE *index,long offset_no, int t) {
  fseek(index,offset_no,SEEK_SET); //mudar para seek_set
  fread(&no->nchaves, sizeof(int), 1, index);
  fread(&no->eh_folha,sizeof(int),1,index);
  fread(no->chaves,sizeof(CHAVE),2*t-1,index);// isso ta certo?
  fread(no->offset_filho,sizeof(long),2*t,index);
  fread(&no->offset_prox,sizeof(long),1,index);
}

//----------------------------------------------------------------------------------------------------------
void salva_no(ARVORE *no, FILE *index, long offset_no,int t) {
  fseek(index,offset_no,SEEK_SET);
  fwrite(&no->nchaves, sizeof(int), 1, index);
  fwrite(&no->eh_folha,sizeof(int),1,index);
  fwrite(no->chaves,sizeof(CHAVE),2*t-1,index);// isso ta certo?
  fwrite(no->offset_filho,sizeof(long),2*t,index);
  fwrite(&no->offset_prox,sizeof(long),1,index);
}

//----------------------------------------------------------------------------------------------------------
void libera(char *index,char *dados,char *raiz){
  remove(index);
  remove(dados);
  remove(raiz);
}


//----------------------------------------------------------------------------------------------------------
void carrega_raiz(FILE *raiz,CABECALHO *cab) {
  fseek(raiz,0,SEEK_SET);
  fread(cab,sizeof(CABECALHO),1,raiz);
}

//----------------------------------------------------------------------------------------------------------
long busca_no_aux(ARVORE *no,FILE *index, char *nome,int t, long offset_no) {
  if (offset_no == -1) return -1;

  carrega_no(no,index,offset_no,t);
  int i = 0;
  while ((i < no->nchaves) && (strcmp(nome,no->chaves[i].id_no) > 0)) i++;

  if (no->eh_folha) {
    if ((i < no->nchaves) && (strcmp(nome,no->chaves[i].id_no) == 0)) {
      return no->chaves[i].offset_dados;
    }
    printf("O filme não está na arvore.\n");
    return -1;
  }

  return busca_no_aux(no,index,nome,t,no->offset_filho[i]); // ver com luiz o que fazer nessa situacao
}
//---------------------------------------------------------------------------------------------------------
long busca_no(FILE *fp_index,CABECALHO *cab, char *nome) {
  ARVORE *novo = cria_no(cab->t);
  long resp = busca_no_aux(novo,fp_index,nome,cab->t,cab->offset_raiz);
  libera_no(novo);
  return resp;
}

//----------------------------------------------------------------------------------------------------------
void insere_nao_completo(ARVORE *x, FILE* index,CHAVE *chave, long offset_x,int t) {
  int i = x->nchaves-1;
  if (x->eh_folha) {
    while ((i >= 0) && (strcmp(chave->id_no,x->chaves[i].id_no) < 0)) {
      memcpy(&(x->chaves[i+1]), &(x->chaves[i]),sizeof(CHAVE));
      i--;
    }
    memcpy(&(x->chaves[i+1]),chave,sizeof(CHAVE));
    x->nchaves++;
    salva_no(x,index,offset_x,t);
    return;
  }

  while((i>=0) && (strcmp(chave->id_no,x->chaves[i].id_no)) < 0) i--;
  i++;
  ARVORE *no = cria_no(t);
  carrega_no(no,index,x->offset_filho[i],t);

  if (no->nchaves == (2*t)-1) {
    arvore_divisao(x,(i+1),no,index,t);
    salva_no(x,index,offset_x,t);
    salva_no(no,index,x->offset_filho[i],t);
    if (strcmp(chave->id_no,x->chaves[i].id_no) > 0) {
      i++;
      libera_no(no);
      no = cria_no(t);
      carrega_no(no,index,x->offset_filho[i],t);
    }
  }
  insere_nao_completo(no,index,chave,x->offset_filho[i],t);
  libera_no(no);
}

//----------------------------------------------------------------------------------------------------------
void arvore_insere(char *index, char *raiz, CHAVE *chave) {
  FILE *fp_raiz = fopen(raiz,"rb+");
  if(!fp_raiz) {exit(EXIT_FAILURE);}

  FILE *fp_index = fopen(index,"rb+");
  if(!fp_index) exit(EXIT_FAILURE);

  CABECALHO cab;
  carrega_raiz(fp_raiz,&cab);
  ARVORE *T = cria_no(cab.t);

  long offset_buscado = busca_no(fp_index,&cab,chave->id_no);

  // no presente na arvore
  if(offset_buscado != -1) {
    fclose(fp_raiz);
    fclose(fp_index);
    libera_no(T);
    return;
  }


  if(cab.offset_raiz == -1) {
    memcpy(&(T->chaves[0]),chave,sizeof(CHAVE));
    T->nchaves =1;
    long offset_final = fim_arquivo(fp_index);
    salva_no(T,fp_index,offset_final,cab.t);
    cab.offset_raiz = offset_final;
    atualiza_raiz(fp_raiz,&cab);
    fclose(fp_index);
    fclose(fp_raiz);
    libera_no(T);
    return;
  }

  carrega_no(T,fp_index,cab.offset_raiz,cab.t);

  if(T->nchaves == (2*cab.t)-1) {
    ARVORE *S = cria_no(cab.t);
    S->eh_folha = 0;
    S->offset_filho[0] = cab.offset_raiz;
    arvore_divisao(S,1,T,fp_index,cab.t);
    long fim_arq =fim_arquivo((fp_index));
    salva_no(S,fp_index,fim_arq,cab.t);
    salva_no(T,fp_index,cab.offset_raiz,cab.t);
    cab.offset_raiz = fim_arq;
    atualiza_raiz(fp_raiz,&cab);
    insere_nao_completo(S,fp_index,chave,fim_arq,cab.t);
    cab.offset_raiz = fim_arq; // altero o offset da raiz ja que inseri o S no final
    atualiza_raiz(fp_raiz,&cab);
    libera_no(S);
    libera_no(T);
    fclose(fp_raiz);
    fclose(fp_index);
    return;
  }

  insere_nao_completo(T,fp_index,chave,cab.offset_raiz,cab.t);

  libera_no(T);
  fclose(fp_index);
  fclose(fp_raiz);
}
//----------------------------------------------------------------------------------------------------------
void arvore_divisao(ARVORE *x,int i,ARVORE *Y,FILE *fp_index,int t) {
  ARVORE *Z = cria_no(t);
  Z->eh_folha = Y->eh_folha;
  long offset_Z = fim_arquivo(fp_index);

  int j;
  if (!Y->eh_folha) {
    Z->nchaves = t-1;
    for (j =0;j <t-1;j++) memcpy(&(Z->chaves[j]),&(Y->chaves[j+t]),sizeof(CHAVE));

    for (j = 0; j<t ; j++){
      Z->offset_filho[j] = Y->offset_filho[j+t];
      Y->offset_filho[j+t] = -1;
    }
  }

  else {
    Z->nchaves = t;
    for (j = 0; j<t ; j++) memcpy(&(Z->chaves[j]),&(Y->chaves[j+t-1]),sizeof(CHAVE));
    Z->offset_prox = Y->offset_prox;
    Y->offset_prox = offset_Z;
  }

  Y->nchaves = t-1;
  for(j=x->nchaves; j>=i; j--) x->offset_filho[j+1]=x->offset_filho[j];
  x->offset_filho[i] = offset_Z;
  for(j=x->nchaves; j>=i; j--) memcpy(&(x->chaves[j]),&(x->chaves[j-1]),sizeof(CHAVE));
  memcpy(&(x->chaves[i-1]), &(Y->chaves[t-1]),sizeof(CHAVE));
  x->nchaves++;
  salva_no(Z,fp_index,offset_Z,t);
  libera_no(Z);
  }

//---------------------------------------------------------------------------------------------------------------

void imp(long offset,FILE *index, int t, int andar){
  if(offset!=-1){
    ARVORE *a = cria_no(t);
    carrega_no(a,index,offset,t);
    int i,j;
    imp(a->offset_filho[a->nchaves],index,t,andar+1);
    for(i=a->nchaves-1; i >= 0; i--){
      for(j=0; j<=andar; j++) printf("\t");
      printf("%s\n", a->chaves[i].id_no);
      imp(a->offset_filho[i],index,t,andar+1);
    }
    libera_no(a);
  }
}
//----------------------------------------------------------------------------------------------------------------
void arvore_imprime(char* index,char* raiz){
  FILE* fp_index = fopen(index,"rb");
  if(!fp_index){ exit(EXIT_FAILURE);}
  FILE* fp_raiz = fopen(raiz,"rb");
  if(!fp_raiz){ exit(EXIT_FAILURE);}
  CABECALHO cab;
  carrega_raiz(fp_raiz,&cab);
  imp(cab.offset_raiz,fp_index,cab.t,0);
  fclose(fp_raiz);
}
void arvore_remover(char *index, char *raiz, char *nome) {
  FILE *fp_raiz = fopen(raiz,"rb");
  if(!fp_raiz){ exit(EXIT_FAILURE);}

  FILE* fp_index = fopen(index,"rb+");
  if(!fp_index){ exit(EXIT_FAILURE);}

  CABECALHO cab;
  carrega_raiz(fp_raiz,&cab);
  ARVORE *arv = cria_no(cab.t);
  carrega_no(arv,fp_index,cab.offset_raiz,cab.t);
  while (arv->eh_folha == 0) {
    int i =0;
    while (i < arv->nchaves && strcmp(nome, arv->chaves[i].id_no) > 0 )i++;
    long prox = arv->offset_filho[i];
    libera_no(arv);
    arv = cria_no(cab.t);
    carrega_no(arv, fp_index, prox, cab.t);
  }
  int i;
  for (i = 0; i < arv->nchaves && (strcmp(arv->chaves[i].id_no,nome) < 0); i++);
  if ((i < arv->nchaves) && (strcmp(nome,arv->chaves[i].id_no) == 0) && (arv->eh_folha)) { //C1
    printf("CASO 1");
    int j;
    for (j=i; j<arv->nchaves-1; j++) {
      memcpy(&arv->chaves[j],&arv->chaves[j+1],sizeof(CHAVE));
    }
    arv->nchaves --;
    salva_no(arv, fp_index, cab.offset_raiz, cab.t);

    libera_no(arv);
    fclose(fp_index);
    fclose(fp_raiz);
    }
    // aqui vou por o caso 3

    //   if(!arv) return arv;
    //   int i;
    //   for(i = 0; i < arv->nchaves && arv->chave[i] < ch; i++);
    //   if((i < arv->nchaves) && (ch == arv->chave[i]) && (arv->folha)){ //CASO 1
    //     printf("\nCASO 1\n");
    //     int j;
    //     for(j=i; j<arv->nchaves-1;j++) arv->chave[j] = arv->chave[j+1];
    //     arv->nchaves--;
  }




// ARVORE *TARVBM_insere(ARVORE *T, int mat, int t){
//   if(TARVBM_busca(T, mat)) return T;
//   if(!T){
//     T=TARVBM_cria(t);
//     T->chave[0] = mat;
//     T->nchaves=1;
//     return T;
//   }
//   if(T->nchaves == (2*t)-1){
//     ARVORE *S = TARVBM_cria(t);
//     S->nchaves=0;
//     S->folha = 0;
//     S->filho[0] = T;
//     S = divisao(S,1,T,t);
//     S = insere_nao_completo(S, mat, t);
//     return S;
//   }
//   T = insere_nao_completo(T, mat, t);
//   return T;
// }

// ARVORE *TARVBM_busca_aux( FILE* index,char *filme, long offset_no){
//   ARVORE no_atual = carrega();
//   if (!a) return NULL;
//   int i = 0;
//   while ((i < a->nchaves) && (strcmp(filme,a->chaves[i]->id_no) )) i++;
//   if ((i < a->nchaves) && (a->folha) && (mat == a->chave[i])) return a;
//   if (a-> folha) return NULL;
//   if (a->chave[i] == mat) i++;
//   return TARVBM_busca(a->filho[i], mat);
// }
//
//
// void TARVBM_imprime_chaves(ARVORE *a){
//   if(!a) return;
//   ARVORE *p = a;
//   while(p->filho[0]) p = p->filho[0];
//   while(p){
//     int i;
//     for(i = 0; i < p->nchaves; i++) printf("%d ", p->chave[i]);
//     p = p->prox;
//   }
//   printf("\n");
// }
//
//
// void imp(ARVORE *a, int andar){
//   if(a){
//     int i,j;
//     imp(a->filho[a->nchaves],andar+1);
//     for(i=a->nchaves-1; i >= 0; i--){
//       for(j=0; j<=andar; j++) printf("\t");
//       printf("%d\n", a->chave[i]);
//       imp(a->filho[i],andar+1);
//     }
//   }
// }
//
// void TARVBM_imprime(ARVORE *a){
//   imp(a, 0);
// }
//
//
// void imp0(ARVORE *a, int andar){
//   if(a){
//     int i,j;
//     for(i=0; i<=a->nchaves-1; i++){
//       imp(a->filho[i],andar+1);
//       for(j=0; j<=andar; j++) printf("\t");
//       printf("%d\n", a->chave[i]);
//     }
//     imp(a->filho[i],andar+1);
//   }
// }
//
// void TARVBM_imprime0(ARVORE *a){
//   imp0(a, 0);
// }
//
// ARVORE *divisao(ARVORE *x, int i, ARVORE* y, int t){
//   ARVORE *z = TARVBM_cria(t);
//   z->folha = y->folha;
//   int j;
//   if(!y->folha){
//     z->nchaves = t-1;
//     for(j=0;j<t-1;j++) z->chave[j] = y->chave[j+t];
//     for(j=0;j<t;j++){
//       z->filho[j] = y->filho[j+t];
//       y->filho[j+t] = NULL;
//     }
//   }
//   else {
//     z->nchaves = t; //z possuirá uma chave a mais que y se for folha
//     for(j=0;j < t;j++) z->chave[j] = y->chave[j+t-1];//Caso em que y é folha, temos q passar a info para o nó da direita
//     z->prox = y->prox; //ultima revisao: 04/2020
//     y->prox = z;
//   }
//   y->nchaves = t-1;
//   for(j=x->nchaves; j>=i; j--) x->filho[j+1]=x->filho[j];
//   x->filho[i] = z;
//   for(j=x->nchaves; j>=i; j--) x->chave[j] = x->chave[j-1];
//   x->chave[i-1] = y->chave[t-1];
//   x->nchaves++;
//   return x;
// }
//
//
// ARVORE *insere_nao_completo(ARVORE *x, int mat, int t){
//   int i = x->nchaves-1;
//   if(x->folha){
//     while((i>=0) && (mat < x->chave[i])){
//       x->chave[i+1] = x->chave[i];
//       i--;
//     }
//     x->chave[i+1] = mat;
//     x->nchaves++;
//     return x;
//   }
//   while((i>=0) && (mat < x->chave[i])) i--;
//   i++;
//   if(x->filho[i]->nchaves == ((2*t)-1)){
//     x = divisao(x, (i+1), x->filho[i], t);
//     if(mat > x->chave[i]) i++;
//   }
//   x->filho[i] = insere_nao_completo(x->filho[i], mat, t);
//   return x;
// }
//
//
//
// ARVORE* remover(ARVORE* arv, int ch, int t){
//   if(!arv) return arv;
//   int i;
//   for(i = 0; i < arv->nchaves && arv->chave[i] < ch; i++);
//   if((i < arv->nchaves) && (ch == arv->chave[i]) && (arv->folha)){ //CASO 1
//     printf("\nCASO 1\n");
//     int j;
//     for(j=i; j<arv->nchaves-1;j++) arv->chave[j] = arv->chave[j+1];
//     arv->nchaves--;
//     if(!arv->nchaves){ //ultima revisao: 04/2020
//       TARVBM_libera(arv);
//       arv = NULL;
//     }
//     return arv;
//   }
//
//   if((i < arv->nchaves) && (ch == arv->chave[i])) i++;
//   ARVORE *y = arv->filho[i], *z = NULL;
//   if (y->nchaves == t-1){ //CASOS 3A e 3B
//     if((i < arv->nchaves) && (arv->filho[i+1]->nchaves >=t)){ //CASO 3A
//       printf("\nCASO 3A: i menor que nchaves\n");
//       z = arv->filho[i+1];
//       if(!y->folha){
//         y->chave[t-1] = arv->chave[i];   //dar a y a chave i da arv
//         arv->chave[i] = z->chave[0];     //dar a arv uma chave de z
//       }
//       else{
//         arv->chave[i] = z->chave[0] + 1;
//         y->chave[t-1] = z->chave[0];
//       }
//       y->nchaves++;
//
//       int j;
//       for(j=0; j < z->nchaves-1; j++)  //ajustar chaves de z
//         z->chave[j] = z->chave[j+1];
//       y->filho[y->nchaves] = z->filho[0]; //enviar ponteiro menor de z para o novo elemento em y
//       for(j=0; j < z->nchaves; j++)       //ajustar filhos de z
//         z->filho[j] = z->filho[j+1];
//       z->nchaves--;
//       arv->filho[i] = remover(arv->filho[i], ch, t);
//       return arv;
//     }
//     if((i > 0) && (!z) && (arv->filho[i-1]->nchaves >= t)){ //CASO 3A
//       printf("\nCASO 3A: i igual a nchaves\n");
//       z = arv->filho[i-1];
//       int j;
//       for(j = y->nchaves; j>0; j--)               //encaixar lugar da nova chave
//         y->chave[j] = y->chave[j-1];
//       for(j = y->nchaves+1; j>0; j--) //encaixar lugar dos filhos da nova chave
//         y->filho[j] = y->filho[j-1];
//
//       if(!y->folha){
//         y->chave[0] = arv->chave[i-1];    //dar a y a chave i da arv
//         arv->chave[i-1] = z->chave[z->nchaves - 1];   //dar a arv uma chave de z
//       }
//       else{
//         arv->chave[i-1] = z->chave[z->nchaves - 1];
//         y->chave[0] = z->chave[z->nchaves-1];
//       }
//       y->nchaves++;
//
//       //enviar ponteiro de z para o novo elemento em y
//       y->filho[0] = z->filho[z->nchaves];
//       z->nchaves--;
//       arv->filho[i] = remover(y, ch, t);
//       return arv;
//     }
//     if(!z){ //CASO 3B
//       if(i < arv->nchaves && arv->filho[i+1]->nchaves == t-1){
//         printf("\nCASO 3B: i menor que nchaves\n");
//         z = arv->filho[i+1];
//         if(!y->folha){
//           y->chave[t-1] = arv->chave[i];//pegar chave [i] e coloca ao final de filho[i]
//           y->nchaves++;
//         }
//         int j = 0;
//         while(j < t-1){
//           if(!y->folha) y->chave[t+j] = z->chave[j];
//           else y->chave[t+j-1] = z->chave[j];
//           y->nchaves++;
//           j++;
//         }
//         y->prox = z->prox;
//         if(!y->folha){
//           for(j=0; j<t; j++){
//             y->filho[t+j] = z->filho[j];
//             z->filho[j] = NULL; //ultima revisao: 04/2020
//           }
//           //TARVBM_libera(z); 07/2024
//         }
//         TARVBM_libera(z); // 07/2024
//         for(j=i; j < arv->nchaves-1; j++){ //limpar referências de i
//           arv->chave[j] = arv->chave[j+1];
//           arv->filho[j+1] = arv->filho[j+2];
//         }
//         arv->filho[arv->nchaves] = NULL;
//         arv->nchaves--;
//         if(!arv->nchaves){ //ultima revisao: 04/2020
//           ARVORE *temp = arv;
//           arv = arv->filho[0];
//           temp->filho[0] = NULL;
//           TARVBM_libera(temp);
//         }
//         arv = remover(arv, ch, t);
//         return arv;
//       }
//       if((i > 0) && (arv->filho[i-1]->nchaves == t-1)){
//         printf("\nCASO 3B: i igual a nchaves\n");
//         z = arv->filho[i-1];
//         if(!y->folha){
//           if(i == arv->nchaves){
//             z->chave[t-1] = arv->chave[i-1]; //pegar chave[i] e poe ao final de filho[i-1]
//           }else{
//             z->chave[t-1] = arv->chave[i];   //pegar chave [i] e poe ao final de filho[i-1]
//           }
//           z->nchaves++;
//         }
//         int j = 0;
//         while(j < t-1){
//           if(!y->folha) z->chave[t+j] = y->chave[j];
//           else z->chave[t+j-1] = y->chave[j];
//           z->nchaves++;
//           j++;
//         }
//         z->prox = y->prox;
//         if(!z->folha){
//           for(j=0; j<t; j++){
//             z->filho[t+j] = y->filho[j];
//             y->filho[j] = NULL; //ultima revisao: 04/2020
//           }
//           //TARVBM_libera(y); 07/2024
//         }
//         TARVBM_libera(y); // 07/2024
//         arv->filho[arv->nchaves] = NULL;
//         arv->nchaves--;
//         if(!arv->nchaves){ //ultima revisao: 04/2020
//           ARVORE *temp = arv;
//           arv = arv->filho[0];
//           temp->filho[0] = NULL;
//           TARVBM_libera(temp);
//           arv = remover(arv, ch, t);
//         }
//         else{
//           i--;
//           arv->filho[i] = remover(arv->filho[i], ch, t);
//         }
//         return arv;
//       }
//     }
//   }
//   arv->filho[i] = remover(arv->filho[i], ch, t);
//   return arv;
// }
//
//carrega_raiz
// ARVORE* TARVBM_retira(ARVORE* arv, int k, int t){
//   if(!arv || !TARVBM_busca(arv, k)) return arv;
//   return remover(arv, k, t);
// }
