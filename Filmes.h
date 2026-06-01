//
// Created by alcir on 29/05/2026.
//
#include "Arvore.h"

#ifndef TRABALHO_RO7_FILMES_H
#define TRABALHO_RO7_FILMES_H
#define QTD_PARTICIPANTES 100

typedef struct Filmes {
    char titulo[TAM_NOME_FILME];
    int ano;
    char subtitulo[TAM_SUBTITULO_FILME];
    int n_participantes;
    int participantes[QTD_PARTICIPANTES];
}FILME_REG;

void filmes_atualiza(FILE *f, long offset, FILME_REG *reg);
#endif //TRABALHO_RO7_FILMES_H