/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 3 "l24.y"

#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include<memory.h>
#include<string.h>

#include "vm.hpp"
#include "lexer.hpp"

// int tx;         /* 符号表当前尾指针 */
// int cx;         /* 虚拟机代码指针, 取值范围[0, cxmax-1] */
// int px;         /* 嵌套过程索引表proctable的指针 */
int cur_level;        /* 层次记录 */
int cur_block_pc; /* 存储当前函数体的起始位置 */
int tbl_idx_proc[MAX_NEST_DEPTH];
int num_ret[MAX_NEST_DEPTH];
type_desp* type_ret[MAX_NEST_DEPTH];
bool push_flag; /* 是否在声明变量时申请栈空间 */
// int proctable[3];   /* 嵌套过程索引表，最多嵌套三层 */
// char id[al];
// int num;
// bool listswitch;   /* 显示虚拟机代码与否 */
// bool tableswitch;  /* 显示符号表与否 */

FILE* fsource;      /* 输入源文件 */
FILE* flog = NULL;	  /* 编译日志输出 */
FILE* finput = NULL;	  /* 程序执行输入 */
FILE* foutput = NULL;	  /* 程序执行输出 */
FILE* fcode = NULL;    /* 输出虚拟机代码 */
FILE* finfo = NULL;     /* 堆栈信息 */

int err;
extern int last_char;
extern int line; 
extern int err_line;
bool err_args(int proc_tbl_idx, args_s* args, const char* add = NULL);
bool err_ident(int tbl_idx, const char* add = NULL);
bool err_lval(int islval, const char* add = NULL);
bool err_type(type_desp* expr_type, int t, bool eq = true, const char* add = NULL);
bool err_dim(int expr_dim, int decl_dim, const char* add = NULL);
void yyerror(const char *msg);

expr_s* gen_cmp_code(int op, expr_s* s1, expr_s* s3);
/*
用于二元逻辑运算的语义计算
生成一条二元逻辑运算的虚拟机代码
返回一个类型为lbool的expr_s
*/

// void init();
// void enter(enum object k);
// int position(char *s);
// void setdx(int n);
// void gen(enum fct x, int y, int z);
// void listall();
// void displaytable();
// void interpret();
// int base(int l, int* s, int b);

#line 132 "parser.cpp"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "parser.hpp"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_MAINSYM = 3,                    /* MAINSYM  */
  YYSYMBOL_IFSYM = 4,                      /* IFSYM  */
  YYSYMBOL_ELSESYM = 5,                    /* ELSESYM  */
  YYSYMBOL_THENSYM = 6,                    /* THENSYM  */
  YYSYMBOL_ENDSYM = 7,                     /* ENDSYM  */
  YYSYMBOL_FORSYM = 8,                     /* FORSYM  */
  YYSYMBOL_WHILESYM = 9,                   /* WHILESYM  */
  YYSYMBOL_PRINTSYM = 10,                  /* PRINTSYM  */
  YYSYMBOL_SCANSYM = 11,                   /* SCANSYM  */
  YYSYMBOL_RETSYM = 12,                    /* RETSYM  */
  YYSYMBOL_LBRACE = 13,                    /* LBRACE  */
  YYSYMBOL_RBRACE = 14,                    /* RBRACE  */
  YYSYMBOL_LBRACKET = 15,                  /* LBRACKET  */
  YYSYMBOL_RBRACKET = 16,                  /* RBRACKET  */
  YYSYMBOL_SEMICOLON = 17,                 /* SEMICOLON  */
  YYSYMBOL_COMMA = 18,                     /* COMMA  */
  YYSYMBOL_LPAREN = 19,                    /* LPAREN  */
  YYSYMBOL_RPAREN = 20,                    /* RPAREN  */
  YYSYMBOL_OR = 21,                        /* OR  */
  YYSYMBOL_AND = 22,                       /* AND  */
  YYSYMBOL_LSS = 23,                       /* LSS  */
  YYSYMBOL_LEQ = 24,                       /* LEQ  */
  YYSYMBOL_GTR = 25,                       /* GTR  */
  YYSYMBOL_GEQ = 26,                       /* GEQ  */
  YYSYMBOL_EQ = 27,                        /* EQ  */
  YYSYMBOL_NEQ = 28,                       /* NEQ  */
  YYSYMBOL_PLUS = 29,                      /* PLUS  */
  YYSYMBOL_MINUS = 30,                     /* MINUS  */
  YYSYMBOL_TIMES = 31,                     /* TIMES  */
  YYSYMBOL_SLASH = 32,                     /* SLASH  */
  YYSYMBOL_MOD = 33,                       /* MOD  */
  YYSYMBOL_INC = 34,                       /* INC  */
  YYSYMBOL_DEC = 35,                       /* DEC  */
  YYSYMBOL_NOT = 36,                       /* NOT  */
  YYSYMBOL_ADDR = 37,                      /* ADDR  */
  YYSYMBOL_BECOMES = 38,                   /* BECOMES  */
  YYSYMBOL_INTSYM = 39,                    /* INTSYM  */
  YYSYMBOL_STRSYM = 40,                    /* STRSYM  */
  YYSYMBOL_BOOLSYM = 41,                   /* BOOLSYM  */
  YYSYMBOL_ID = 42,                        /* ID  */
  YYSYMBOL_LIT = 43,                       /* LIT  */
  YYSYMBOL_YYACCEPT = 44,                  /* $accept  */
  YYSYMBOL_program = 45,                   /* program  */
  YYSYMBOL_46_1 = 46,                      /* $@1  */
  YYSYMBOL_stmt_list = 47,                 /* stmt_list  */
  YYSYMBOL_stmt = 48,                      /* stmt  */
  YYSYMBOL_49_2 = 49,                      /* $@2  */
  YYSYMBOL_50_3 = 50,                      /* $@3  */
  YYSYMBOL_51_4 = 51,                      /* $@4  */
  YYSYMBOL_52_5 = 52,                      /* $@5  */
  YYSYMBOL_53_6 = 53,                      /* $@6  */
  YYSYMBOL_54_7 = 54,                      /* $@7  */
  YYSYMBOL_proc = 55,                      /* proc  */
  YYSYMBOL_56_8 = 56,                      /* $@8  */
  YYSYMBOL_57_9 = 57,                      /* $@9  */
  YYSYMBOL_ret_stmt = 58,                  /* ret_stmt  */
  YYSYMBOL_end_block = 59,                 /* end_block  */
  YYSYMBOL_type_spec = 60,                 /* type_spec  */
  YYSYMBOL_pointer = 61,                   /* pointer  */
  YYSYMBOL_type = 62,                      /* type  */
  YYSYMBOL_declarator = 63,                /* declarator  */
  YYSYMBOL_decl_stmt = 64,                 /* decl_stmt  */
  YYSYMBOL_65_10 = 65,                     /* $@10  */
  YYSYMBOL_66_11 = 66,                     /* @11  */
  YYSYMBOL_67_12 = 67,                     /* $@12  */
  YYSYMBOL_arr_dim = 68,                   /* arr_dim  */
  YYSYMBOL_arr_idx = 69,                   /* arr_idx  */
  YYSYMBOL_param_list = 70,                /* param_list  */
  YYSYMBOL_arg_list = 71,                  /* arg_list  */
  YYSYMBOL_expr_stmt = 72,                 /* expr_stmt  */
  YYSYMBOL_condition = 73,                 /* condition  */
  YYSYMBOL_if_stmt = 74,                   /* if_stmt  */
  YYSYMBOL_75_13 = 75,                     /* @13  */
  YYSYMBOL_76_14 = 76,                     /* $@14  */
  YYSYMBOL_if_part_ifthen = 77,            /* if_part_ifthen  */
  YYSYMBOL_78_15 = 78,                     /* @15  */
  YYSYMBOL_if_part_else = 79,              /* if_part_else  */
  YYSYMBOL_while_stmt = 80,                /* while_stmt  */
  YYSYMBOL_81_16 = 81,                     /* @16  */
  YYSYMBOL_82_17 = 82,                     /* @17  */
  YYSYMBOL_for_expr = 83,                  /* for_expr  */
  YYSYMBOL_for_substmt = 84,               /* for_substmt  */
  YYSYMBOL_for_stmt = 85,                  /* for_stmt  */
  YYSYMBOL_86_18 = 86,                     /* @18  */
  YYSYMBOL_87_19 = 87,                     /* @19  */
  YYSYMBOL_scan_item = 88,                 /* scan_item  */
  YYSYMBOL_print_item = 89,                /* print_item  */
  YYSYMBOL_scan_list = 90,                 /* scan_list  */
  YYSYMBOL_print_list = 91,                /* print_list  */
  YYSYMBOL_scan_stmt = 92,                 /* scan_stmt  */
  YYSYMBOL_print_stmt = 93,                /* print_stmt  */
  YYSYMBOL_expr = 94,                      /* expr  */
  YYSYMBOL_and_expr = 95,                  /* and_expr  */
  YYSYMBOL_cmp_expr = 96,                  /* cmp_expr  */
  YYSYMBOL_add_expr = 97,                  /* add_expr  */
  YYSYMBOL_multi_expr = 98,                /* multi_expr  */
  YYSYMBOL_unary_expr = 99,                /* unary_expr  */
  YYSYMBOL_pre_expr = 100,                 /* pre_expr  */
  YYSYMBOL_post_expr = 101,                /* post_expr  */
  YYSYMBOL_term = 102,                     /* term  */
  YYSYMBOL_factor = 103,                   /* factor  */
  YYSYMBOL_104_20 = 104,                   /* $@20  */
  YYSYMBOL_105_21 = 105,                   /* $@21  */
  YYSYMBOL_rec_cbuf_pc = 106,              /* rec_cbuf_pc  */
  YYSYMBOL_load_params = 107               /* load_params  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if 1

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* 1 */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  4
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   378

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  44
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  64
/* YYNRULES -- Number of rules.  */
#define YYNRULES  123
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  225

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   298


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   112,   112,   111,   135,   138,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   149,   150,   150,   151,   151,
     152,   152,   153,   153,   154,   154,   155,   162,   161,   179,
     178,   197,   218,   232,   236,   243,   247,   252,   256,   260,
     267,   272,   278,   313,   326,   338,   312,   351,   364,   379,
     391,   407,   419,   431,   451,   471,   489,   500,   511,   513,
     518,   512,   524,   523,   533,   538,   542,   537,   555,   556,
     558,   558,   560,   564,   559,   579,   591,   600,   602,   605,
     607,   610,   613,   618,   622,   638,   642,   658,   662,   678,
     694,   709,   724,   739,   755,   759,   814,   835,   839,   882,
     903,   925,   929,   946,   950,   965,   979,   997,  1015,  1019,
    1037,  1057,  1061,  1097,  1125,  1129,  1154,  1228,  1227,  1257,
    1256,  1283,  1302,  1307
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if 1
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "MAINSYM", "IFSYM",
  "ELSESYM", "THENSYM", "ENDSYM", "FORSYM", "WHILESYM", "PRINTSYM",
  "SCANSYM", "RETSYM", "LBRACE", "RBRACE", "LBRACKET", "RBRACKET",
  "SEMICOLON", "COMMA", "LPAREN", "RPAREN", "OR", "AND", "LSS", "LEQ",
  "GTR", "GEQ", "EQ", "NEQ", "PLUS", "MINUS", "TIMES", "SLASH", "MOD",
  "INC", "DEC", "NOT", "ADDR", "BECOMES", "INTSYM", "STRSYM", "BOOLSYM",
  "ID", "LIT", "$accept", "program", "$@1", "stmt_list", "stmt", "$@2",
  "$@3", "$@4", "$@5", "$@6", "$@7", "proc", "$@8", "$@9", "ret_stmt",
  "end_block", "type_spec", "pointer", "type", "declarator", "decl_stmt",
  "$@10", "@11", "$@12", "arr_dim", "arr_idx", "param_list", "arg_list",
  "expr_stmt", "condition", "if_stmt", "@13", "$@14", "if_part_ifthen",
  "@15", "if_part_else", "while_stmt", "@16", "@17", "for_expr",
  "for_substmt", "for_stmt", "@18", "@19", "scan_item", "print_item",
  "scan_list", "print_list", "scan_stmt", "print_stmt", "expr", "and_expr",
  "cmp_expr", "add_expr", "multi_expr", "unary_expr", "pre_expr",
  "post_expr", "term", "factor", "$@20", "$@21", "rec_cbuf_pc",
  "load_params", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-189)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-120)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
       8,  -189,    37,  -189,  -189,    50,  -189,    79,     2,    73,
    -189,    78,    83,   -12,  -189,  -189,   -12,    -3,    -3,    -3,
      -3,    -3,   -12,    -3,  -189,  -189,  -189,    17,  -189,  -189,
      68,    58,    86,    87,   106,   107,   118,  -189,  -189,   109,
     110,    -9,   108,    20,    21,    40,  -189,  -189,    72,  -189,
    -189,   -12,   -12,   112,   -12,   -12,   111,    55,    72,    72,
    -189,    72,    72,  -189,  -189,   -12,   113,   114,  -189,   127,
     119,   116,  -189,   117,  -189,   136,  -189,   139,  -189,   140,
    -189,   137,  -189,   149,  -189,   150,   -12,   -12,   -12,   -12,
     -12,   -12,   -12,   -12,   -12,   -12,   -12,   -12,   -12,   -12,
    -189,  -189,   133,   111,  -189,   141,   138,   -12,  -189,    49,
     111,  -189,    66,   111,  -189,    -7,   -12,   143,   -12,   144,
     124,   142,  -189,  -189,   151,   156,   163,   147,  -189,   164,
     165,   108,   111,    20,    21,    21,    21,    21,    21,    21,
      40,    40,  -189,  -189,  -189,   182,   -12,  -189,   170,   -12,
    -189,   -12,  -189,  -189,    81,   111,  -189,    -6,  -189,   177,
     148,   176,  -189,  -189,  -189,  -189,   189,  -189,  -189,  -189,
    -189,   -12,  -189,  -189,  -189,   -12,  -189,  -189,  -189,   181,
     178,  -189,   135,  -189,   186,   183,   188,   111,  -189,  -189,
      41,  -189,  -189,  -189,  -189,  -189,  -189,  -189,   184,  -189,
     175,  -189,   215,   190,    41,   187,  -189,   -12,  -189,  -189,
    -189,  -189,   193,   255,  -189,  -189,  -189,   195,   207,  -189,
    -189,   295,   335,  -189,  -189
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     2,     0,   122,     1,     0,     4,     0,     0,     0,
      65,     0,     0,     0,     3,    26,     0,     0,     0,     0,
       0,     0,     0,     0,    38,    37,    39,   115,   121,     5,
      20,     0,    33,    14,    16,    18,    59,     9,    10,    22,
      24,    56,    83,    85,    87,    94,    97,   101,   103,   108,
     111,     0,    70,     0,     0,     0,    31,     0,   106,   107,
     113,   104,   105,   102,   112,     0,   117,   116,    11,     0,
      40,    42,    35,    34,     6,     0,     7,     0,     8,     0,
      58,     0,    12,     0,    13,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     109,   110,     0,    57,    68,    71,     0,     0,    79,     0,
      76,    77,     0,    75,   114,     0,     0,     0,     0,     0,
       0,    41,    44,    36,     0,     0,     0,     0,    60,     0,
       0,    84,    55,    86,    90,    91,    92,    93,    88,    89,
      95,    96,    98,    99,   100,     0,     0,    72,     0,     0,
      82,     0,    81,    49,     0,    53,   120,     0,    21,     0,
       0,     0,    15,    17,    19,     4,     0,    23,    25,    62,
      69,     0,    66,    80,    78,     0,   118,    50,    47,     0,
     123,    32,     0,    61,     0,     0,     0,    54,    48,    29,
       0,    45,    64,     4,    73,     4,   122,    51,   123,    46,
       0,   122,     0,     0,     0,     0,    63,    70,    67,     4,
      52,    27,     0,     0,   122,   122,    30,     0,     0,     4,
       4,     0,     0,    28,    74
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -189,  -189,  -189,  -160,  -189,  -189,  -189,  -189,  -189,  -189,
    -189,  -189,  -189,  -189,  -189,  -189,  -189,  -189,  -189,  -189,
    -184,  -189,  -189,  -189,  -189,  -189,  -189,  -189,   -51,  -103,
    -189,  -189,  -189,  -189,  -189,  -189,  -189,  -189,  -189,  -189,
      14,  -189,  -189,  -189,    71,    82,  -189,  -189,  -189,  -189,
     -13,   152,   145,   -36,    16,   -20,  -189,    44,    47,  -189,
    -189,  -189,  -188,    30
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     2,     3,     7,    29,    75,    77,    79,    69,    83,
      85,   181,   214,   196,    30,   191,    31,    73,    32,    71,
      33,   122,   161,   199,   121,    67,   198,   154,    34,   102,
      35,    81,   166,    36,   184,   128,    37,    53,   186,   105,
     106,    38,   171,   201,   111,   108,   112,   109,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
     116,   117,     5,   190
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      56,   104,    63,    57,   148,   182,   197,    16,   203,   153,
     177,     1,    86,   207,    86,    86,    16,    17,    18,    19,
     210,    51,    20,    21,    22,    23,   217,   218,    19,    87,
      27,    28,    65,   200,    23,   202,    66,     4,   103,    27,
      28,   110,   113,    89,    90,    91,    92,    93,    94,   213,
      95,    96,   115,   134,   135,   136,   137,   138,   139,   221,
     222,    58,    59,     6,    61,    62,    60,   149,   185,   150,
      64,    97,    98,    99,   132,   114,    86,   142,   143,   144,
      24,    25,    26,     8,   151,    68,   152,     9,    10,    11,
      12,    13,    52,    14,   103,   170,    15,    54,    16,   175,
      70,   176,    55,   155,    74,   157,   100,   101,    17,    18,
      19,   140,   141,    20,    21,    22,    23,    72,    24,    25,
      26,    27,    28,    76,    78,    80,    82,    84,   119,   118,
      88,   107,    86,  -119,   120,   -43,   110,   124,   113,     8,
     125,   126,   127,     9,    10,    11,    12,    13,   123,   192,
     129,   130,    15,   145,    16,   147,   104,   160,   103,   146,
     165,   158,   187,   156,    17,    18,    19,   159,   162,    20,
      21,    22,    23,   163,    24,    25,    26,    27,    28,     8,
     164,   167,   168,     9,    10,    11,    12,    13,   169,   206,
     172,   179,    15,   178,    16,   180,   183,   188,   189,   193,
     194,   195,   204,   209,    17,    18,    19,   211,   219,    20,
      21,    22,    23,   215,    24,    25,    26,    27,    28,     8,
     220,   212,   174,     9,    10,    11,    12,    13,   205,   208,
       0,   173,    15,   133,    16,     0,     0,     0,   131,     0,
       0,     0,     0,     0,    17,    18,    19,     0,     0,    20,
      21,    22,    23,     0,    24,    25,    26,    27,    28,     8,
       0,     0,     0,     9,    10,    11,    12,    13,     0,   216,
       0,     0,    15,     0,    16,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    17,    18,    19,     0,     0,    20,
      21,    22,    23,     0,    24,    25,    26,    27,    28,     8,
       0,     0,     0,     9,    10,    11,    12,    13,     0,   223,
       0,     0,    15,     0,    16,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    17,    18,    19,     0,     0,    20,
      21,    22,    23,     0,    24,    25,    26,    27,    28,     8,
       0,     0,     0,     9,    10,    11,    12,    13,     0,   224,
       0,     0,    15,     0,    16,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    17,    18,    19,     0,     0,    20,
      21,    22,    23,     0,    24,    25,    26,    27,    28
};

static const yytype_int16 yycheck[] =
{
      13,    52,    22,    16,   107,   165,   190,    19,   196,    16,
      16,     3,    21,   201,    21,    21,    19,    29,    30,    31,
     204,    19,    34,    35,    36,    37,   214,   215,    31,    38,
      42,    43,    15,   193,    37,   195,    19,     0,    51,    42,
      43,    54,    55,    23,    24,    25,    26,    27,    28,   209,
      29,    30,    65,    89,    90,    91,    92,    93,    94,   219,
     220,    17,    18,    13,    20,    21,    19,    18,   171,    20,
      23,    31,    32,    33,    87,    20,    21,    97,    98,    99,
      39,    40,    41,     4,    18,    17,    20,     8,     9,    10,
      11,    12,    19,    14,   107,   146,    17,    19,    19,    18,
      42,    20,    19,   116,    17,   118,    34,    35,    29,    30,
      31,    95,    96,    34,    35,    36,    37,    31,    39,    40,
      41,    42,    43,    17,    17,     7,    17,    17,     1,    15,
      22,    19,    21,    20,    15,    19,   149,     1,   151,     4,
       1,     1,     5,     8,     9,    10,    11,    12,    31,    14,
       1,     1,    17,    20,    19,    17,   207,    15,   171,    18,
      13,    17,   175,    20,    29,    30,    31,    43,    17,    34,
      35,    36,    37,    17,    39,    40,    41,    42,    43,     4,
      17,    17,    17,     8,     9,    10,    11,    12,     6,    14,
      20,    43,    17,    16,    19,    19,     7,    16,    20,    13,
      17,    13,    18,    13,    29,    30,    31,    20,    13,    34,
      35,    36,    37,    20,    39,    40,    41,    42,    43,     4,
      13,   207,   151,     8,     9,    10,    11,    12,   198,    14,
      -1,   149,    17,    88,    19,    -1,    -1,    -1,    86,    -1,
      -1,    -1,    -1,    -1,    29,    30,    31,    -1,    -1,    34,
      35,    36,    37,    -1,    39,    40,    41,    42,    43,     4,
      -1,    -1,    -1,     8,     9,    10,    11,    12,    -1,    14,
      -1,    -1,    17,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    29,    30,    31,    -1,    -1,    34,
      35,    36,    37,    -1,    39,    40,    41,    42,    43,     4,
      -1,    -1,    -1,     8,     9,    10,    11,    12,    -1,    14,
      -1,    -1,    17,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    29,    30,    31,    -1,    -1,    34,
      35,    36,    37,    -1,    39,    40,    41,    42,    43,     4,
      -1,    -1,    -1,     8,     9,    10,    11,    12,    -1,    14,
      -1,    -1,    17,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    29,    30,    31,    -1,    -1,    34,
      35,    36,    37,    -1,    39,    40,    41,    42,    43
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,    45,    46,     0,   106,    13,    47,     4,     8,
       9,    10,    11,    12,    14,    17,    19,    29,    30,    31,
      34,    35,    36,    37,    39,    40,    41,    42,    43,    48,
      58,    60,    62,    64,    72,    74,    77,    80,    85,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,    19,    19,    81,    19,    19,    94,    94,   101,   101,
     102,   101,   101,    99,   102,    15,    19,    69,    17,    52,
      42,    63,    31,    61,    17,    49,    17,    50,    17,    51,
       7,    75,    17,    53,    17,    54,    21,    38,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    73,    94,    72,    83,    84,    19,    89,    91,
      94,    88,    90,    94,    20,    94,   104,   105,    15,     1,
      15,    68,    65,    31,     1,     1,     1,     5,    79,     1,
       1,    95,    94,    96,    97,    97,    97,    97,    97,    97,
      98,    98,    99,    99,    99,    20,    18,    17,    73,    18,
      20,    18,    20,    16,    71,    94,    20,    94,    17,    43,
      15,    66,    17,    17,    17,    13,    76,    17,    17,     6,
      72,    86,    20,    89,    88,    18,    20,    16,    16,    43,
      19,    55,    47,     7,    78,    73,    82,    94,    16,    20,
     107,    59,    14,    13,    17,    13,    57,    64,    70,    67,
      47,    87,    47,   106,    18,   107,    14,   106,    14,    13,
      64,    20,    84,    47,    56,    20,    14,   106,   106,    13,
      13,    47,    47,    14,    14
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    44,    46,    45,    47,    47,    48,    48,    48,    48,
      48,    48,    48,    48,    49,    48,    50,    48,    51,    48,
      52,    48,    53,    48,    54,    48,    48,    56,    55,    57,
      55,    58,    59,    60,    60,    61,    61,    62,    62,    62,
      63,    63,    64,    65,    66,    67,    64,    68,    68,    69,
      69,    70,    70,    71,    71,    72,    72,    73,    74,    75,
      76,    74,    78,    77,    79,    81,    82,    80,    83,    83,
      84,    84,    86,    87,    85,    88,    89,    90,    90,    91,
      91,    92,    93,    94,    94,    95,    95,    96,    96,    96,
      96,    96,    96,    96,    97,    97,    97,    98,    98,    98,
      98,    99,    99,   100,   100,   100,   100,   100,   101,   101,
     101,   102,   102,   102,   103,   103,   103,   104,   103,   105,
     103,   103,   106,   107
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     6,     0,     2,     2,     2,     2,     1,
       1,     2,     2,     2,     0,     4,     0,     4,     0,     4,
       0,     4,     0,     4,     0,     4,     1,     0,    10,     0,
       7,     2,     0,     1,     2,     1,     2,     1,     1,     1,
       1,     2,     2,     0,     0,     0,     7,     3,     4,     3,
       4,     1,     3,     1,     3,     3,     1,     1,     2,     0,
       0,     5,     0,     9,     4,     0,     0,     9,     1,     3,
       0,     1,     0,     0,    15,     1,     1,     1,     3,     1,
       3,     4,     4,     1,     3,     1,     3,     1,     3,     3,
       3,     3,     3,     3,     1,     3,     3,     1,     3,     3,
       3,     1,     2,     1,     2,     2,     2,     2,     1,     2,
       2,     1,     2,     2,     3,     1,     2,     0,     5,     0,
       4,     1,     0,     0
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


/* Context of a parse error.  */
typedef struct
{
  yy_state_t *yyssp;
  yysymbol_kind_t yytoken;
} yypcontext_t;

/* Put in YYARG at most YYARGN of the expected tokens given the
   current YYCTX, and return the number of tokens stored in YYARG.  If
   YYARG is null, return the number of expected tokens (guaranteed to
   be less than YYNTOKENS).  Return YYENOMEM on memory exhaustion.
   Return 0 if there are more than YYARGN expected tokens, yet fill
   YYARG up to YYARGN. */
static int
yypcontext_expected_tokens (const yypcontext_t *yyctx,
                            yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  int yyn = yypact[+*yyctx->yyssp];
  if (!yypact_value_is_default (yyn))
    {
      /* Start YYX at -YYN if negative to avoid negative indexes in
         YYCHECK.  In other words, skip the first -YYN actions for
         this state because they are default actions.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;
      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yyx;
      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
        if (yycheck[yyx + yyn] == yyx && yyx != YYSYMBOL_YYerror
            && !yytable_value_is_error (yytable[yyx + yyn]))
          {
            if (!yyarg)
              ++yycount;
            else if (yycount == yyargn)
              return 0;
            else
              yyarg[yycount++] = YY_CAST (yysymbol_kind_t, yyx);
          }
    }
  if (yyarg && yycount == 0 && 0 < yyargn)
    yyarg[0] = YYSYMBOL_YYEMPTY;
  return yycount;
}




#ifndef yystrlen
# if defined __GLIBC__ && defined _STRING_H
#  define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
# else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
# endif
#endif

#ifndef yystpcpy
# if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#  define yystpcpy stpcpy
# else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
# endif
#endif

#ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
      char const *yyp = yystr;
      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
#endif


static int
yy_syntax_error_arguments (const yypcontext_t *yyctx,
                           yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yyctx->yytoken != YYSYMBOL_YYEMPTY)
    {
      int yyn;
      if (yyarg)
        yyarg[yycount] = yyctx->yytoken;
      ++yycount;
      yyn = yypcontext_expected_tokens (yyctx,
                                        yyarg ? yyarg + 1 : yyarg, yyargn - 1);
      if (yyn == YYENOMEM)
        return YYENOMEM;
      else
        yycount += yyn;
    }
  return yycount;
}

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return -1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return YYENOMEM if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                const yypcontext_t *yyctx)
{
  enum { YYARGS_MAX = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  yysymbol_kind_t yyarg[YYARGS_MAX];
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* Actual size of YYARG. */
  int yycount = yy_syntax_error_arguments (yyctx, yyarg, YYARGS_MAX);
  if (yycount == YYENOMEM)
    return YYENOMEM;

  switch (yycount)
    {
#define YYCASE_(N, S)                       \
      case N:                               \
        yyformat = S;                       \
        break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
    }

  /* Compute error message size.  Don't count the "%s"s, but reserve
     room for the terminator.  */
  yysize = yystrlen (yyformat) - 2 * yycount + 1;
  {
    int yyi;
    for (yyi = 0; yyi < yycount; ++yyi)
      {
        YYPTRDIFF_T yysize1
          = yysize + yytnamerr (YY_NULLPTR, yytname[yyarg[yyi]]);
        if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
          yysize = yysize1;
        else
          return YYENOMEM;
      }
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return -1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yytname[yyarg[yyi++]]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* $@1: %empty  */
#line 112 "l24.y"
    {
        /* 在一开始设置跳转至main的block入口的指令 */
        vm_record();
        vm_gen(jmp, 0, 0);
        vm_output();

    }
#line 1695 "parser.cpp"
    break;

  case 3: /* program: MAINSYM $@1 rec_cbuf_pc LBRACE stmt_list RBRACE  */
#line 120 "l24.y"
    {
        vm_set_jmp(0, vm_code_cnt, false);
        vm_gen(ret, 0, 0);
        table_print(flog);
        /* 清空缓存的变量地址偏移 */
        tbl_id_addr[cur_level] = INI_PUSH_SIZE;
        /* 清空不需要再使用的符号表项 */
        table_clear(&tbl_tail, cur_level);
        /* 将code buffer中的一个block的内容输出到code中 */
        vm_output();
    }
#line 1711 "parser.cpp"
    break;

  case 4: /* stmt_list: %empty  */
#line 135 "l24.y"
    {

    }
#line 1719 "parser.cpp"
    break;

  case 14: /* $@2: %empty  */
#line 149 "l24.y"
                {yyerror("expect a \";\"");}
#line 1725 "parser.cpp"
    break;

  case 16: /* $@3: %empty  */
#line 150 "l24.y"
                {yyerror("expect a \";\"");}
#line 1731 "parser.cpp"
    break;

  case 18: /* $@4: %empty  */
#line 151 "l24.y"
              {yyerror("expect a \";\"");}
#line 1737 "parser.cpp"
    break;

  case 20: /* $@5: %empty  */
#line 152 "l24.y"
               {yyerror("expect a \";\"");}
#line 1743 "parser.cpp"
    break;

  case 22: /* $@6: %empty  */
#line 153 "l24.y"
                {yyerror("expect a \";\"");}
#line 1749 "parser.cpp"
    break;

  case 24: /* $@7: %empty  */
#line 154 "l24.y"
                 {yyerror("expect a \";\"");}
#line 1755 "parser.cpp"
    break;

  case 27: /* $@8: %empty  */
#line 162 "l24.y"
    {
        int tbl_idx = tbl_idx_proc[cur_level - 1];
        /* 更新表项的size */
        table[tbl_idx].size = (yyvsp[-2].arg_inf)->len;
        /* 更新表象的 proc_info */
        proc_desp* proc_info = create_proc_desp((yyvsp[-2].arg_inf), type_ret[cur_level]);
        table[tbl_idx].proc_info = proc_info;
        /* 更新表项的 pc起始位置 */
        table[tbl_idx].addr = vm_code_cnt - 1;
    }
#line 1770 "parser.cpp"
    break;

  case 28: /* proc: LPAREN load_params param_list load_params RPAREN $@8 rec_cbuf_pc LBRACE stmt_list RBRACE  */
#line 173 "l24.y"
    {
        if(num_ret[cur_level] == 0){
            yyerror("missing return statement");
        }
    }
#line 1780 "parser.cpp"
    break;

  case 29: /* $@9: %empty  */
#line 179 "l24.y"
    {
        int tbl_idx = tbl_idx_proc[cur_level - 1];
        /* 更新表项的size */
        table[tbl_idx].size = 0;
        /* 更新表象的 proc_info */
        proc_desp* proc_info = create_proc_desp(creat_args_s(), type_ret[cur_level]);
        table[tbl_idx].proc_info = proc_info;
        /* 更新表项的 pc起始位置 */
        table[tbl_idx].addr = vm_code_cnt;
    }
#line 1795 "parser.cpp"
    break;

  case 30: /* proc: LPAREN RPAREN $@9 rec_cbuf_pc LBRACE stmt_list RBRACE  */
#line 190 "l24.y"
    {
        /* 填入返回值的类型 */
        if(num_ret[cur_level] == 0){
            yyerror("missing return statement");
        }
    }
#line 1806 "parser.cpp"
    break;

  case 31: /* ret_stmt: RETSYM expr  */
#line 198 "l24.y"
    {
        if((yyvsp[0].expr_inf)){
            type_desp* type = (yyvsp[0].expr_inf)->type_info;
            if(!type_equal(type, type_ret[cur_level])){
                yyerror("different ret type");
            }
            else{
                if((yyvsp[0].expr_inf)->islval == 1){
                    vm_gen(lod, -1, 0);
                }
                vm_gen(ret, 0, (yyvsp[0].expr_inf)->type_info->t);
                num_ret[cur_level] += 1;
            }
        }
        else{
            num_ret[cur_level] += 1;
        }
    }
#line 1829 "parser.cpp"
    break;

  case 32: /* end_block: %empty  */
#line 218 "l24.y"
    {
        /* 存储当前函数体的起始位置 */
        cur_block_pc = vm_code_cnt;
        table_print(flog);
        /* 清空缓存的变量地址偏移 */
        tbl_id_addr[cur_level] = INI_PUSH_SIZE;
        /* 清空不需要再使用的符号表项 */
        table_clear(&tbl_tail, cur_level);

        /* 将code buffer中的一个block的内容输出到code中 */
        vm_output();
    }
#line 1846 "parser.cpp"
    break;

  case 33: /* type_spec: type  */
#line 233 "l24.y"
    {
        (yyval.type_inf) = create_type_desp((yyvsp[0].type_name), -1, -1);
    }
#line 1854 "parser.cpp"
    break;

  case 34: /* type_spec: type pointer  */
#line 237 "l24.y"
    {
        (yyval.type_inf) = create_type_desp(pointer, -1, (yyvsp[0].number));
        type_desp* member_type = create_type_desp((yyvsp[-1].type_name), -1, -1);
        (yyval.type_inf)->member_t = member_type;
    }
#line 1864 "parser.cpp"
    break;

  case 35: /* pointer: TIMES  */
#line 244 "l24.y"
    {
        (yyval.number) = 1;
    }
#line 1872 "parser.cpp"
    break;

  case 36: /* pointer: pointer TIMES  */
#line 248 "l24.y"
    {
        (yyval.number) = (yyvsp[-1].number) + 1;
    }
#line 1880 "parser.cpp"
    break;

  case 37: /* type: STRSYM  */
#line 253 "l24.y"
    {
        (yyval.type_name) = (yyvsp[0].type_name);
    }
#line 1888 "parser.cpp"
    break;

  case 38: /* type: INTSYM  */
#line 257 "l24.y"
    {
        (yyval.type_name) = (yyvsp[0].type_name);
    }
#line 1896 "parser.cpp"
    break;

  case 39: /* type: BOOLSYM  */
#line 261 "l24.y"
    {
        (yyval.type_name) = (yyvsp[0].type_name);
    }
#line 1904 "parser.cpp"
    break;

  case 40: /* declarator: ID  */
#line 268 "l24.y"
    {
        strcpy(tbl_id, (yyvsp[0].ident));
        (yyval.type_inf) = NULL;
    }
#line 1913 "parser.cpp"
    break;

  case 41: /* declarator: ID arr_dim  */
#line 273 "l24.y"
    {
        strcpy(tbl_id, (yyvsp[-1].ident));
        (yyval.type_inf) = (yyvsp[0].type_inf);
    }
#line 1922 "parser.cpp"
    break;

  case 42: /* decl_stmt: type_spec declarator  */
#line 279 "l24.y"
    {
        int tbl_idx = table_position(tbl_id, tbl_tail);
        type_desp* type_decl;
        if((yyvsp[0].type_inf) == NULL){
            /* 声明的变量不是数组 */
            type_decl = (yyvsp[-1].type_inf);
        }
        else{
            /* 声明的变量是一个数组 */
            type_decl = (yyvsp[0].type_inf);
            type_decl->member_t = (yyvsp[-1].type_inf);
        }
        /* 检查是否有已被声明的不同类型的同标识符 */
        if(tbl_idx != -1 && !type_equal(table[tbl_idx].type, type_decl)){
            yyerror("a declared variable");
        }
        else if(push_flag && tbl_idx != -1 && table[tbl_idx].proc_info && table[tbl_idx].proc_info->is_param){
            yyerror("a declared variable");
        }
        else{
            int size = get_type_size(type_decl);
            if(push_flag){
                vm_gen(push, 0, size);
                table_enter(type_decl, tbl_id, cur_level, size, &tbl_tail);
                // proc_desp* proc_info = create_proc_desp(NULL, NULL, true);
                // table[tbl_tail].proc_info = proc_info;
            }
            else{
                table_enter(type_decl, tbl_id, cur_level, 1, &tbl_tail);
            }
            (yyval.type_name) = type_decl->t;
        }
    }
#line 1960 "parser.cpp"
    break;

  case 43: /* $@10: %empty  */
#line 313 "l24.y"
    {
        if((yyvsp[0].type_inf) != NULL){
            /* 函数的返回值类型不应该是数组 */
            yyerror("unable to ret an array");
        }
        else{
            /* 在符号表注册，size待填，params待填, addr待填*/
            type_desp* type_decl = create_type_desp(proc, -1, -1);
            type_decl->member_t = (yyvsp[-1].type_inf);
            table_enter(type_decl, tbl_id, cur_level, 0, &tbl_tail);
            tbl_idx_proc[cur_level] = tbl_tail;
        }
    }
#line 1978 "parser.cpp"
    break;

  case 44: /* @11: %empty  */
#line 326 "l24.y"
    {
        /* 函数跳转入口 */
        vm_record();
        (yyval.number) = vm_code_cnt;
        vm_gen(jmp, 0, 0);
        vm_output();

        cur_level ++;
        /* 记录定义函数的返回值类型 */
        type_ret[cur_level] = (yyvsp[-2].type_inf);
    }
#line 1994 "parser.cpp"
    break;

  case 45: /* $@12: %empty  */
#line 338 "l24.y"
    {
        /* 记录定义函数的返回值类型 */
        type_ret[cur_level] = NULL;
        num_ret[cur_level] = 0;
        cur_level --;
    }
#line 2005 "parser.cpp"
    break;

  case 46: /* decl_stmt: type_spec declarator $@10 @11 proc end_block $@12  */
#line 344 "l24.y"
    {
        vm_set_jmp((yyvsp[-3].number), cur_block_pc, false);
        (yyval.type_name) = proc;
    }
#line 2014 "parser.cpp"
    break;

  case 47: /* arr_dim: LBRACKET LIT RBRACKET  */
#line 352 "l24.y"
    {

        if ((yyvsp[-1].lit_inf)->t != lint){
            yyerror("dimension is a non-integer");
        }
        else if((yyvsp[-1].lit_inf)->val <= 0){
            yyerror("dimension is a non-post");
        }
        else{
            (yyval.type_inf) = create_type_desp(array, (yyvsp[-1].lit_inf)->val, 1);
        }
    }
#line 2031 "parser.cpp"
    break;

  case 48: /* arr_dim: arr_dim LBRACKET LIT RBRACKET  */
#line 365 "l24.y"
    {
        if((yyval.type_inf)->dim == MAX_ARR_DIM){
            yyerror("too many dimensions");
        }
        else if ((yyvsp[-1].lit_inf)->t != lint){
            yyerror("dimension is a non-integer");
        }
        else{
            (yyval.type_inf) = (yyvsp[-3].type_inf);
            (yyval.type_inf)->shape[(yyval.type_inf)->dim] = (yyvsp[-1].lit_inf)->val;
            (yyval.type_inf)->dim += 1;
        }
    }
#line 2049 "parser.cpp"
    break;

  case 49: /* arr_idx: LBRACKET expr RBRACKET  */
#line 380 "l24.y"
    {   
        if((yyvsp[-1].expr_inf)){
            bool flag1 = err_type((yyvsp[-1].expr_inf)->type_info, lint, true, "dimension is a non-integer");
            if(flag1){
                (yyval.arr_idx_inf) = create_arr_idx_s(1, ((yyvsp[-1].expr_inf)->islval));
            }
        }
        else{
            (yyval.arr_idx_inf) = create_arr_idx_s(1, 0);
        }
    }
#line 2065 "parser.cpp"
    break;

  case 50: /* arr_idx: arr_idx LBRACKET expr RBRACKET  */
#line 392 "l24.y"
    {
        if((yyvsp[-1].expr_inf)){
            bool flag1 = err_type((yyvsp[-1].expr_inf)->type_info, lint, true, "dimension is a non-integer");
            if(flag1){
                (yyval.arr_idx_inf) = (yyvsp[-3].arr_idx_inf);
                (yyval.arr_idx_inf)->islval[(yyval.arr_idx_inf)->dim] = ((yyvsp[-1].expr_inf)->islval);
                (yyval.arr_idx_inf)->dim += 1;
            }
        }
        else{
            (yyval.arr_idx_inf)->islval[(yyval.arr_idx_inf)->dim] = 0;
            (yyval.arr_idx_inf)->dim += 1;
        }
    }
#line 2084 "parser.cpp"
    break;

  case 51: /* param_list: decl_stmt  */
#line 408 "l24.y"
    {
        type_desp* type = table[tbl_tail].type;
        if(err_type(type, proc, false)){
            /* 从符号表最后一项中获取形参类型 */
            proc_desp* proc_info = create_proc_desp(NULL, NULL, true);
            table[tbl_tail].proc_info = proc_info;
            (yyval.arg_inf) = creat_args_s();
            (yyval.arg_inf)->t_list[(yyval.arg_inf)->len] = type;
            (yyval.arg_inf)->len += 1;
        }
    }
#line 2100 "parser.cpp"
    break;

  case 52: /* param_list: param_list COMMA decl_stmt  */
#line 420 "l24.y"
    {
        type_desp* type = table[tbl_tail].type;
        if(err_type(type, proc, false)){
            proc_desp* proc_info = create_proc_desp(NULL, NULL, true);
            table[tbl_tail].proc_info = proc_info;
            (yyval.arg_inf) = (yyvsp[-2].arg_inf);
            (yyval.arg_inf)->t_list[(yyval.arg_inf)->len] = type;
            (yyval.arg_inf)->len += 1;
        }
    }
#line 2115 "parser.cpp"
    break;

  case 53: /* arg_list: expr  */
#line 432 "l24.y"
    {
        if((yyvsp[0].expr_inf)){
            /* 函数参数只使用值传递，左值需要被转化成右值
                如果左值是字符串类型，需要复制串并返回新的地址
            */
            if((yyvsp[0].expr_inf)->islval == 1){
                vm_gen(lod, -1, 0);
                if((yyvsp[0].expr_inf)->type_info->t == lstring){
                    vm_gen(snew, 0, lstring, NULL);
                }
            }
            (yyval.arg_inf) = creat_args_s();
            (yyval.arg_inf)->t_list[(yyval.arg_inf)->len] = (yyvsp[0].expr_inf)->type_info;
            (yyval.arg_inf)->len += 1;
        }
        else{
            (yyval.arg_inf) = NULL;
        }
    }
#line 2139 "parser.cpp"
    break;

  case 54: /* arg_list: arg_list COMMA expr  */
#line 452 "l24.y"
    {
        if((yyvsp[-2].arg_inf) && (yyvsp[0].expr_inf)){
            (yyval.arg_inf) = (yyvsp[-2].arg_inf);
            if((yyvsp[0].expr_inf)->islval == 1){
                vm_gen(lod, -1, 0);
                if((yyvsp[0].expr_inf)->type_info->t == lstring){
                    vm_gen(snew, 0, lstring, NULL);
                }
            }
            (yyval.arg_inf)->t_list[(yyval.arg_inf)->len] = (yyvsp[0].expr_inf)->type_info;
            (yyval.arg_inf)->len += 1;
        }
        else{
            (yyval.arg_inf) = NULL;
        }
    }
#line 2160 "parser.cpp"
    break;

  case 55: /* expr_stmt: expr BECOMES expr  */
#line 472 "l24.y"
    {
        if((yyvsp[-2].expr_inf) && (yyvsp[0].expr_inf)){
            /* 将栈顶元素赋值，要求左侧表达式为左值 */
            bool flag1 = type_equal((yyvsp[-2].expr_inf)->type_info, (yyvsp[0].expr_inf)->type_info);
            bool flag2 = (yyvsp[-2].expr_inf)->islval;
            if(!flag1){
                yyerror("assign a different type variable");
            }
            if(!flag2){
                yyerror("assign a rval variable");
            }
            if(flag1 && flag2){
                int tbl_idx = (yyvsp[-2].expr_inf)->tbl_idx;
                vm_gen(sto, -1, 0, NULL, (yyvsp[0].expr_inf)->islval, 0);
            }
        }
    }
#line 2182 "parser.cpp"
    break;

  case 56: /* expr_stmt: expr  */
#line 490 "l24.y"
    {
        if((yyvsp[0].expr_inf)){
            /* 直接弹出栈顶元素 */
            vm_gen(pop, 0, 1);
        }
    }
#line 2193 "parser.cpp"
    break;

  case 57: /* condition: expr  */
#line 501 "l24.y"
    {
        if((yyvsp[0].expr_inf)){
            (yyval.expr_inf) = (yyvsp[0].expr_inf);
            err_type((yyvsp[0].expr_inf)->type_info, lbool, true, "condition is not a bool expr");
        }
        else{
            (yyval.expr_inf) = NULL;
        }
    }
#line 2207 "parser.cpp"
    break;

  case 59: /* @13: %empty  */
#line 513 "l24.y"
    {
        (yyval.number) = vm_record(false);
        vm_gen(jmp, 0, 0);
    }
#line 2216 "parser.cpp"
    break;

  case 60: /* $@14: %empty  */
#line 518 "l24.y"
    {
        vm_set_jmp((yyvsp[-1].number), vm_record(false));
    }
#line 2224 "parser.cpp"
    break;

  case 62: /* @15: %empty  */
#line 524 "l24.y"
    {
        (yyval.number) = vm_record(false);
        vm_gen(jpc, 0, 0);
    }
#line 2233 "parser.cpp"
    break;

  case 63: /* if_part_ifthen: IFSYM LPAREN condition RPAREN THENSYM @15 LBRACE stmt_list RBRACE  */
#line 529 "l24.y"
    {
        /* 还有一条jmp指令 */ 
        vm_set_jmp((yyvsp[-3].number), vm_record(false) + 1);
    }
#line 2242 "parser.cpp"
    break;

  case 65: /* @16: %empty  */
#line 538 "l24.y"
    {
        (yyval.number) = vm_record(false);
    }
#line 2250 "parser.cpp"
    break;

  case 66: /* @17: %empty  */
#line 542 "l24.y"
    {
        (yyval.number) = vm_record(false);
        vm_gen(jpc, 0, 0);
    }
#line 2259 "parser.cpp"
    break;

  case 67: /* while_stmt: WHILESYM @16 LPAREN condition RPAREN @17 LBRACE stmt_list RBRACE  */
#line 547 "l24.y"
    {
        
        vm_gen(jmp, 0, 0);
        vm_set_jmp(vm_record(false)-1, (yyvsp[-7].number));
        vm_set_jmp((yyvsp[-3].number), vm_record(false));
    }
#line 2270 "parser.cpp"
    break;

  case 72: /* @18: %empty  */
#line 560 "l24.y"
    {
        (yyval.number) = vm_record(false);
    }
#line 2278 "parser.cpp"
    break;

  case 73: /* @19: %empty  */
#line 564 "l24.y"
    {
        (yyval.number) = vm_record(false);
        vm_gen(jpc, 0, 0);
    }
#line 2287 "parser.cpp"
    break;

  case 74: /* for_stmt: FORSYM LPAREN for_substmt SEMICOLON @18 condition SEMICOLON @19 rec_cbuf_pc for_substmt RPAREN rec_cbuf_pc LBRACE stmt_list RBRACE  */
#line 570 "l24.y"
    {
        vm_swap_block();
        vm_gen(jmp, 0, 0);
        vm_set_jmp(vm_record(false)-1, (yyvsp[-10].number));
        vm_set_jmp((yyvsp[-7].number), vm_record(false));
    }
#line 2298 "parser.cpp"
    break;

  case 75: /* scan_item: expr  */
#line 580 "l24.y"
    {
        if((yyvsp[0].expr_inf)){
            /* scan list 的项都应为左值 */
            bool flag1 = err_lval((yyvsp[0].expr_inf)->islval, "scan expr should be lval");
            bool flag2 = err_type((yyvsp[0].expr_inf)->type_info, array, false, "unable to scan array");
            if(flag1 && flag2){
                vm_gen(scn, 0, (yyvsp[0].expr_inf)->type_info->t);
                vm_gen(sto, -1, 0);
            }
        }
    }
#line 2314 "parser.cpp"
    break;

  case 76: /* print_item: expr  */
#line 592 "l24.y"
    {
        if((yyvsp[0].expr_inf)){
            if(err_type((yyvsp[0].expr_inf)->type_info, array, false, "unable to wrt array")){
                vm_gen(wrt, 0, (yyvsp[0].expr_inf)->type_info->t, NULL, (yyvsp[0].expr_inf)->islval, 0);
            }
        }
    }
#line 2326 "parser.cpp"
    break;

  case 77: /* scan_list: scan_item  */
#line 601 "l24.y"
    {}
#line 2332 "parser.cpp"
    break;

  case 78: /* scan_list: scan_list COMMA scan_item  */
#line 603 "l24.y"
    {}
#line 2338 "parser.cpp"
    break;

  case 79: /* print_list: print_item  */
#line 606 "l24.y"
    {}
#line 2344 "parser.cpp"
    break;

  case 80: /* print_list: print_list COMMA print_item  */
#line 608 "l24.y"
    {}
#line 2350 "parser.cpp"
    break;

  case 81: /* scan_stmt: SCANSYM LPAREN scan_list RPAREN  */
#line 611 "l24.y"
    {}
#line 2356 "parser.cpp"
    break;

  case 82: /* print_stmt: PRINTSYM LPAREN print_list RPAREN  */
#line 614 "l24.y"
    {}
#line 2362 "parser.cpp"
    break;

  case 83: /* expr: and_expr  */
#line 619 "l24.y"
    {
        (yyval.expr_inf) = (yyvsp[0].expr_inf);
    }
#line 2370 "parser.cpp"
    break;

  case 84: /* expr: expr OR and_expr  */
#line 623 "l24.y"
    {
        if((yyvsp[-2].expr_inf) && (yyvsp[0].expr_inf)){
            int op = opor;
            if((yyvsp[-2].expr_inf)->type_info->t != lbool || (yyvsp[0].expr_inf)->type_info->t != lbool){
                yyerror("or on unsupported type");
            }
            else{
                (yyval.expr_inf) = gen_cmp_code(op, (yyvsp[0].expr_inf), (yyvsp[-2].expr_inf));
            }
        }
        else{
            (yyval.expr_inf) = NULL;
        }
    }
#line 2389 "parser.cpp"
    break;

  case 85: /* and_expr: cmp_expr  */
#line 639 "l24.y"
    {
        (yyval.expr_inf) = (yyvsp[0].expr_inf);
    }
#line 2397 "parser.cpp"
    break;

  case 86: /* and_expr: and_expr AND cmp_expr  */
#line 643 "l24.y"
    {
        if((yyvsp[-2].expr_inf) && (yyvsp[0].expr_inf)){
            int op = opand;
            if((yyvsp[-2].expr_inf)->type_info->t != lbool || (yyvsp[0].expr_inf)->type_info->t != lbool){
                yyerror("and on unsupported type");
            }
            else{
                (yyval.expr_inf) = gen_cmp_code(op, (yyvsp[0].expr_inf), (yyvsp[-2].expr_inf));
            }
        }
        else{
            (yyval.expr_inf) = NULL;
        }
    }
#line 2416 "parser.cpp"
    break;

  case 87: /* cmp_expr: add_expr  */
#line 659 "l24.y"
    {
        (yyval.expr_inf) = (yyvsp[0].expr_inf);
    }
#line 2424 "parser.cpp"
    break;

  case 88: /* cmp_expr: cmp_expr EQ add_expr  */
#line 663 "l24.y"
    {
        if((yyvsp[-2].expr_inf) && (yyvsp[0].expr_inf)){
            int op = opeq;
            if((yyvsp[-2].expr_inf)->type_info->t == lstring) op = opseq;
            if(type_equal((yyvsp[-2].expr_inf)->type_info, (yyvsp[0].expr_inf)->type_info) && (yyvsp[-2].expr_inf)->type_info->t != array){
                (yyval.expr_inf) = gen_cmp_code(op, (yyvsp[0].expr_inf), (yyvsp[-2].expr_inf));
            }
            else{
                yyerror("cmp on unsupported types");
            }
        }
        else{
            (yyval.expr_inf) = NULL;
        }
    }
#line 2444 "parser.cpp"
    break;

  case 89: /* cmp_expr: cmp_expr NEQ add_expr  */
#line 679 "l24.y"
    {
        if((yyvsp[-2].expr_inf) && (yyvsp[0].expr_inf)){
            int op = opneq;
            if((yyvsp[-2].expr_inf)->type_info->t == lstring) op = opsneq;
            if(type_equal((yyvsp[-2].expr_inf)->type_info, (yyvsp[0].expr_inf)->type_info) && (yyvsp[-2].expr_inf)->type_info->t != array){
                (yyval.expr_inf) = gen_cmp_code(op, (yyvsp[0].expr_inf), (yyvsp[-2].expr_inf));
            }
            else{
                yyerror("cmp on unsupported types");
            }
        }
        else{
            (yyval.expr_inf) = NULL;
        }
    }
#line 2464 "parser.cpp"
    break;

  case 90: /* cmp_expr: cmp_expr LSS add_expr  */
#line 695 "l24.y"
    {
        if((yyvsp[-2].expr_inf) && (yyvsp[0].expr_inf)){
            int op = oplss;
            if(type_equal((yyvsp[-2].expr_inf)->type_info, (yyvsp[0].expr_inf)->type_info) && (yyvsp[-2].expr_inf)->type_info->t == lint){
                (yyval.expr_inf) = gen_cmp_code(op, (yyvsp[0].expr_inf), (yyvsp[-2].expr_inf));
            }
            else{
                yyerror("cmp on unsupported types");
            }
        }
        else{
            (yyval.expr_inf) = NULL;
        }
    }
#line 2483 "parser.cpp"
    break;

  case 91: /* cmp_expr: cmp_expr LEQ add_expr  */
#line 710 "l24.y"
    {
        if((yyvsp[-2].expr_inf) && (yyvsp[0].expr_inf)){
            int op = opleq;
            if(type_equal((yyvsp[-2].expr_inf)->type_info, (yyvsp[0].expr_inf)->type_info) && (yyvsp[-2].expr_inf)->type_info->t == lint){
                (yyval.expr_inf) = gen_cmp_code(op, (yyvsp[0].expr_inf), (yyvsp[-2].expr_inf));
            }
            else{
                yyerror("cmp on unsupported types");
            }
        }
        else{
            (yyval.expr_inf) = NULL;
        }
    }
#line 2502 "parser.cpp"
    break;

  case 92: /* cmp_expr: cmp_expr GTR add_expr  */
#line 725 "l24.y"
    {
        if((yyvsp[-2].expr_inf) && (yyvsp[0].expr_inf)){
            int op = opgtr;
            if(type_equal((yyvsp[-2].expr_inf)->type_info, (yyvsp[0].expr_inf)->type_info) && (yyvsp[-2].expr_inf)->type_info->t == lint){
                (yyval.expr_inf) = gen_cmp_code(op, (yyvsp[0].expr_inf), (yyvsp[-2].expr_inf));
            }
            else{
                yyerror("cmp on unsupported types");
            }
        }
        else{
            (yyval.expr_inf) = NULL;
        }
    }
#line 2521 "parser.cpp"
    break;

  case 93: /* cmp_expr: cmp_expr GEQ add_expr  */
#line 740 "l24.y"
    {
        if((yyvsp[-2].expr_inf) && (yyvsp[0].expr_inf)){
            int op = opgeq;
            if(type_equal((yyvsp[-2].expr_inf)->type_info, (yyvsp[0].expr_inf)->type_info) && (yyvsp[-2].expr_inf)->type_info->t == lint){
                (yyval.expr_inf) = gen_cmp_code(op, (yyvsp[0].expr_inf), (yyvsp[-2].expr_inf));
            }
            else{
                yyerror("cmp on unsupported types");
            }
        }
        else{
            (yyval.expr_inf) = NULL;
        }
    }
#line 2540 "parser.cpp"
    break;

  case 94: /* add_expr: multi_expr  */
#line 756 "l24.y"
    {
        (yyval.expr_inf) = (yyvsp[0].expr_inf);
    }
#line 2548 "parser.cpp"
    break;

  case 95: /* add_expr: add_expr PLUS multi_expr  */
#line 760 "l24.y"
    {
        if((yyvsp[-2].expr_inf) && (yyvsp[0].expr_inf)){
            if((yyvsp[-2].expr_inf)->type_info->t == lint && (yyvsp[0].expr_inf)->type_info->t == lint){
                vm_gen(binop, opplus, 0, NULL, (yyvsp[0].expr_inf)->islval, (yyvsp[-2].expr_inf)->islval);

                (yyval.expr_inf) = (yyvsp[-2].expr_inf);
                (yyval.expr_inf)->tbl_idx = -1;
                (yyval.expr_inf)->islval = 0;

                free((yyvsp[0].expr_inf)->type_info);
                free((yyvsp[0].expr_inf));
            }
            else if((yyvsp[-2].expr_inf)->type_info->t == lint && (yyvsp[0].expr_inf)->type_info->t == lstring){
                vm_gen(mov, opmo, 2);
                vm_gen(snew, 0, lint, NULL, (yyvsp[-2].expr_inf)->islval);
                vm_gen(binop, opscat, 0, NULL, 0, (yyvsp[0].expr_inf)->islval);
                vm_gen(mov, opmi, 1);
                
                (yyval.expr_inf) = (yyvsp[0].expr_inf);
                (yyval.expr_inf)->tbl_idx = -1;
                (yyval.expr_inf)->islval = 0;

                free((yyvsp[-2].expr_inf)->type_info);
                free((yyvsp[-2].expr_inf));
            }
            else if((yyvsp[-2].expr_inf)->type_info->t == lstring && (yyvsp[0].expr_inf)->type_info->t == lint){
                vm_gen(snew, 0, lint, NULL, (yyvsp[0].expr_inf)->islval);
                vm_gen(binop, opscat, 0, NULL, 0, (yyvsp[-2].expr_inf)->islval);

                (yyval.expr_inf) = (yyvsp[-2].expr_inf);
                (yyval.expr_inf)->tbl_idx = -1;
                (yyval.expr_inf)->islval = 0;

                free((yyvsp[0].expr_inf)->type_info);
                free((yyvsp[0].expr_inf));
            }
            else if((yyvsp[-2].expr_inf)->type_info->t == lstring && (yyvsp[0].expr_inf)->type_info->t == lstring){
                vm_gen(binop, opscat, 0, NULL, (yyvsp[0].expr_inf)->islval, (yyvsp[-2].expr_inf)->islval);

                (yyval.expr_inf) = (yyvsp[-2].expr_inf);
                (yyval.expr_inf)->tbl_idx = -1;
                (yyval.expr_inf)->islval = 0;

                free((yyvsp[0].expr_inf)->type_info);
                free((yyvsp[0].expr_inf));
            }
            else{
                yyerror("plus on unsupported types");
            }
        }
        else{
            (yyval.expr_inf) = NULL;
        }
    }
#line 2607 "parser.cpp"
    break;

  case 96: /* add_expr: add_expr MINUS multi_expr  */
#line 815 "l24.y"
    {
        if((yyvsp[-2].expr_inf) && (yyvsp[0].expr_inf)){
            if((yyvsp[-2].expr_inf)->type_info->t == lint && (yyvsp[0].expr_inf)->type_info->t == lint){
                vm_gen(binop, opminus, 0, NULL, (yyvsp[0].expr_inf)->islval, (yyvsp[-2].expr_inf)->islval);

                (yyval.expr_inf) = (yyvsp[-2].expr_inf);
                (yyval.expr_inf)->tbl_idx = -1;
                (yyval.expr_inf)->islval = 0;

                free((yyvsp[0].expr_inf)->type_info);
                free((yyvsp[0].expr_inf));
            }
            else{
                yyerror("minus on unsupported types");
            }
        }
        else{
            (yyval.expr_inf) = NULL;
        }
    }
#line 2632 "parser.cpp"
    break;

  case 97: /* multi_expr: unary_expr  */
#line 836 "l24.y"
    {
        (yyval.expr_inf) = (yyvsp[0].expr_inf);
    }
#line 2640 "parser.cpp"
    break;

  case 98: /* multi_expr: multi_expr TIMES unary_expr  */
#line 840 "l24.y"
    {
        if((yyvsp[-2].expr_inf) && (yyvsp[0].expr_inf)){
            if((yyvsp[-2].expr_inf)->type_info->t == lint && (yyvsp[0].expr_inf)->type_info->t == lint){
                vm_gen(binop, optimes, 0, NULL, (yyvsp[0].expr_inf)->islval, (yyvsp[-2].expr_inf)->islval);

                (yyval.expr_inf) = (yyvsp[-2].expr_inf);
                (yyval.expr_inf)->tbl_idx = -1;
                (yyval.expr_inf)->islval = 0;

                free((yyvsp[0].expr_inf)->type_info);
                free((yyvsp[0].expr_inf));
            }
            else if((yyvsp[-2].expr_inf)->type_info->t == lstring && (yyvsp[0].expr_inf)->type_info->t == lint){
                vm_gen(binop, opspow, 0, NULL, (yyvsp[0].expr_inf)->islval, (yyvsp[-2].expr_inf)->islval);

                (yyval.expr_inf) = (yyvsp[-2].expr_inf);
                (yyval.expr_inf)->tbl_idx = -1;
                (yyval.expr_inf)->islval = 0;

                free((yyvsp[0].expr_inf)->type_info);
                free((yyvsp[0].expr_inf));
            }
            else if((yyvsp[-2].expr_inf)->type_info->t == lint && (yyvsp[0].expr_inf)->type_info->t == lstring){
                vm_gen(mov, opmo, 2);
                vm_gen(binop, opspow, 0, NULL, (yyvsp[-2].expr_inf)->islval, (yyvsp[0].expr_inf)->islval);
                vm_gen(mov, opmi, 1);

                (yyval.expr_inf) = (yyvsp[0].expr_inf);
                (yyval.expr_inf)->tbl_idx = -1;
                (yyval.expr_inf)->islval = 0;

                free((yyvsp[-2].expr_inf)->type_info);
                free((yyvsp[-2].expr_inf));
            }
            else{
                yyerror("times on unsupported types");
            }
        }
        else{
            (yyval.expr_inf) = NULL;
        }
    }
#line 2687 "parser.cpp"
    break;

  case 99: /* multi_expr: multi_expr SLASH unary_expr  */
#line 883 "l24.y"
    {
        if((yyvsp[-2].expr_inf) && (yyvsp[0].expr_inf)){
            if((yyvsp[-2].expr_inf)->type_info->t == lint && (yyvsp[0].expr_inf)->type_info->t == lint){
                vm_gen(binop, opslash, 0, NULL, (yyvsp[0].expr_inf)->islval, (yyvsp[-2].expr_inf)->islval);

                (yyval.expr_inf) = (yyvsp[-2].expr_inf);
                (yyval.expr_inf)->tbl_idx = -1;
                (yyval.expr_inf)->islval = 0;

                free((yyvsp[0].expr_inf)->type_info);
                free((yyvsp[0].expr_inf));
            }
            else{
                yyerror("slash on unsupported types");
            }
        }
        else{
            (yyval.expr_inf) = NULL;
        }
    }
#line 2712 "parser.cpp"
    break;

  case 100: /* multi_expr: multi_expr MOD unary_expr  */
#line 904 "l24.y"
    {
        if((yyvsp[-2].expr_inf) && (yyvsp[0].expr_inf)){
            if((yyvsp[-2].expr_inf)->type_info->t == lint && (yyvsp[0].expr_inf)->type_info->t == lint){
                vm_gen(binop, opmod, 0, NULL, (yyvsp[0].expr_inf)->islval, (yyvsp[-2].expr_inf)->islval);

                (yyval.expr_inf) = (yyvsp[-2].expr_inf);
                (yyval.expr_inf)->tbl_idx = -1;
                (yyval.expr_inf)->islval = 0;

                free((yyvsp[0].expr_inf)->type_info);
                free((yyvsp[0].expr_inf));
            }
            else{
                yyerror("mod on unsupported types");
            }
        }
        else{
            (yyval.expr_inf) = NULL;
        }
    }
#line 2737 "parser.cpp"
    break;

  case 101: /* unary_expr: pre_expr  */
#line 926 "l24.y"
    {
        (yyval.expr_inf) = (yyvsp[0].expr_inf);
    }
#line 2745 "parser.cpp"
    break;

  case 102: /* unary_expr: NOT unary_expr  */
#line 930 "l24.y"
    {
        if((yyvsp[0].expr_inf)){
            bool flag1 = err_type((yyvsp[0].expr_inf)->type_info, lbool, true, "not a non-bool");
            if(flag1){
                vm_gen(unaop, opnot, 0, NULL, (yyvsp[0].expr_inf)->islval, 0);
                (yyval.expr_inf) = (yyvsp[0].expr_inf);
                (yyval.expr_inf)->tbl_idx = -1;
                (yyval.expr_inf)->islval = 0;
            }
        }
        else{
            (yyval.expr_inf) = NULL;
        }
    }
#line 2764 "parser.cpp"
    break;

  case 103: /* pre_expr: post_expr  */
#line 947 "l24.y"
    {
        (yyval.expr_inf) = (yyvsp[0].expr_inf);
    }
#line 2772 "parser.cpp"
    break;

  case 104: /* pre_expr: INC post_expr  */
#line 951 "l24.y"
    {
        if((yyvsp[0].expr_inf)){
            bool flag1 = err_type((yyvsp[0].expr_inf)->type_info, lint, true, "inc a non-integer");
            bool flag2 = err_lval((yyvsp[0].expr_inf)->islval);
            if(flag1 && flag2){
                vm_gen(unaop, opinc, 0, NULL, 1, 0);
                (yyval.expr_inf) = (yyvsp[0].expr_inf);
            }
        }
        else{
            (yyval.expr_inf) = NULL;
        }
        
    }
#line 2791 "parser.cpp"
    break;

  case 105: /* pre_expr: DEC post_expr  */
#line 966 "l24.y"
    {
        if((yyvsp[0].expr_inf)){
            bool flag1 = err_type((yyvsp[0].expr_inf)->type_info, lint, true, "dec a non-integer");
            bool flag2 = err_lval((yyvsp[0].expr_inf)->islval);
            if(flag1 && flag2){
                vm_gen(unaop, opdec, 0, NULL, 1, 0);
                (yyval.expr_inf) = (yyvsp[0].expr_inf);
            }
        }
        else{
            (yyval.expr_inf) = NULL;
        }
    }
#line 2809 "parser.cpp"
    break;

  case 106: /* pre_expr: PLUS post_expr  */
#line 980 "l24.y"
    {
        if((yyvsp[0].expr_inf)){
            bool flag1 = err_type((yyvsp[0].expr_inf)->type_info, lint, true);
            if(flag1){
                /* 不做任何运算，但结果转为右值 */
                if((yyvsp[0].expr_inf)->islval){
                    vm_gen(lod, -1, 0);
                }
                (yyval.expr_inf) = (yyvsp[0].expr_inf);
                (yyval.expr_inf)->tbl_idx = -1;
                (yyval.expr_inf)->islval = 0;
            }
        }
        else{
            (yyval.expr_inf) = NULL;
        }
    }
#line 2831 "parser.cpp"
    break;

  case 107: /* pre_expr: MINUS post_expr  */
#line 998 "l24.y"
    {
        if((yyvsp[0].expr_inf)){
            bool flag1 = err_type((yyvsp[0].expr_inf)->type_info, lint, true);
            if(flag1){
                vm_gen(lit, 0, -1);
                vm_gen(binop, optimes, 0, NULL, 0, (yyvsp[0].expr_inf)->islval);
                (yyval.expr_inf) = (yyvsp[0].expr_inf);
                (yyval.expr_inf)->tbl_idx = -1;
                (yyval.expr_inf)->islval = 0;
            }
        }
        else{
            (yyval.expr_inf) = NULL;
        }
    }
#line 2851 "parser.cpp"
    break;

  case 108: /* post_expr: term  */
#line 1016 "l24.y"
    {
        (yyval.expr_inf) = (yyvsp[0].expr_inf);
    }
#line 2859 "parser.cpp"
    break;

  case 109: /* post_expr: post_expr INC  */
#line 1020 "l24.y"
    {
        if((yyvsp[-1].expr_inf)){
            bool flag1 = err_type((yyvsp[-1].expr_inf)->type_info, lint, true, "inc a non-integer");
            bool flag2 = err_lval((yyvsp[-1].expr_inf)->islval);
            if(flag1 && flag2){
                vm_gen(unaop, opinc, 0, NULL, 1, 0);
                vm_gen(lit, 0, 1);
                vm_gen(binop, opminus, 0, NULL, 0, 1);/* 执行二元操作时，压入栈的永远是右值 */
                (yyval.expr_inf) = (yyvsp[-1].expr_inf);
                (yyval.expr_inf)->tbl_idx = -1;
                (yyval.expr_inf)->islval = 0;
            }        
        }
        else{
            (yyval.expr_inf) = NULL;
        }
    }
#line 2881 "parser.cpp"
    break;

  case 110: /* post_expr: post_expr DEC  */
#line 1038 "l24.y"
    {
        if((yyvsp[-1].expr_inf)){
            bool flag1 = err_type((yyvsp[-1].expr_inf)->type_info, lint, true, "dec a non-integer");
            bool flag2 = err_lval((yyvsp[-1].expr_inf)->islval);
            if(flag1 && flag2){
                vm_gen(unaop, opdec, 0, NULL, 1, 0);
                vm_gen(lit, 0, 1);
                vm_gen(binop, opplus, 0, NULL, 0, 1);
                (yyval.expr_inf) = (yyvsp[-1].expr_inf);
                (yyval.expr_inf)->tbl_idx = -1;
                (yyval.expr_inf)->islval = 0;
        }  
        }
        else{
            (yyval.expr_inf) = NULL;
        }
        
    }
#line 2904 "parser.cpp"
    break;

  case 111: /* term: factor  */
#line 1058 "l24.y"
    {
        (yyval.expr_inf) = (yyvsp[0].expr_inf);
    }
#line 2912 "parser.cpp"
    break;

  case 112: /* term: ADDR term  */
#line 1062 "l24.y"
    {
        if((yyvsp[0].expr_inf)){
            /* 取地址符只能处理左值类型 */
            /* 将栈顶元素作为一个右值 */
            if(err_lval((yyvsp[0].expr_inf)->islval, "addr a rval variable")){
                (yyval.expr_inf) = (yyvsp[0].expr_inf);
                /* 表达式是一个指针类型，需要修改member_t字段 */
                type_desp* type_term = create_type_desp(pointer, -1, 1);

                /* 对数组取地址：获取数组首元素的地址 */
                if((yyval.expr_inf)->type_info->t == array){
                    type_term->member_t = (yyval.expr_inf)->type_info->member_t;
                    free((yyval.expr_inf)->type_info);
                    (yyval.expr_inf)->type_info = type_term;
                }
                /* 对指针取地址：增加指针的维度 */
                else if((yyval.expr_inf)->type_info->t == pointer){
                    type_term->dim += (yyval.expr_inf)->type_info->dim;
                    type_term->member_t = (yyval.expr_inf)->type_info->member_t;
                    free((yyval.expr_inf)->type_info);
                    (yyval.expr_inf)->type_info = type_term;
                }
                /* 对普通类型取地址 */
                else{
                    type_term->member_t = (yyval.expr_inf)->type_info;
                    (yyval.expr_inf)->type_info = type_term;
                }
                (yyval.expr_inf)->tbl_idx = -1;
                (yyval.expr_inf)->islval = 0;
            }
        }
        else{
            (yyval.expr_inf) = NULL;
        }
    }
#line 2952 "parser.cpp"
    break;

  case 113: /* term: TIMES term  */
#line 1098 "l24.y"
    {
        if((yyvsp[0].expr_inf)){
            /* 如果栈顶是左值, 加载值至栈顶 */
            if((yyvsp[0].expr_inf)->islval == 1){
                vm_gen(lod, -1, 0);
            }

            /* 
                对指针执行一层解引用
            */
            if(err_type((yyvsp[0].expr_inf)->type_info, pointer, true, "derefer a non-pointer")){
                type_desp* type_term;
                (yyval.expr_inf) = (yyvsp[0].expr_inf);
                (yyval.expr_inf)->islval = 1;
                (yyval.expr_inf)->type_info->dim -= 1;
                if((yyval.expr_inf)->type_info->dim == 0){
                    type_term = (yyval.expr_inf)->type_info->member_t;
                    free((yyval.expr_inf)->type_info);
                    (yyval.expr_inf)->type_info = type_term;
                }
            }
        }
        else{
            (yyval.expr_inf) = NULL;
        }
    }
#line 2983 "parser.cpp"
    break;

  case 114: /* factor: LPAREN expr RPAREN  */
#line 1126 "l24.y"
    {
        (yyval.expr_inf) = (yyvsp[-1].expr_inf);
    }
#line 2991 "parser.cpp"
    break;

  case 115: /* factor: ID  */
#line 1130 "l24.y"
    {
        int tbl_idx = table_position((yyvsp[0].ident), tbl_tail);
        if(err_ident(tbl_idx)){
            type_desp* type_factor;
            vm_gen(lit, 0, table[tbl_idx].addr);
            vm_gen(addr, cur_level - table[tbl_idx].level, 0);
            /* 如果是数组，表达式的值为数组首地址，此时表达式为右值 */
            if(table[tbl_idx].type->t == array){
                type_factor = create_type_desp(pointer, -1, 1);
                type_factor->member_t = type_copy(table[tbl_idx].type->member_t);

                (yyval.expr_inf) = create_expr_s(type_factor, -1);
            }
            else{
                if(err_type(table[tbl_idx].type, proc, false)){
                    type_factor = type_copy(table[tbl_idx].type);
                    (yyval.expr_inf) = create_expr_s(type_factor, tbl_idx);
                }
            }
        }
        else{
            (yyval.expr_inf) = NULL;
        }
    }
#line 3020 "parser.cpp"
    break;

  case 116: /* factor: ID arr_idx  */
#line 1155 "l24.y"
    {
        int tbl_idx = table_position((yyvsp[-1].ident), tbl_tail);
        if(err_ident(tbl_idx)){
            type_desp* var_type_info = table[tbl_idx].type;
            /* 类型检查和维度检查 */
            bool flag_arr = var_type_info->t == array;
            bool flag_ptr = var_type_info->t == pointer;

            if(flag_arr && err_dim((yyvsp[0].arr_idx_inf)->dim, var_type_info->dim)){
                /* 索引计算 */
                vm_gen(mov, opmo, (yyvsp[0].arr_idx_inf)->dim);
                if((yyvsp[0].arr_idx_inf)->dim == 1 && (yyvsp[0].arr_idx_inf)->islval[0]){
                    vm_gen(lod, -1, 0);
                }

                for(int i = 1; i < (yyvsp[0].arr_idx_inf)->dim; i++){
                    vm_gen(lit, 0, var_type_info->shape[i]);
                    vm_gen(binop, optimes, 0, NULL, 0, (yyvsp[0].arr_idx_inf)->islval[i-1]);
                    vm_gen(binop, opplus, 0, NULL, 0, (yyvsp[0].arr_idx_inf)->islval[i]);
                }
                vm_gen(mov, opmi, 1);

                vm_gen(lit, 0, table[tbl_idx].addr);
                vm_gen(addr, cur_level - table[tbl_idx].level, 0);
                /* 如果ID是数组类型的形参，实参是指针，要通过指针获取值 */
                if(table[tbl_idx].proc_info && table[tbl_idx].proc_info->is_param){
                    vm_gen(lod, -1, 0);
                }
                vm_gen(binop, opplus, 0, NULL, 0, 0);

                /* 判断表达式的类型，可能是数组项或其地址 */
                int t = var_type_info->t, member_t = var_type_info->member_t->t;
                type_desp* type_factor;
                if(var_type_info->dim == (yyvsp[0].arr_idx_inf)->dim){
                    type_factor = type_copy(var_type_info->member_t);
                    (yyval.expr_inf) = create_expr_s(type_factor, tbl_idx);
                }
                else{
                    /* 如果是地址，转换成指针类型 */
                    type_factor = create_type_desp(pointer, -1, 1);
                    type_factor->member_t = type_copy(var_type_info->member_t);
                    (yyval.expr_inf) = create_expr_s(type_factor, -1);
                }
                
            }
            else if(flag_ptr && err_dim((yyvsp[0].arr_idx_inf)->dim, 1)){
                /* 索引计算 */
                vm_gen(lit, 0, table[tbl_idx].addr);
                vm_gen(addr, cur_level - table[tbl_idx].level, 0);
                vm_gen(lod, -1, 0);
                vm_gen(binop, opplus, 0, NULL, 1, (yyvsp[0].arr_idx_inf)->islval[0]);
                vm_gen(addr, cur_level - table[tbl_idx].level, 0);

                type_desp* type_factor;
                if(table[tbl_idx].type->dim == 1){
                    type_factor = type_copy(table[tbl_idx].type->member_t);
                }
                else{
                    type_factor = type_copy(table[tbl_idx].type);
                    type_factor->dim -= 1;
                }
                (yyval.expr_inf) = create_expr_s(type_factor, tbl_idx);
            }
            else{
                yyerror("index a non-array");
            }
        }
        else{
            (yyval.expr_inf) = NULL;
        }

    }
#line 3097 "parser.cpp"
    break;

  case 117: /* $@20: %empty  */
#line 1228 "l24.y"
    {
        /* 压入当前堆尾地址 */
        vm_gen(reg, 0, 0);
    }
#line 3106 "parser.cpp"
    break;

  case 118: /* factor: ID LPAREN $@20 arg_list RPAREN  */
#line 1233 "l24.y"
    {
        /* 获取函数标识符表项 */
        int tbl_idx = table_position((yyvsp[-4].ident), tbl_tail);
        if(err_ident(tbl_idx)){
            bool flag1 = err_type(table[tbl_idx].type, proc);
            bool flag2 = flag1 && err_args(tbl_idx, (yyvsp[-1].arg_inf));
            if(flag2){
            
            /* 参数被压入辅助栈 */
            vm_gen(mov, opmo, table[tbl_idx].size + 1);

            /* cal指令计算RA，DL，SL，压入辅助栈后将所有元素弹回主栈 */
            vm_gen(cal, cur_level - table[tbl_idx].level, table[tbl_idx].addr);

            /* 语义计算结果为返回值类型，右值 */
            type_desp* type_factor = type_copy(table[tbl_idx].proc_info->ret_type);
            (yyval.expr_inf) = create_expr_s(type_factor, -1);
            }
        }
        else{
            (yyval.expr_inf) = NULL;
        }
    }
#line 3134 "parser.cpp"
    break;

  case 119: /* $@21: %empty  */
#line 1257 "l24.y"
    {
        /* 压入当前堆尾地址 */
        vm_gen(reg, 0, 0);
    }
#line 3143 "parser.cpp"
    break;

  case 120: /* factor: ID LPAREN $@21 RPAREN  */
#line 1262 "l24.y"
    {
        /* 获取函数标识符表项 */
        int tbl_idx = table_position((yyvsp[-3].ident), tbl_tail);
        if(err_ident(tbl_idx)){
            bool flag1 = err_type(table[tbl_idx].type, proc);
            if(flag1){
                /* 参数被压入辅助栈 */
                vm_gen(mov, opmo, table[tbl_idx].size + 1);

                /* cal指令计算RA，DL，SL，压入辅助栈后将所有元素弹回主栈 */
                vm_gen(cal, cur_level - table[tbl_idx].level, table[tbl_idx].addr);

                /* 语义计算结果为返回值类型，右值 */
                type_desp* type_factor = type_copy(table[tbl_idx].proc_info->ret_type);
                (yyval.expr_inf) = create_expr_s(type_factor, -1);
            }
        }
        else{
            (yyval.expr_inf) = NULL;
        }
    }
#line 3169 "parser.cpp"
    break;

  case 121: /* factor: LIT  */
#line 1284 "l24.y"
    {
        if((yyvsp[0].lit_inf)){
            type_desp* type_factor = create_type_desp((yyvsp[0].lit_inf)->t, -1, -1);
            if((yyvsp[0].lit_inf)->t == lstring){
                /* 处理字符串类型 */
                vm_gen(snew, 0, lstring, (yyvsp[0].lit_inf)->str);
            }
            else{
                /* 处理布尔/整型类型 */
                vm_gen(lit, 0, (yyvsp[0].lit_inf)->val, NULL);
            }
            (yyval.expr_inf) = create_expr_s(type_factor, -1);
        }
        else{
            (yyval.expr_inf) = NULL;
        }
    }
#line 3191 "parser.cpp"
    break;

  case 122: /* rec_cbuf_pc: %empty  */
#line 1302 "l24.y"
    {
        /* 更新 block_pc_stack*/
        (yyval.number) = vm_record(); 
    }
#line 3200 "parser.cpp"
    break;

  case 123: /* load_params: %empty  */
#line 1307 "l24.y"
    {
        push_flag = !push_flag;
    }
#line 3208 "parser.cpp"
    break;


#line 3212 "parser.cpp"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      {
        yypcontext_t yyctx
          = {yyssp, yytoken};
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == -1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *,
                             YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (yymsg)
              {
                yysyntax_error_status
                  = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
                yymsgp = yymsg;
              }
            else
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = YYENOMEM;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == YYENOMEM)
          YYNOMEM;
      }
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
  return yyresult;
}

#line 1314 "l24.y"

expr_s* gen_cmp_code(int op, expr_s* s1, expr_s* s3){
    vm_gen(binop, op, 0, NULL, s1->islval, s3->islval);

    type_desp* type_cmp = create_type_desp(lbool, -1, -1); 
    expr_s* ret = create_expr_s(type_cmp, -1);

    free(s1->type_info);
    free(s1);
    free(s3->type_info);
    free(s3);
    return ret;
}

void yyerror(const char *msg) {
	err = err + 1;
    fprintf(flog, "\nError: %s in line %d\n", msg, err_line);
    err_line = line;
	return;
}

bool err_ident(int tbl_idx, const char* rep){
    if(tbl_idx == -1){
        if(rep) yyerror(rep);
        else yyerror("a variable undeclared");
        return false;
    }
    else return true;
}

bool err_lval(int islval, const char* rep){
    if(islval == 0){
        if(rep) yyerror(rep);
        else yyerror("expr is a rval");
        return false;
    }
    else return true;
}

bool err_type(type_desp* expr_type, int t, bool eq, const char* rep){
    if(eq){
        if(expr_type->t != t){
            if(rep) yyerror(rep);
            else yyerror("type of variable mismatch");
            return false;
        }
        else return true;
    }
    else{
        if(expr_type->t == t){
            if(rep) yyerror(rep);
            else yyerror("type of variable mismatch");
            return false;
        }
        else return true;
    }
    
}
bool err_dim(int expr_dim, int decl_dim, const char* rep){
    if(expr_dim > decl_dim){
        if(rep) yyerror(rep);
        else yyerror("an array with too many dimensions");
        return false;
    }
    return true;
}

bool err_args(int proc_tbl_idx, args_s* args, const char* rep){
    args_s* param_info = table[proc_tbl_idx].proc_info->param_info;
    int param_len = param_info->len;
    /* 检查形参与实参是否类型匹配 */
    if(param_len == args->len){
        int m_args = 0;
        while(m_args < param_len){
            if(type_equal(param_info->t_list[m_args], args->t_list[m_args])){
                m_args++;
            }
            else if(param_info->t_list[m_args]->t == array
                    && args->t_list[m_args]->t == pointer
                    && type_equal(param_info->t_list[m_args]->member_t, args->t_list[m_args]->member_t)){
                m_args++;
            }
            else{
                break;
            }
        }
        if(m_args != param_len){
            if(rep) yyerror(rep);
            else yyerror("args don't match");
            return false;
        }
    }
    else if(param_len < args->len){
        yyerror("too many args");
        return false;
    }
    else{
        yyerror("too few args");
        return false;
    }
    return true;
}

void redirectInput(FILE *input, FILE* output) {
	yyin=input;
    if(output) yyout = foutput;
}

void init() {
    err = 0;
    cur_level = 0;
    push_flag = true;
    for(int i = 0; i < MAX_NEST_DEPTH; i++){
        num_ret[i] = 0;
        type_ret[i] = NULL;
        tbl_idx_proc[i] = -1;
    }
    cur_block_pc = 0; /* 存储当前函数体的起始位置 */
    table_init();
    vm_init();
}

int main(int argc,char **argv) {
    const char* fname = argv[1];

    if ((flog = fopen("flog.txt", "w")) == NULL){
        printf("Can't open flog.txt file!\n");
        exit(1);
    }

    if ((fcode = fopen("fcode.txt", "w")) == NULL){
        printf("Can't open fcode.txt file!\n");
        exit(1);
    }
    if ((fsource = fopen(fname, "r")) == NULL){
		printf("Can't open the source file!\n");
		exit(1);
	}

    if(argc > 2 && strcmp(argv[2], "-g") == 0){
        if ((finput = fopen("finput.txt", "r")) == NULL){
            printf("Can't open finput.txt file!\n");
            exit(1);
        }
    }
    else{
        finput = stdin;
        if ((finfo = fopen("finfo.txt", "w")) == NULL){
            printf("Can't open finfo.txt file!\n");
            exit(1);
        }
        if ((foutput = fopen("foutput.txt", "w")) == NULL){
            printf("Can't open foutput.txt file!\n");
            exit(1);
        }
    }

    redirectInput(fsource, foutput);	
	init();

    fprintf(flog, "===compiling...===\n");
    yyparse();

	if(err == 0)
	{
		fprintf(flog, "\n===success!===\n");
        vm_print_code(fcode);
	}
    else
	{
		fprintf(flog, "%d errors in PL/0 program\n", err);
	}
    fclose(flog);
    fclose(fcode);
    fclose(fsource);

    int cmd;
    if(argc > 2 && strcmp(argv[2], "-g") == 0){
        int cmd_cnt = 0;
        char cache_file[50];
        FILE* cache;

        sprintf(cache_file, "./cache/%d.txt", cmd_cnt);
        if((cache = fopen(cache_file, "w")) == NULL){
            printf("Can't open cache file!\n");
            exit(1);
        }
        cmd_cnt+=1;
        fclose(cache);
        
        while(scanf("%c", &cmd) > 0){
            if(cmd == 'q'){ break; }
            else if(cmd == 'e'){
                sprintf(cache_file, "./cache/%d.txt", cmd_cnt);
                if((cache = fopen(cache_file, "w")) == NULL){
                    printf("Can't open cache file!\n");
                    exit(1);
                }
                vm_execute(finput, cache);
                fclose(cache);
            }
            else if(cmd == 's'){
                sprintf(cache_file, "./cache/%d.txt", cmd_cnt);
                if((cache = fopen(cache_file, "w")) == NULL){
                    printf("Can't open cache file!\n");
                    exit(1);
                }
                if(vm_pc < vm_code_cnt) {
                    vm_step(finput, cache);
                    fclose(cache);
                }
                else continue;
            }
            else if (cmd >= '0' && cmd <= '9'){
                sprintf(cache_file, "./cache/%d.txt", cmd_cnt);
                if((cache = fopen(cache_file, "w")) == NULL){
                    printf("Can't open cache file!\n");
                    exit(1);
                }
                vm_print_storage(cmd, cache);
                fclose(cache);
            }
            else{}
            cmd_cnt += 1;
        }
    }
    else{
        while(scanf("%c", &cmd) > 0){
            if(cmd == 'q'){ break; }
            else if(cmd == 'e'){
                vm_execute(finput, foutput);
            }
            else if(cmd == 's'){
                if(vm_pc < vm_code_cnt) vm_step(finput, foutput);
                else continue;
            }
            else{
                vm_print_storage(cmd, finfo);
            }
        }
        fclose(foutput);
        fclose(finfo);
    }


	
    return 0;
}
