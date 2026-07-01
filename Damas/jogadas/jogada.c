/*
    Codigo feito em 2026.1 pelos estudantes
    Ana Rebeca Mendes de Souza (602242)
    Arthur Araujo Custodio (604827)
    Rafael Calisto Oliveira da Silva (606478)
*/

#include <stdlib.h>
#include "jogada.h"

/*
    verifica se o caractere informado representa
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
    verifica se o caractere representa uma peça
    do adversário.
*/
short int peca_adversaria(char peca, Jogador jogador)
{
    if(peca == ' ' || peca == '#')
        return 0;

    return !peca_do_jogador(peca, jogador);
}

/*
    verifica se existe alguma peça do jogador
    que possua captura obrigatória.
*/
static short int jogador_tem_captura(
    PontTab tab,
    Jogador jogador
)
{
    Casa casa;

    for(casa.lin = 0; casa.lin < 10; casa.lin++)
    {
        for(casa.col = 0; casa.col < 10; casa.col++)
        {
            if(peca_do_jogador(
                tab[casa.lin][casa.col],
                jogador))
            {
                if(da_para_comer(
                    tab,
                    jogador,
                    casa))
                {
                    return 1;
                }
            }
        }
    }

    return 0;
}

/*
    verifica se um movimento sem captura é válido.
*/
short int mover_eh_valido(
    PontTab tab,
    Jogador jogador,
    Casa inicial,
    Casa final
)
{
    int dl = final.lin - inicial.lin;
    int dc = final.col - inicial.col;

    /* O destino deve estar vazio */
    if(tab[final.lin][final.col] != ' ')
        return 0;

    /* dama */

    if(tab[inicial.lin][inicial.col] == jogador.dama)
    {
        int sl, sc;
        int l, c;

        /* deve mover em diagonal */
        if(abs(dl) != abs(dc))
            return 0;

        sl = (dl > 0) ? 1 : -1;
        sc = (dc > 0) ? 1 : -1;

        l = inicial.lin + sl;
        c = inicial.col + sc;

        /*
            todo o caminho deve estar vazio.
            a dama NÃO pode atravessar peças.
        */
        while(l != final.lin)
        {
            if(tab[l][c] != ' ')
                return 0;

            l += sl;
            c += sc;
        }

        return 1;
    }

    /* peão */

    if(jogador.id == 'C')
    {
        if(dl == 1 && abs(dc) == 1)
            return 1;
    }

    if(jogador.id == 'B')
    {
        if(dl == -1 && abs(dc) == 1)
            return 1;
    }

    return 0;
}

/*
    verifica se uma captura é válida.
*/
short int comer_eh_valido(
    PontTab tab,
    Jogador jogador,
    Casa inicial,
    Casa final
)
{
    int dl = final.lin - inicial.lin;
    int dc = final.col - inicial.col;

    /* destino deve estar vazio */
    if(tab[final.lin][final.col] != ' ')
        return 0;

    /* sempre deve mover em diagonal */
    if(abs(dl) != abs(dc))
        return 0;

    /* peão */

    if(tab[inicial.lin][inicial.col] == jogador.peao)
    {
        int ml;
        int mc;

        if(abs(dl) != 2)
            return 0;

        ml = (inicial.lin + final.lin) / 2;
        mc = (inicial.col + final.col) / 2;

        return peca_adversaria(tab[ml][mc], jogador);
    }

    /* dama */

    {
        int sl = (dl > 0) ? 1 : -1;
        int sc = (dc > 0) ? 1 : -1;

        int l = inicial.lin + sl;
        int c = inicial.col + sc;

        int encontrou = 0;

        while(l != final.lin)
        {
            if(tab[l][c] != ' ')
            {
                /* peça do próprio jogador bloqueia */
                if(peca_do_jogador(tab[l][c], jogador))
                    return 0;

                /* segunda peça adversária impede captura */
                if(encontrou)
                    return 0;

                encontrou = 1;
            }

            l += sl;
            c += sc;
        }

        /* deve existir exatamente uma peça adversária */
        return encontrou;
    }
}

/*
    verifica se a peça possui alguma captura disponível.
*/
short int da_para_comer(
    PontTab tab,
    Jogador jogador,
    Casa casa
)
{
    Casa destino;

    /* peão */

    if(tab[casa.lin][casa.col] == jogador.peao)
    {
        destino.lin = casa.lin + 2;
        destino.col = casa.col + 2;

        if(dentro_do_tabuleiro(destino.lin, destino.col))
            if(comer_eh_valido(tab, jogador, casa, destino))
                return 1;

        destino.lin = casa.lin + 2;
        destino.col = casa.col - 2;

        if(dentro_do_tabuleiro(destino.lin, destino.col))
            if(comer_eh_valido(tab, jogador, casa, destino))
                return 1;

        destino.lin = casa.lin - 2;
        destino.col = casa.col + 2;

        if(dentro_do_tabuleiro(destino.lin, destino.col))
            if(comer_eh_valido(tab, jogador, casa, destino))
                return 1;

        destino.lin = casa.lin - 2;
        destino.col = casa.col - 2;

        if(dentro_do_tabuleiro(destino.lin, destino.col))
            if(comer_eh_valido(tab, jogador, casa, destino))
                return 1;

        return 0;
    }

    /* dama */

    {
        int dl[4] = { 1, 1,-1,-1 };
        int dc[4] = { 1,-1, 1,-1 };
        int i;

        for(i = 0; i < 4; i++)
        {
            int l = casa.lin + dl[i];
            int c = casa.col + dc[i];

            while(dentro_do_tabuleiro(l, c))
            {
                destino.lin = l;
                destino.col = c;

                if(comer_eh_valido(
                    tab,
                    jogador,
                    casa,
                    destino))
                {
                    return 1;
                }

                l += dl[i];
                c += dc[i];
            }
        }
    }

    return 0;
}

/*
    verifica se existe pelo menos um movimento possível
    para a peça informada.
*/
short int da_para_mover(
    PontTab tab,
    Jogador jogador,
    Casa casa
)
{
    /* dama */

    if(tab[casa.lin][casa.col] == jogador.dama)
    {
        int dl[4] = { 1, 1,-1,-1 };
        int dc[4] = { 1,-1, 1,-1 };
        int i;

        for(i = 0; i < 4; i++)
        {
            int l = casa.lin + dl[i];
            int c = casa.col + dc[i];

            while(dentro_do_tabuleiro(l, c))
            {
                /*
                    encontrou uma peça e
                    a diagonal está bloqueada.
                */
                if(tab[l][c] != ' ')
                    break;

                /*
                    existe pelo menos um movimento.
                */
                return 1;

                l += dl[i];
                c += dc[i];
            }
        }

        return 0;
    }

    /* peão */

    {
        int direcao;

        direcao = (jogador.id == 'C') ? 1 : -1;

        if(dentro_do_tabuleiro(
            casa.lin + direcao,
            casa.col - 1))
        {
            if(tab[casa.lin + direcao][casa.col - 1] == ' ')
                return 1;
        }

        if(dentro_do_tabuleiro(
            casa.lin + direcao,
            casa.col + 1))
        {
            if(tab[casa.lin + direcao][casa.col + 1] == ' ')
                return 1;
        }

        return 0;
    }
}

