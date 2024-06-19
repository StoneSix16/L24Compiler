LEX = ../win_flex_bison/win_flex.exe
YACC = ../win_flex_bison/win_bison.exe
CC = g++
OPT = -std=c++11


L24Parser: lexer.cpp parser.cpp
	$(CC) $(OPT) lexer.cpp parser.cpp table.cpp vm.cpp -o $@

lexer.hpp lexer.cpp: l24.l
	$(LEX) -o lexer.cpp --header-file=lexer.hpp l24.l

parser.hpp parser.cpp: l24.y
	$(YACC) -Wcounterexamples -d -v -o parser.cpp l24.y

clean:
	rm parser.*
	rm lexer.*