/*
    Codigo feito em 2026.1 pelos estudantes
    Ana Rebeca Mendes de Souza (602242)
    Arthur Araujo Custodio (604827)
    Rafael Calisto Oliveira da Silva (606478)
*/
#include <stdio.h>
#include <stdlib.h>
#include "tabuleiro.h"

/* cria o tabuleiro com a configuracao inicial */
PontTab criar_tabuleiro() {
    PontTab tab = malloc(10 * sizeof(*tab));
    int i, j;

    for(i = 0;i < 10;i++) {
        for(j = 0;j < 10;j++)
            if((i + j) % 2 == 0)
                tab[i][j] = '#';
            else if(i <= 2)
                tab[i][j] = 'o';
            else if(i >= 7)
                tab[i][j] = '@';
            else
                tab[i][j] = ' ';
    }

    return tab;
}

/* imprime o tabuleiro como caracteres */
void imprimir_tabuleiro (PontTab tab) {
    int i, j;

    printf("  A B C D E F G H I J\n");
    printf(" +-+-+-+-+-+-+-+-+-+-+\n");

    for(i = 0;i < 10;i++) {
        printf("%d|", i);
        for(j = 0;j < 10;j++)
            printf("%c|", tab[i][j]);
        printf("%d", i);
        printf("\n +-+-+-+-+-+-+-+-+-+-+\n");
    }

    printf("  A B C D E F G H I J\n");
}

/* checa se a coordenada esta dentro do tabuleiro */
short int dentro_do_tabuleiro(int linha, int coluna) {
    if(linha < 0 || linha > 9 || coluna < 0 || coluna > 9)
        return 0;
    return 1;
}

/*

Tabuleiro inicial:

A B C D E F G H I J
+-+-+-+-+-+-+-+-+-+-+
0|#|o|#|o|#|o|#|o|#|o|0
+-+-+-+-+-+-+-+-+-+-+
1|o|#|o|#|o|#|o|#|o|#|1
+-+-+-+-+-+-+-+-+-+-+
2|#|o|#|o|#|o|#|o|#|o|2
+-+-+-+-+-+-+-+-+-+-+
3| |#| |#| |#| |#| |#|3
+-+-+-+-+-+-+-+-+-+-+
4|#| |#| |#| |#| |#| |4
+-+-+-+-+-+-+-+-+-+-+
5| |#| |#| |#| |#| |#|5
+-+-+-+-+-+-+-+-+-+-+
6|#| |#| |#| |#| |#| |6
+-+-+-+-+-+-+-+-+-+-+
7|@|#|@|#|@|#|@|#|@|#|7
+-+-+-+-+-+-+-+-+-+-+
8|#|@|#|@|#|@|#|@|#|@|8
+-+-+-+-+-+-+-+-+-+-+
9|@|#|@|#|@|#|@|#|@|#|9
+-+-+-+-+-+-+-+-+-+-+
A B C D E F G H I J

*/