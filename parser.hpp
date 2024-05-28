/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_PARSER_HPP_INCLUDED
# define YY_YY_PARSER_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    MAINSYM = 258,                 /* MAINSYM  */
    IFSYM = 259,                   /* IFSYM  */
    ELSESYM = 260,                 /* ELSESYM  */
    THENSYM = 261,                 /* THENSYM  */
    ENDSYM = 262,                  /* ENDSYM  */
    FORSYM = 263,                  /* FORSYM  */
    WHILESYM = 264,                /* WHILESYM  */
    PRINTSYM = 265,                /* PRINTSYM  */
    SCANSYM = 266,                 /* SCANSYM  */
    RETSYM = 267,                  /* RETSYM  */
    LBRACE = 268,                  /* LBRACE  */
    RBRACE = 269,                  /* RBRACE  */
    LBRACKET = 270,                /* LBRACKET  */
    RBRACKET = 271,                /* RBRACKET  */
    SEMICOLON = 272,               /* SEMICOLON  */
    COMMA = 273,                   /* COMMA  */
    LPAREN = 274,                  /* LPAREN  */
    RPAREN = 275,                  /* RPAREN  */
    OR = 276,                      /* OR  */
    AND = 277,                     /* AND  */
    LSS = 278,                     /* LSS  */
    LEQ = 279,                     /* LEQ  */
    GTR = 280,                     /* GTR  */
    GEQ = 281,                     /* GEQ  */
    EQ = 282,                      /* EQ  */
    NEQ = 283,                     /* NEQ  */
    PLUS = 284,                    /* PLUS  */
    MINUS = 285,                   /* MINUS  */
    TIMES = 286,                   /* TIMES  */
    SLASH = 287,                   /* SLASH  */
    MOD = 288,                     /* MOD  */
    INC = 289,                     /* INC  */
    DEC = 290,                     /* DEC  */
    NOT = 291,                     /* NOT  */
    ADDR = 292,                    /* ADDR  */
    BECOMES = 293,                 /* BECOMES  */
    INTSYM = 294,                  /* INTSYM  */
    STRSYM = 295,                  /* STRSYM  */
    BOOLSYM = 296,                 /* BOOLSYM  */
    ID = 297,                      /* ID  */
    LIT = 298                      /* LIT  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 68 "l24.y"

char *ident;
// int var_size;
int type_name;
int number;

type_desp* type_inf;
arr_idx_s* arr_idx_inf;
literal_s* lit_inf;
expr_s* expr_inf;
args_s* arg_inf;

#line 120 "parser.hpp"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_PARSER_HPP_INCLUDED  */
