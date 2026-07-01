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


/* Conversores */

int letra_para_numero(char c) {
    /* retorna a abscissa que a letra representa, por exemplo, A=0, B=1, ..., J=9 */
    return c-'A';
}

int char_para_numero(char n) {
    /* retorna a ordenada que o numero representa, no caso, o proprio numero */
    return n-'0';
}


/* Verificadores (exeto os de vitoria) */

short int eh_letra_tabuleiro(char c) {
    /* retorna se eh uma letra dentro do tabuleiro */
    return c >= 'A' && c <= 'J';
}

short int eh_numero_tabuleiro(char n) {
    /* retorna se eh um numero dentro do tabuleiro */
    return n >= '0' && n <= '9';
}

/* retorna se o carctere nao esta no vetor */
short int not_in_str(char *v, char c) {
    /* retorna se o carctere nao esta no vetor */
    int i=0;
    while(v[i] != '\0')
        if (v[i++] == c) return 0;
    return 1;
}

short int da_para_mover_1_casa(PontTab tabuleiro, Jogador jogador, Casa casa) {
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


/* Modificadores */

short int pecas_jogador_id(Jogador *jogador, char id) {
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

short int trocar_jogador(Jogador *jogador) {
    /* Retorna 1 se nao alterar jogador, se sim retorna 0 */
    switch (jogador->id) {
        case 'B':
            return pecas_jogador_id(jogador, 'C');
        case 'C':
            return pecas_jogador_id(jogador, 'B');
        }
    return 1;
}

short int peca_para_dama(PontTab tabuleiro, Jogador jogador) {
    int linha, j; /* linha que vai ser verificada se da para transformar em dama */
    if (jogador.id == 'C')
        linha = 9;
    else if (jogador.id == 'B')
        linha = 0;
    else
        return -1;
    
    for (j=0; j<10; j++)
        if (tabuleiro[linha][j] == jogador.peao) {
            tabuleiro[linha][j] = jogador.dama;
            return 1;
        }
    return 0;
}


/* Verificadores de vitoria */

short int verifica_derrota_por_afogamento(PontTab tabuleiro, Jogador jogador) {
    Casa casa;
    /* Pecorre o tabuleiro ate o final ou ate as pecas acabarem */
    for (casa.lin=0; casa.lin<10; casa.lin++)
        for (casa.col=0; casa.col<10; casa.col++)
            if (tabuleiro[casa.lin][casa.col] == jogador.peao || tabuleiro[casa.lin][casa.col] == jogador.dama) {
                if (da_para_mover_1_casa(tabuleiro, jogador, casa)) return 0; 
                if (da_para_comer(tabuleiro, jogador, casa)) return 0;
            }
    return 1;
}

short int verifica_vitoria_por_qtd(PontTab tabuleiro, Jogador jogador) {
    int i, j;
    Jogador oponente;
    oponente = jogador;
    trocar_jogador(&oponente);
    for (i=0; i<10; i++)
        for (j=0; j<10; j++)
            if (tabuleiro[i][j] == oponente.peao || tabuleiro[i][j] == oponente.dama) return 0;
    return 1;
}


/* Impressao em caso de jogada invalida */
void jogada_invalida(PontTab tabuleiro) {
    imprimir_tabuleiro(tabuleiro);
    printf("Jogada invalida.\n");
}


/* Jogo jogador contra jogador */
short int jogo_JxJ() {
    /* Retorna se quer jogar novamente */
    short int jogada_valida, jog; /* auxiliar para receber retorno de jogada() */
    char vitorioso = '\0',
        c, /* auxiliar para deletar buffer exedente */
        entrada_comeca[3], /* sao 3 contando \n e \0 */
        comeca,
        entrada_sair[3], /* sao 3 contando \n e \0 */
        sair,
        entrada_jogada[8]; /* sao 8 contando \n e \0 */
    PontTab tabuleiro = criar_tabuleiro();
    Jogador jogador;
    Casa casa_inicial, casa_final;

    imprimir_tabuleiro(tabuleiro);

    /* Recebe a entrada de quem comeca */
    printf("Digite quem comeca o jogo (\"C\", para cima ou \"B\", para baixo): ");
    if (fgets(entrada_comeca, sizeof(entrada_comeca), stdin) != NULL) {
        if (not_in_str(entrada_comeca, '\n'))
            while ((c = getchar()) != '\n' && c != EOF); /* Descarta o buffer */
        else
            comeca = entrada_comeca[0];
    }
    while(pecas_jogador_id(&jogador, comeca)) {
        printf("\nEntrada invalida.\nDigite quem comeca o jogo (\"C\", para cima ou \"B\", para baixo): ");
        if (fgets(entrada_comeca, sizeof(entrada_comeca), stdin) != NULL) {
            if (not_in_str(entrada_comeca, '\n'))
                while ((c = getchar()) != '\n' && c != EOF); /* Descarta o buffer */
            else
                comeca = entrada_comeca[0];
        }
    }

    /* Loop das jogadas */
    while (vitorioso == '\0') {
        /* Faz a leitura da jogada */
        if (!verifica_derrota_por_afogamento(tabuleiro, jogador)) {
            jogada_valida = 0;
            imprimir_tabuleiro(tabuleiro);
            while (!jogada_valida) {
                printf("Entre com a jogada do jogador %c: ", jogador.id);
                if (fgets(entrada_jogada, sizeof(entrada_jogada), stdin) != NULL) {
                    if (not_in_str(entrada_jogada, '\n')) {
                        /* Descarta o buffer */
                        while ((c = getchar()) != '\n' && c != EOF);
                        jogada_invalida(tabuleiro); /* Se tiver mais caracteres na entrada que 6 */
                    } else if (
                        eh_letra_tabuleiro(entrada_jogada[0]) &&
                        eh_numero_tabuleiro(entrada_jogada[1]) &&
                        entrada_jogada[2] == '-' && entrada_jogada[3] == '-' &&
                        eh_letra_tabuleiro(entrada_jogada[4]) &&
                        eh_numero_tabuleiro(entrada_jogada[5])
                    ) {
                        casa_inicial.col = letra_para_numero(entrada_jogada[0]);
                        casa_inicial.lin = char_para_numero(entrada_jogada[1]);
                        casa_final.col = letra_para_numero(entrada_jogada[4]);
                        casa_final.lin = char_para_numero(entrada_jogada[5]);

                        jog = jogada(tabuleiro, jogador, casa_inicial, casa_final);
                        if (jog != -1) {
                            peca_para_dama(tabuleiro, jogador); /* promove peao na ultima ou primeira linha para dama */
                            if (jog) {
                                if(verifica_vitoria_por_qtd(tabuleiro, jogador))
                                    vitorioso=jogador.id;
                            } else {
                                trocar_jogador(&jogador);
                            }
                            jogada_valida = 1;
                        } else {
                            jogada_invalida(tabuleiro); /* Se nao for uma jogada permitida */
                        }
                    } else {
                        jogada_invalida(tabuleiro); /* Se nao estiver no padrao A0--B0 */
                    }
                } else {
                    jogada_invalida(tabuleiro); /* Se a entrada for EOF */
                }
            }
        } else {
            trocar_jogador(&jogador);
            vitorioso = jogador.id;
        }
    }
    imprimir_tabuleiro(tabuleiro);
    printf("O vencedor eh o usuario de ");
    printf((vitorioso == 'C') ? "CIMA.\n" : "BAIXO.\n");

    /* Recebe a entrada de se quer continuar ou nao */
    printf("\nDesejam jogar novamente? (\"s\", para sim ou \"n\", para nao): ");
    if (fgets(entrada_sair, sizeof(entrada_sair), stdin) != NULL) {
        if (not_in_str(entrada_sair, '\n'))
            while ((c = getchar()) != '\n' && c != EOF); /* Descarta o buffer */
        else
            sair = entrada_sair[0];
    }
    while (sair != 's' && sair != 'n') {
        printf("\nEntrada invalida\nDesejam jogar novamente? (\"s\", para sim ou \"n\", para nao): ");
        if (fgets(entrada_sair, sizeof(entrada_sair), stdin) != NULL) {
            if (not_in_str(entrada_sair, '\n'))
                while ((c = getchar()) != '\n' && c != EOF); /* Descarta o buffer */
            else
                sair = entrada_sair[0];
        }
    }
    return sair=='s';
}


/* Modificador */

void zerar_entrada_jogada(char* entrada) {
    int t = 6; /* tamanho de entrada_jogada */
    while(t--) {
        entrada[t] = '\0';
    }
} 


/* Impressao em caso de jogada invalida */
void jogada_invalida_linha(int linha) {
    printf("Jogada invalida na linha %d do arquivo de entrada.\n", linha);
}


/* Jogo offline */
void jogo_offline(FILE* arquivo) {
    char vitorioso = '\0',
        c, /* auxiliar para coleta dos caracteres do arquivo */
        comeca,
        entrada_jogada[6]; /* sao 6 porque não preciso do /0, pois nao utilizarei como string */
    PontTab tabuleiro = criar_tabuleiro();
    Jogador jogador;
    Casa casa_inicial, casa_final;
    int qtd_caracteres=0, qtd_pecas_C=0, qtd_pecas_B=0, linha=2; /* acumulador para saber em qual linha esta */
    short int jog; /* auxiliar para receber retorno de jogada() */

    if (((comeca = fgetc(arquivo)) != EOF) && (((c = fgetc(arquivo)) == '\n') || (c == EOF))) {
        if (comeca == 'C' || comeca == 'B') {
            pecas_jogador_id(&jogador, comeca);
            zerar_entrada_jogada(entrada_jogada);
            /* pecorre o arquivo caractere a caracter */
            while ((c = fgetc(arquivo)) != EOF) {
                if (vitorioso == '\0') {
                    if (!verifica_derrota_por_afogamento(tabuleiro, jogador)) {
                        /* lê a linha e armazena até encher entrada_jogada */
                        if (c != '\n') {
                            if (qtd_caracteres<6)
                                entrada_jogada[qtd_caracteres++] = c;
                            else qtd_caracteres++;
                        } else { /* faz a jogada */
                            if (qtd_caracteres == 6) {
                                if (
                                    eh_letra_tabuleiro(entrada_jogada[0]) &&
                                    eh_numero_tabuleiro(entrada_jogada[1]) &&
                                    entrada_jogada[2] == '-' && entrada_jogada[3] == '-' &&
                                    eh_letra_tabuleiro(entrada_jogada[4]) &&
                                    eh_numero_tabuleiro(entrada_jogada[5])
                                ) {
                                    casa_inicial.col = letra_para_numero(entrada_jogada[0]);
                                    casa_inicial.lin = char_para_numero(entrada_jogada[1]);
                                    casa_final.col = letra_para_numero(entrada_jogada[4]);
                                    casa_final.lin = char_para_numero(entrada_jogada[5]);

                                    jog = jogada(tabuleiro, jogador, casa_inicial, casa_final);
                                    if (jog != -1) {
                                        peca_para_dama(tabuleiro, jogador); /* promove peao na ultima ou primeira linhapara dama */
                                        if (jog) {
                                            if (jogador.id == 'C') qtd_pecas_C++;
                                            else qtd_pecas_B++;
                                            
                                            if(verifica_vitoria_por_qtd(tabuleiro, jogador))
                                                vitorioso=jogador.id;
                                        } else
                                            trocar_jogador(&jogador);
                                    } else
                                        jogada_invalida_linha(linha); /* Se nao for uma jogada permitida */
                                } else
                                    jogada_invalida_linha(linha); /* Se nao estiver no padrao A0--B0 */
                            } else
                                jogada_invalida_linha(linha); /* Se a linha nao tiver 6 caracteres */
                            
                            zerar_entrada_jogada(entrada_jogada);
                            qtd_caracteres=0;
                            linha++;
                        }
                    } else {
                        trocar_jogador(&jogador);
                        vitorioso = jogador.id;
                        if (c == '\n') jogada_invalida_linha(linha++); /* Se o jogador ja tiver ganhado nao aceita mais jogadas */
                    }
                } else
                    if (c == '\n') jogada_invalida_linha(linha++); /* Se o jogador ja tiver ganhado nao aceita mais jogadas */
            }

            /* existe o risco de a jogada terminar com EOF e nao ser verificado o afogamento */
            if (vitorioso == '\0') {
                if (verifica_derrota_por_afogamento(tabuleiro, jogador)) {
                        trocar_jogador(&jogador);
                        vitorioso = jogador.id;
                }
            }

            imprimir_tabuleiro(tabuleiro);
            printf("Cima = %d / Baixo = %d\n", qtd_pecas_C, qtd_pecas_B);
            if (vitorioso != '\0') {
                printf("O vencedor eh o usuario de ");
                printf((vitorioso == 'C') ? "CIMA.\n" : "BAIXO.\n");
            }
        } else
            printf("Jogador inicial invalido.\n");
    } else
        printf("Jogador inicial invalido.\n");
}
