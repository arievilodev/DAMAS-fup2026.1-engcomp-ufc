/*
    Codigo feito em 2026.1 pelos estudantes
    Ana Rebeca Mendes de Souza (602242)
    Arthur de Araujo Custodio (604827)
    Rafael Calisto Oliveira da Silva (606478)
*/

#include <stdlib.h>     /* utilizado para a função abs() */
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

    espaços vazios e casas não jogáveis ('#')
    não são considerados peças.
*/
short int peca_adversaria(char peca, Jogador jogador)
{
    if (peca == ' ' || peca == '#')
        return 0;

    return !peca_do_jogador(peca, jogador);
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
    /* diferença entre origem e destino */
    int dl = final.lin - inicial.lin;
    int dc = final.col - inicial.col;

    /* a casa de destino precisa estar vazia */
    if(tab[final.lin][final.col] != ' ')
        return 0;

    /* movimento de dama */
    if(tab[inicial.lin][inicial.col] == jogador.dama)
    {
        int sl, sc;
        int l, c;

        /* o deslocamento deve ocorrer em uma diagonal */
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

    /* movimento do jogador de cima */
    if(jogador.id == 'C')
    {
        if(dl == 1 && abs(dc) == 1)
            return 1;
    }

    /* movimento do jogador de baixo */
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

    /* o destino deve estar vazio */
    if(tab[final.lin][final.col] != ' ')
        return 0;

    /* sempre deve mover em diagonal */
    if(abs(dl) != abs(dc))
        return 0;

    /* captura realizada por um peão */
    if(tab[inicial.lin][inicial.col] == jogador.peao)
    {
        /* o peão deve saltar exatamente duas casas */
        int ml;
        int mc;

        if(abs(dl) != 2)
            return 0;

        /* calcula a posição da peça capturada */
        ml = (inicial.lin + final.lin) / 2;
        mc = (inicial.col + final.col) / 2;

        return peca_adversaria(tab[ml][mc], jogador);
    }

    /* captura realizada por uma dama */

    {
        /* sentido do deslocamento */
        int sl = (dl > 0) ? 1 : -1;
        int sc = (dc > 0) ? 1 : -1;

        int l = inicial.lin + sl;
        int c = inicial.col + sc;

        int encontrou = 0;

        /*
            percorre toda a diagonal procurando
            exatamente uma peça adversária.
        */
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
                    encontrou uma peça.
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

/*
    verifica se existe alguma peça do jogador
    que possua captura obrigatória.
*/
short int jogador_tem_captura(
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
    valida e executa uma jogada.

    retorna:
        -1 -> jogada inválida
         0 -> movimento simples
         1 -> captura
*/
short int jogada(
    PontTab tab,
    Jogador jogador,
    Casa inicial,
    Casa final
)
{
    int l;
    int c;

    /* verifica se origem e destino pertencem ao tabuleiro */
    if(!dentro_do_tabuleiro(inicial.lin, inicial.col))
        return -1;

    if(!dentro_do_tabuleiro(final.lin, final.col))
        return -1;

    /* confere se a peça pertence ao jogador da vez */
    if(!peca_do_jogador(tab[inicial.lin][inicial.col], jogador))
        return -1;

    /* caso exista captura disponível, ela será obrigatória */
    if(jogador_tem_captura(tab, jogador))
    {
        if(!comer_eh_valido(tab, jogador, inicial, final))
            return -1;

        /* remove a peça capturada pelo peão */
        if(tab[inicial.lin][inicial.col] == jogador.peao)
        {
            l = (inicial.lin + final.lin)/2;
            c = (inicial.col + final.col)/2;

            tab[l][c] = ' ';
        }
        else
        {
            /* procura e remove a peça capturada pela dama */
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

        /* move a peça para o destino */
        tab[final.lin][final.col] =
            tab[inicial.lin][inicial.col];

        /* libera a casa de origem */
        tab[inicial.lin][inicial.col] = ' ';

        return 1;
    }

    /* caso não haja captura, verifica movimento simples */
    if(!mover_eh_valido(tab, jogador, inicial, final))
        return -1;

    /* move a peça */
    tab[final.lin][final.col] =
        tab[inicial.lin][inicial.col];

    tab[inicial.lin][inicial.col] = ' ';

    /* promove o peão a dama ao alcançar a última linha */
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
