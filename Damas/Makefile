#   
#	Codigo feito em 2026.1 pelos estudantes
#	Ana Rebeca Mendes de Souza (602242)
#	Arthur de Araujo Custodio (604827)
#	Rafael Calisto Oliveira da Silva (606478)
#

ALL: damas

damas:	main.o jogo.o tabuleiro.o jogada.o
		gcc -std=c99 -Wall -Wextra main.o jogo.o tabuleiro.o jogada.o -o damas

main.o:	main.c jogo.h
		gcc -std=c99 -Wall -Wextra -c main.c

jogo.o:	jogo.c jogo.h tabuleiro.h jogada.h
		gcc -std=c99 -Wall -Wextra -c jogo.c

tabuleiro.o: tabuleiro.c tabuleiro.h
		gcc -std=c99 -Wall -Wextra -c tabuleiro.c

jogada.o: jogada.c jogada.h tabuleiro.h
		gcc -std=c99 -Wall -Wextra -c jogada.c

clean:
		rm -f *.o *.exe damas

clean_win:
		del /Q *.o damas.exe damas
