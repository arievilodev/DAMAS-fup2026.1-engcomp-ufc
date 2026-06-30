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

short int eh_letra_tabuleiro(char c) {
    /* retorna se eh uma letra dentro do tabuleiro */
    return c >= 'A' && c <= 'J';
}

short int eh_numero_tabuleiro(char n) {
    /* retorna se eh um numero dentro do tabuleiro */
    return n >= '0' && n <= '9';
}

short int not_in_str(char *v, char c) {
    /* retorna se o carctere nao esta no vetor */
    int i=0;
    while(v[i] != '/0')
        if (v[i++] == c) return 0;
    return 1;
}

int letra_para_numero(char c) {
    /* retorna a abscissa que a letra representa, por exemplo, A=0, B=1, ..., J=9 */
    return c-'A';
}

int char_para_numero(char n) {
    /* retorna a ordenada que o numero representa, no caso, o proprio numero */
    return n-'0';
}

short int verifica_vitoria_por_qtd(int qtd_oponente) {
    return qtd_oponente <= 0;
}

short int da_para_mover(PontTab tabuleiro, Jogador jogador, Casa casa) {
    /*
        Verifica se essa peca pode se mover pelo menos uma casa (no caso das damas podem se mover mais que uma casa, mas basta uma para nao estar afogada).
        Verifica qual jogador eh, para saber se move para cima ou para baixo
        e verifica se eh um peao ou uma dama
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
    /* Retorna 1 se nao alterar jogador, se sim retorna 0 */
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
    short int jogada_valida;
    char vitorioso = '\0',
        c, /* auxiliar para deletar buffer exedente */
        comeca,
        sair,
        entrada_jogada[8]; /* sao 8 contando \n e \0 */
    PontTab tabuleiro;
    Jogador jogador;
    Casa casa_inicial, casa_final;

    tabuleiro = criar_tabuleiro();

    imprimir_tabuleiro(tabuleiro);

    printf("Digite quem começa o jogo (\"C\", para cima ou \"B\", para baixo): ");
    scanf("%c", &comeca);
    while(pecas_Jogador_id(&jogador, comeca)) {
        printf("\nEntrada Invalida\nDigite quem começa o jogo (\"C\", para cima ou \"B\", para baixo): ");
        scanf("%c", &comeca);
    }
    
    /* Loop das jogadas */
    while (vitorioso == '\n') {
        /* Faz a leitura da jogada */
        jogada_valida = 0;
        while (!jogada_valida) {
            imprimir_tabuleiro(tabuleiro);
            printf("Entre com a jogada do jogador %c: ", jogador.id);
            if (fgets(entrada_jogada, sizeof(entrada_jogada), stdin) != NULL) {
                if (not_in_str(entrada_jogada, '\n')) {
                    /* Descarta o buffer */
                    while ((c = getchar()) != '\n' && c != EOF);

                    printf("Entrada invalida!\n"); /* Se tiver mais caracteres na entrada que 6 */
                } else if (
                    eh_letra_tabuleiro(tabuleiro[0]) &&
                    eh_numero_tabuleiro(tabuleiro[1]) &&
                    tabuleiro[2] == '-' && tabuleiro[3] == '-' &&
                    eh_letra_tabuleiro(tabuleiro[4]) &&
                    eh_numero_tabuleiro(tabuleiro[5])
                ) {
                    casa_inicial.lin = letra_para_numero(tabuleiro[0]);
                    casa_inicial.col = char_para_numero(tabuleiro[1]);
                    casa_final.lin = letra_para_numero(tabuleiro[4]);
                    casa_final.col = char_para_numero(tabuleiro[5]);
                    if (jogada(tabuleiro, jogador, casa_inicial, casa_final) != -1) {
                        jogada_valida = 1;
                    } else {
                        printf("Entrada invalida!\n"); /* Se nao estiver for uma jogada permitida */
                    }
                } else {
                    printf("Entrada invalida!\n"); /* Se nao estiver no padrao A0--B0 */
                }
            } else {
                printf("Entrada invalida!\n"); /* Se a entrada for EOF */
            }
        }
    }

    printf("Deseja jogar novamente? (\"s\", para sim ou \"n\", para nao): ");
    scanf("%c", &sair);
    while(sair != 's' && sair != 'n') {
        printf("\nEntrada Invalida\nDeseja jogar novamente? (\"s\", para sim ou \"n\", para nao): ");
        scanf("%c", &sair);
    }
    return sair=='s';
}
