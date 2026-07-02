/*
    Codigo feito em 2026.1 pelos estudantes
    Ana Rebeca Mendes de Souza (602242)
    Arthur de Araujo Custodio (604827)
    Rafael Calisto Oliveira da Silva (606478)
*/
#include <stdio.h>
#include <stdlib.h>
#include "jogo.h"

int main (int narg, char *argv[]) {
    FILE *fin;
    char *entrada = argv[1];
      
    if(narg > 1){ /* opcao com arquivo de entrada */

        if(narg > 2)
            printf("Apenas o primeiro parametro sera lido.\n");

        /* abertura do arquivo */
        fin = fopen(entrada, "rt");
        if(fin == NULL) {
            printf("Erro na abertura do arquivo.\n");
            return EXIT_FAILURE;
        }

        /* chama a função de jogo.c */
        jogo_offline(fin);

        fclose(fin);

    }else{ /* opcao com entrada padrao */

        /* chama a função de jogo.c */
        while(jogo_JxJ());
    }

    return EXIT_SUCCESS;
}
