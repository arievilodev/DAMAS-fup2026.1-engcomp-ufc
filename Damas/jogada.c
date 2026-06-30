/*
    Codigo feito em 2026.1 pelos estudantes
    Ana Rebeca Mendes de Souza (602242)
    Arthur Araujo Custodio (604827)
    Rafael Calisto Oliveira da Silva (606478)
*/

#include <stdlib.h>     /* Utilizado para a função abs() */
#include "jogada.h"

/*
    Verifica se o caractere informado representa
    uma peça pertencente ao jogador.
*/
short int peca_do_jogador(char peca, Jogador jogador)
{
    return (
        peca == jogador.peao ||
        peca == jogador.dama
    );
}

/*
    Verifica se o caractere representa uma peça
    do adversário.

    Espaços vazios e casas não jogáveis ('#')
    não são considerados peças.
*/
short int peca_adversaria(char peca, Jogador jogador)
{
    if (peca == ' ' || peca == '#')
        return 0;

    return !peca_do_jogador(peca, jogador);
}

/*
    Verifica se um movimento sem captura é válido.
*/
short int mover_eh_valido(
    PontTab tab,
    Jogador jogador,
    Casa inicial,
    Casa final
)
{
    /* Diferença entre origem e destino */
    int dl = final.lin - inicial.lin;
    int dc = final.col - inicial.col;

    /* A casa de destino precisa estar vazia */
    if(tab[final.lin][final.col] != ' ')
        return 0;

    /* Movimento de dama */
    if(tab[inicial.lin][inicial.col] == jogador.dama)
    {
        /* O deslocamento deve ocorrer em uma diagonal */
        if(abs(dl) != abs(dc))
            return 0;

        return 1;
    }

    /* Movimento do jogador de cima */
    if(jogador.id == 'C')
    {
        if(dl == 1 && abs(dc) == 1)
            return 1;
    }

    /* Movimento do jogador de baixo */
    if(jogador.id == 'B')
    {
        if(dl == -1 && abs(dc) == 1)
            return 1;
    }

    return 0;
}

/*
    Verifica se uma captura é válida.
*/
short int comer_eh_valido(
    PontTab tab,
    Jogador jogador,
    Casa inicial,
    Casa final
)
{
    int ml;
    int mc;
    int dl;
    int dc;

    /* O destino deve estar vazio */
    if(tab[final.lin][final.col] != ' ')
        return 0;

    dl = final.lin - inicial.lin;
    dc = final.col - inicial.col;

    /* Captura realizada por um peão */
    if(tab[inicial.lin][inicial.col] == jogador.peao)
    {
        /* O peão deve saltar exatamente duas casas */
        if(abs(dl) != 2 || abs(dc) != 2)
            return 0;

        /* Calcula a posição da peça capturada */
        ml = (inicial.lin + final.lin) / 2;
        mc = (inicial.col + final.col) / 2;

        /* A casa intermediária deve conter um adversário */
        if(peca_adversaria(tab[ml][mc], jogador))
            return 1;

        return 0;
    }

    /* Captura realizada por uma dama */

    if(abs(dl) != abs(dc))
        return 0;

    {
        /* Sentido do deslocamento */
        int sl = (dl > 0) ? 1 : -1;
        int sc = (dc > 0) ? 1 : -1;

        int l = inicial.lin + sl;
        int c = inicial.col + sc;

        int encontrou = 0;

        /*
            Percorre toda a diagonal procurando
            exatamente uma peça adversária.
        */
        while(l != final.lin)
        {
            if(tab[l][c] != ' ')
            {
                if(peca_adversaria(tab[l][c], jogador))
                {
                    /* Não pode haver duas peças adversárias */
                    if(encontrou)
                        return 0;

                    encontrou = 1;
                }
                else
                    /* Caminho bloqueado por peça própria */
                    return 0;
            }

            l += sl;
            c += sc;
        }

        return encontrou;
    }
}

/*
    Verifica se a peça possui alguma captura disponível.
*/
short int da_para_comer(
    PontTab tab,
    Jogador jogador,
    Casa casa
)
{
    Casa destino;

    /* Diagonal inferior direita */
    destino.lin = casa.lin + 2;
    destino.col = casa.col + 2;

    if(dentro_do_tabuleiro(destino.lin, destino.col))
        if(comer_eh_valido(tab,jogador,casa,destino))
            return 1;

    /* Diagonal inferior esquerda */
    destino.lin = casa.lin + 2;
    destino.col = casa.col - 2;

    if(dentro_do_tabuleiro(destino.lin, destino.col))
        if(comer_eh_valido(tab,jogador,casa,destino))
            return 1;

    /* Diagonal superior direita */
    destino.lin = casa.lin - 2;
    destino.col = casa.col + 2;

    if(dentro_do_tabuleiro(destino.lin, destino.col))
        if(comer_eh_valido(tab,jogador,casa,destino))
            return 1;

    /* Diagonal superior esquerda */
    destino.lin = casa.lin - 2;
    destino.col = casa.col - 2;

    if(dentro_do_tabuleiro(destino.lin, destino.col))
        if(comer_eh_valido(tab,jogador,casa,destino))
            return 1;

    return 0;
}

/*
    Verifica se existe pelo menos um movimento possível
    para a peça informada.
*/
short int da_para_mover(
    PontTab tab,
    Jogador jogador,
    Casa casa
)
{
    int direcao;

    /* Caso a peça seja uma dama */
    if(tab[casa.lin][casa.col] == jogador.dama)
    {
        int l;
        int c;

        /*
            Verifica as quatro diagonais adjacentes.
        */
        for(l = -1; l <= 1; l += 2)
        {
            for(c = -1; c <= 1; c += 2)
            {
                if(dentro_do_tabuleiro(
                    casa.lin + l,
                    casa.col + c))
                {
                    if(tab[casa.lin+l][casa.col+c] == ' ')
                        return 1;
                }
            }
        }

        return 0;
    }

    /* Define o sentido de movimentação do peão */
    direcao = (jogador.id == 'C') ? 1 : -1;

    if(dentro_do_tabuleiro(
        casa.lin + direcao,
        casa.col - 1))
    {
        if(tab[casa.lin+direcao][casa.col-1] == ' ')
            return 1;
    }

    if(dentro_do_tabuleiro(
        casa.lin + direcao,
        casa.col + 1))
    {
        if(tab[casa.lin+direcao][casa.col+1] == ' ')
            return 1;
    }

    return 0;
}

/*
    Valida e executa uma jogada.

    Retorna:
        -1 -> jogada inválida
         0 -> movimento simples
         1 -> captura
*/
int jogada(
    PontTab tab,
    Jogador jogador,
    Casa inicial,
    Casa final
)
{
    int l;
    int c;

    /* Verifica se origem e destino pertencem ao tabuleiro */
    if(!dentro_do_tabuleiro(inicial.lin, inicial.col))
        return -1;

    if(!dentro_do_tabuleiro(final.lin, final.col))
        return -1;

    /* Confere se a peça pertence ao jogador da vez */
    if(!peca_do_jogador(tab[inicial.lin][inicial.col], jogador))
        return -1;

    /* Caso exista captura disponível, ela será obrigatória */
    if(da_para_comer(tab, jogador, inicial))
    {
        if(!comer_eh_valido(tab, jogador, inicial, final))
            return -1;

        /* Remove a peça capturada pelo peão */
        if(tab[inicial.lin][inicial.col] == jogador.peao)
        {
            l = (inicial.lin + final.lin)/2;
            c = (inicial.col + final.col)/2;

            tab[l][c] = ' ';
        }
        else
        {
            /* Procura e remove a peça capturada pela dama */
            int sl = (final.lin > inicial.lin) ? 1 : -1;
            int sc = (final.col > inicial.col) ? 1 : -1;

            l = inicial.lin + sl;
            c = inicial.col + sc;

            while(l != final.lin)
            {
                if(peca_adversaria(tab[l][c], jogador))
                {
                    tab[l][c] = ' ';
                    break;
                }

                l += sl;
                c += sc;
            }
        }

        /* Move a peça para o destino */
        tab[final.lin][final.col] =
            tab[inicial.lin][inicial.col];

        /* Libera a casa de origem */
        tab[inicial.lin][inicial.col] = ' ';

        return 1;
    }

    /* Caso não haja captura, verifica movimento simples */
    if(!mover_eh_valido(tab, jogador, inicial, final))
        return -1;

    /* Move a peça */
    tab[final.lin][final.col] =
        tab[inicial.lin][inicial.col];

    tab[inicial.lin][inicial.col] = ' ';

    /* Promove o peão a dama ao alcançar a última linha */
    if(jogador.id == 'C' &&
       final.lin == 9 &&
       tab[final.lin][final.col] == jogador.peao)
    {
        tab[final.lin][final.col] = jogador.dama;
    }

    if(jogador.id == 'B' &&
       final.lin == 0 &&
       tab[final.lin][final.col] == jogador.peao)
    {
        tab[final.lin][final.col] = jogador.dama;
    }

    return 0;
}
