/*
    Codigo feito em 2026.1 pelos estudantes
    Ana Rebeca Mendes de Souza (602242)
    Arthur Araujo Custodio (604827)
    Rafael Calisto Oliveira da Silva (606478)
*/
#ifndef TABULEIRO_H
#define TABULEIRO_H

typedef char (*PontTab)[10]; /* define o vetor de ponteiros como um tipo de dado */

PontTab criar_tabuleiro(); /* cria o tabuleiro com a configuracao inicial */

void imprimir_tabuleiro (PontTab); /* imprime o tabuleiro como caracteres */

short int dentro_do_tabuleiro(int, int); /* checa se a coordenada esta dentro do tabuleiro */

#endif