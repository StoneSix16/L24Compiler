%{
#include "stdio.h"
#include "string.h"
#include "malloc.h"
#include "table.hpp"
#include "parser.hpp"


int read_char(char* buf,int* result, int max_size);
extern FILE* flog;
FILE* lexoutput = NULL;
int line=1;
int err_line = line;
int last_char;
void yyerror(const char *msg);
void print_source(FILE* output);
// int yywrap();

%}

%%
[ \t]		{print_source(lexoutput);}
[\n]		{print_source(lexoutput);if(err_line == line) err_line++;line++;}
"main"		{print_source(lexoutput);return MAINSYM;}
"if"		{print_source(lexoutput);return IFSYM;}
"else"		{print_source(lexoutput);return ELSESYM;}
"then"      {print_source(lexoutput);return THENSYM;}
"end"       {print_source(lexoutput);return ENDSYM;}
"for"		{print_source(lexoutput);return FORSYM;}
"while"		{print_source(lexoutput);return WHILESYM;}
"print"		{print_source(lexoutput);return PRINTSYM;}
"scan"		{print_source(lexoutput);return SCANSYM;}
"int"		{print_source(lexoutput);yylval.type_name = lint;return INTSYM;}
"string"	{print_source(lexoutput);yylval.type_name = lstring;return STRSYM;}
"return" 	{print_source(lexoutput);return RETSYM;}
"bool"		{print_source(lexoutput);yylval.type_name = lbool;return BOOLSYM;}

"{"			{print_source(lexoutput);return LBRACE;}
"}"			{print_source(lexoutput);return RBRACE;}
"["			{print_source(lexoutput);return LBRACKET;}
"]"			{print_source(lexoutput);return RBRACKET;}
";"			{print_source(lexoutput);return SEMICOLON;}
","			{print_source(lexoutput);return COMMA;}
"("			{print_source(lexoutput);return LPAREN;}
")"			{print_source(lexoutput);return RPAREN;}

"="			{print_source(lexoutput);return BECOMES;}
"<"			{print_source(lexoutput);return LSS;}
"<="		{print_source(lexoutput);return LEQ;}
">"			{print_source(lexoutput);return GTR;}
">="		{print_source(lexoutput);return GEQ;}
"=="		{print_source(lexoutput);return EQ;}
"!="		{print_source(lexoutput);return NEQ;}
"+"			{print_source(lexoutput);return PLUS;}
"-"			{print_source(lexoutput);return MINUS;}
"*"			{print_source(lexoutput);return TIMES;}
"/"			{print_source(lexoutput);return	SLASH;}
"%"			{print_source(lexoutput);return	MOD;}

"&&"		{print_source(lexoutput);return AND;}
"||"		{print_source(lexoutput);return OR;}
"!"		    {print_source(lexoutput);return NOT;}


"&"			{print_source(lexoutput);return ADDR;}
"++"		{print_source(lexoutput);return INC;}
"--"		{print_source(lexoutput);return DEC;}


"true"  {
	print_source(lexoutput);
	yylval.lit_inf = (literal_s*)malloc(sizeof(literal_s));
	yylval.lit_inf -> t = lbool;
	yylval.lit_inf -> val = 1;
    yylval.lit_inf -> str = NULL;
	return LIT;}
"false" {
	print_source(lexoutput);
	yylval.lit_inf = (literal_s*)malloc(sizeof(literal_s));
	yylval.lit_inf -> t = lbool;
	yylval.lit_inf -> val = 0;
    yylval.lit_inf -> str = NULL;
	return LIT;}

[a-zA-Z_$]+[a-zA-Z_$0-9]*	{
	print_source(lexoutput);
	yylval.ident=(char *)malloc(strlen(yytext)+1);
                                 strcpy(yylval.ident, yytext);
                                 return ID;
}

[1-9]+[0-9]*|0	{
	print_source(lexoutput);

	yylval.lit_inf = (literal_s*)malloc(sizeof(literal_s));
	yylval.lit_inf -> t = lint;
	yylval.lit_inf -> val = atoi(yytext);
    yylval.lit_inf -> str = NULL;
	return LIT;}
\"([^\"\\]|\\.)*\" {
	print_source(lexoutput);

	yylval.lit_inf = (literal_s*)malloc(sizeof(literal_s));
	yylval.lit_inf -> t = lstring;
	yylval.lit_inf -> val = 0;
	int len = strlen(yytext), p = 1, i = 0;
	yylval.lit_inf -> str = (char*) malloc(strlen(yytext));

	while(p < len - 1){
		if(yytext[p] == '\\'){
			switch (yytext[p+1]){
				case 'n':
					yylval.lit_inf -> str[i] = '\n';
					break;
				case '\\':
					yylval.lit_inf -> str[i] = '\\';
					break;
				case 't':
					yylval.lit_inf -> str[i] = '\t';
					break;
				case 'a':
					yylval.lit_inf -> str[i] = '\a';
					break;
				case 'b':
					yylval.lit_inf -> str[i] = '\b';
					break;
				case 'r':
					yylval.lit_inf -> str[i] = '\r';
					break;
				case '0':
					yylval.lit_inf -> str[i] = '\0';
					break;
				case '\'':
					yylval.lit_inf -> str[i] = '\'';
					break;
				case '\"':
					yylval.lit_inf -> str[i] = '\"';
					break;
				case '?':
					yylval.lit_inf -> str[i] = '\?';
					break;
				default:
					yyerror("Unknown escape character!");
					break;
			}
			i+=1;
			p+=2;
		}
		else{
			yylval.lit_inf -> str[i] = yytext[p];
			p+=1; i+=1;
		}
	}
	yylval.lit_inf -> str[i]='\0';
    // strcpy(yylval.lit_inf -> str, yytext);
	return LIT;}

(\/\*).*(\*\/) {}
.	{yyerror("Unknown character!");}

%%

void print_source(FILE* lexoutput){
	if(lexoutput) fprintf(lexoutput, "%s", yytext);
}

int read_char(char* buf,int* result, int max_size){
	int i;
	for(i = 0; i < max_size; i++){
		buf[i] = getc(yyin);
		/* printf("%c", buf[i]); */
	}
	last_char = buf[i-1];
    *result = i;
    return 0;
}

int yywrap(void) {
	return 1;
}