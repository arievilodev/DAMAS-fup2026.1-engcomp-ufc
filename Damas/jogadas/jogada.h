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

/* Verifica se a peça pertence ao jogador informado. */
short int peca_do_jogador(char peca, Jogador jogador);

/* Verifica se a peça pertence ao adversário do jogador informado. */
short int peca_adversaria(char peca, Jogador jogador);

/*
    Verifica se um movimento simples (sem captura)
    é válido de acordo com as regras do jogo.
*/
short int mover_eh_valido(
    PontTab tab,
    Jogador jogador,
    Casa inicial,
    Casa final
);

/*
    Verifica se uma captura é válida.
    Considera tanto peões quanto damas.
*/
short int comer_eh_valido(
    PontTab tab,
    Jogador jogador,
    Casa inicial,
    Casa final
);

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
    Verifica se a peça possui algum movimento legal.
*/
short int da_para_mover(
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
int jogada(
    PontTab tab,
    Jogador jogador,
    Casa inicial,
    Casa final
);

#endif