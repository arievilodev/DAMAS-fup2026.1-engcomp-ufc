/*
    Codigo feito em 2026.1 pelos estudantes
    Ana Rebeca Mendes de Souza (602242)
    Arthur Araujo Custodio (604827)
    Rafael Calisto Oliveira da Silva (606478)
*/

#ifndef JOGADA_H
#define JOGADA_H

#include "tabuleiro.h"

/*
    Estrutura que representa um jogador.

    id   -> identifica o jogador ('C' para cima ou 'B' para baixo)
    peao -> caractere utilizado para representar um peão
    dama -> caractere utilizado para representar uma dama
*/
typedef struct {
    char id;
    char peao;
    char dama;
} Jogador;

/*
    Representa uma posição do tabuleiro através da linha e da coluna.
*/
typedef struct {
    int lin;
    int col;
} Casa;

/*
    Verifica se existe pelo menos uma captura
    possível para a peça indicada.
*/
short int da_para_comer(
    PontTab tab,
    Jogador jogador,
    Casa casa
);

/*
    Executa uma jogada.

    Retornos:
    -1 -> jogada inválida
     0 -> movimento simples realizado
     1 -> captura realizada
*/
short int jogada(
    PontTab tab,
    Jogador jogador,
    Casa inicial,
    Casa final
);

#endif
