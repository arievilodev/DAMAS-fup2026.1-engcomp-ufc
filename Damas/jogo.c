/*
    jogo.c

    Created by Arthur de Araujo Custodio on 23/06/2026.
    Copyright © 2026 Arthur de Araujo Custodio. All rights reserved.
*/
#include "jogo.h"
#include "tabuleiro.h"
#include "jogada.h"

int letra_para_numero(char c) {
    /* retorna a ordenada que a letra representa, por exemplo, A=0, B=1, ..., J=9 */
    return c-'A';
}

short int verifica_vitoria_por_qtd(int qtd_oponente) {
    return qtd_oponente <= 0;
}

short int da_para_mover(PontTab tabuleiro, Jogador jogador, Casa casa) {
    /*
        Verifica se essa peca pode se mover pelo menos uma casa (no caso das damas podem se mover mais que uma casa, mas basta uma para nao estar afogada).
        Verifica qual jogador eh, para saber se move para cima ou para baixo
        e verifica se é um peao ou uma dama
    */
    if ((jogador.id == 'C' && jogador.peao == tabuleiro[casa.lin][casa.col]) || jogador.dama == tabuleiro[casa.lin][casa.col]) {
        /* diagonal para baixo esquerda */
        if (dentro_do_tabuleiro(casa.col-1, casa.lin+1))
            if (tabuleiro[casa.lin+1][casa.col-1] == ' ')
                return 1;
        /* diagonal para baixo direita */
        if (dentro_do_tabuleiro(casa.col+1, casa.lin+1))
            if (tabuleiro[casa.lin+1][casa.col+1] == ' ')
                return 1;
    }
    if ((jogador.id == 'B' && jogador.peao == tabuleiro[casa.lin][casa.col]) || jogador.dama == tabuleiro[casa.lin][casa.col]) {
        /* diagonal para cima esquerda */
        if (dentro_do_tabuleiro(casa.col-1, casa.lin-1))
            if (tabuleiro[casa.lin-1][casa.col-1] == ' ')
                return 1;
        /* diagonal para cima direita */
        if (dentro_do_tabuleiro(casa.col+1, casa.lin-1))
            if (tabuleiro[casa.lin-1][casa.col+1] == ' ')
                return 1;
    }
    return 0;
}

short int verifica_derrota_por_afogamento(Jogador jogador, PontTab tabuleiro, int qtd_pecas) {
    Casa casa;
    /* Pecorre o tabuleiro ate o final ou ate as pecas acabarem */
    for (casa.lin=0; casa.lin<10 && qtd_pecas; casa.lin++)
        for (casa.col=0; casa.col<10 && qtd_pecas; casa.col++)
            if (tabuleiro[casa.lin][casa.col] == jogador.peao || tabuleiro[casa.lin][casa.col] == jogador.dama) {
                qtd_pecas--;
                if (da_para_comer(tabuleiro, jogador, casa)) return 0;
                if (da_para_mover(tabuleiro, jogador, casa)) return 0; 
            }
    return 1;
}

void jogo_JxJ() {
    char jogador_atual_aux, vitorioso = '\0';
    char tabuleiro[10][10];

    return 0;
}
