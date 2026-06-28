/*
    jogo.c

    Created by Arthur de Araujo Custodio on 23/06/2026.
    Copyright © 2026 
        Ana Rebeca Mendes de Souza (602242);
        Arthur Araujo Custodio (604827);
        Rafael Calisto Oliveira da Silva (606478).
    All rights reserved.
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
                if (da_para_mover(tabuleiro, jogador, casa)) return 0; 
                if (da_para_comer(tabuleiro, jogador, casa)) return 0;
            }
    return 1;
}

short int pecas_Jogador_id(Jogador *jogador, char id) {
    /* Retorna 1 se nao alterar jogador, se nao retorna 0 */
    switch (id) {
        case 'C':
            jogador->id = 'C';
            jogador->peao = 'o';
            jogador->dama = 'O';
            return 0;
        case 'B':
            jogador->id = 'B';
            jogador->peao = '@';
            jogador->dama = '&';
            return 0;
        }
    return 1;
}

short int jogo_JxJ() {
    char vitorioso = '\0', comeca, sair;
    PontTab tabuleiro;
    Jogador jogador;

    tabuleiro = criar_tabuleiro();

    printf("Digite quem começa o jogo (\"C\", para cima ou \"B\", para baixo): ");
    scanf("%c", &comeca);
    while(pecas_Jogador_id(&jogador, comeca)) {
        printf("\nEntrada Invalida\nDigite quem começa o jogo (\"C\", para cima ou \"B\", para baixo): ");
        scanf("%c", &comeca);
    }

    printf("Deseja jogar novamente? (\"s\", para sim ou \"n\", para nao): ");
    scanf("%c", &sair);
    while(sair != 's' && sair != 'n') {
        printf("\nEntrada Invalida\nDeseja jogar novamente? (\"s\", para sim ou \"n\", para nao): ");
        scanf("%c", &sair);
    }
    return sair=='s';
}
