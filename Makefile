ALL: main

main:	main.o jogo.o tabuleiro.o jogada.o
		gcc -std=c89 -pedantic-errors -Wall -Wextra main.o jogo.o tabuleiro.o jogada.o -o main

main.o:	main.c jogo.h
		gcc -std=c89 -pedantic-errors -Wall -Wextra -c main.c

jogo.o:	jogo.c jogo.h tabuleiro.h jogada.h
		gcc -std=c89 -pedantic-errors -Wall -Wextra -c jogo.c

tabuleiro.o: tabuleiro.c tabuleiro.h
		gcc -std=c89 -pedantic-errors -Wall _Wextra -c tabuleiro.c

jogada.o: jogada.c jogada.h tabuleiro.h

clean:
		rm -f *.o main.exe main

clean_win:
		del /Q *.o main.exe main
