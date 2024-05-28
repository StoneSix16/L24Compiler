////////////////////////////////////////////////////////
//声明部分
%{
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
%}

////////////////////////////////////////////////////////
//辅助定义部分

%define parse.error verbose
%union{
char *ident;
// int var_size;
int type_name;
int number;

type_desp* type_inf;
arr_idx_s* arr_idx_inf;
literal_s* lit_inf;
expr_s* expr_inf;
args_s* arg_inf;
}


%token MAINSYM IFSYM ELSESYM THENSYM ENDSYM FORSYM WHILESYM PRINTSYM SCANSYM RETSYM
%token LBRACE RBRACE LBRACKET RBRACKET SEMICOLON COMMA LPAREN RPAREN 

%left OR
%left AND
%left LSS LEQ GTR GEQ EQ NEQ
%left PLUS MINUS
%left TIMES SLASH MOD
%token INC DEC 
%right NOT ADDR
%left BECOMES

%token <type_name> INTSYM STRSYM BOOLSYM
%token <ident> ID
%token <lit_inf> LIT

%type <number> pointer rec_cbuf_pc
%type <type_name> type decl_stmt
%type <type_inf> type_spec arr_dim declarator ret_stmt
%type <expr_inf> condition expr and_expr cmp_expr add_expr multi_expr unary_expr post_expr pre_expr term factor
%type <arg_inf> arg_list scan_list param_list
%type <arr_idx_inf> arr_idx
/* %type <var_size> stmt_list */
%start program

////////////////////////////////////////////////////////
//规则部分
%%
/* 程序 */
program: MAINSYM 
    {
        /* 在一开始设置跳转至main的block入口的指令 */
        vm_record();
        vm_gen(jmp, 0, 0);
        vm_output();

    }
    rec_cbuf_pc LBRACE stmt_list RBRACE
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
    ;

/* 语句块 */
stmt_list: 
    {

    }
    | stmt_list stmt ;

/* 语句 */
stmt: decl_stmt SEMICOLON
    | expr_stmt SEMICOLON
    | if_stmt SEMICOLON
    | while_stmt
    | for_stmt
    | ret_stmt SEMICOLON
    | scan_stmt SEMICOLON
    | print_stmt SEMICOLON
    | decl_stmt {yyerror("expect a \";\"");} error  SEMICOLON 
    | expr_stmt {yyerror("expect a \";\"");} error SEMICOLON 
    | if_stmt {yyerror("expect a \";\"");} error SEMICOLON 
    | ret_stmt {yyerror("expect a \";\"");} error SEMICOLON 
    | scan_stmt {yyerror("expect a \";\"");} error SEMICOLON 
    | print_stmt {yyerror("expect a \";\"");} error SEMICOLON 
    | SEMICOLON
    ;

/* 过程
   过程的返回值类型放在参数列表类型的最后一位，过程只能返回右值
 */
proc: LPAREN load_params param_list load_params RPAREN
    {
        int tbl_idx = tbl_idx_proc[cur_level - 1];
        /* 更新表项的size */
        table[tbl_idx].size = $3->len;
        /* 更新表象的 proc_info */
        proc_desp* proc_info = create_proc_desp($3, type_ret[cur_level]);
        table[tbl_idx].proc_info = proc_info;
        /* 更新表项的 pc起始位置 */
        table[tbl_idx].addr = vm_code_cnt - 1;
    }
    rec_cbuf_pc LBRACE stmt_list RBRACE
    {
        if(num_ret[cur_level] == 0){
            yyerror("missing return statement");
        }
    }
    | LPAREN RPAREN 
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
    rec_cbuf_pc LBRACE stmt_list RBRACE
    {
        /* 填入返回值的类型 */
        if(num_ret[cur_level] == 0){
            yyerror("missing return statement");
        }
    }
    ;
ret_stmt: RETSYM expr 
    {
        if($2){
            type_desp* type = $2->type_info;
            if(!type_equal(type, type_ret[cur_level])){
                yyerror("different ret type");
            }
            else{
                if($2->islval == 1){
                    vm_gen(lod, -1, 0);
                }
                vm_gen(ret, 0, $2->type_info->t);
                num_ret[cur_level] += 1;
            }
        }
        else{
            num_ret[cur_level] += 1;
        }
    }
    ;
end_block:  
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
    };

/* 类型限定符 */
type_spec: type
    {
        $$ = create_type_desp($1, -1, -1);
    }
    | type pointer
    {
        $$ = create_type_desp(pointer, -1, $2);
        type_desp* member_type = create_type_desp($1, -1, -1);
        $$->member_t = member_type;
    }
    ;
pointer: TIMES
    {
        $$ = 1;
    }
    | pointer TIMES
    {
        $$ = $1 + 1;
    }
    ;
type: STRSYM
    {
        $$ = $1;
    }
    | INTSYM
    {
        $$ = $1;
    }
    | BOOLSYM
    {
        $$ = $1;
    }
    ;

/* 声明 */
declarator: ID
    {
        strcpy(tbl_id, $1);
        $$ = NULL;
    }
    | ID arr_dim
    {
        strcpy(tbl_id, $1);
        $$ = $2;
    }
    ;
decl_stmt: type_spec declarator
    {
        int tbl_idx = table_position(tbl_id, tbl_tail);
        type_desp* type_decl;
        if($2 == NULL){
            /* 声明的变量不是数组 */
            type_decl = $1;
        }
        else{
            /* 声明的变量是一个数组 */
            type_decl = $2;
            type_decl->member_t = $1;
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
            $$ = type_decl->t;
        }
    }
    | type_spec declarator 
    {
        if($2 != NULL){
            /* 函数的返回值类型不应该是数组 */
            yyerror("unable to ret an array");
        }
        else{
            /* 在符号表注册，size待填，params待填, addr待填*/
            type_desp* type_decl = create_type_desp(proc, -1, -1);
            type_decl->member_t = $1;
            table_enter(type_decl, tbl_id, cur_level, 0, &tbl_tail);
            tbl_idx_proc[cur_level] = tbl_tail;
        }
    }
    {
        /* 函数跳转入口 */
        vm_record();
        $<number>$ = vm_code_cnt;
        vm_gen(jmp, 0, 0);
        vm_output();

        cur_level ++;
        /* 记录定义函数的返回值类型 */
        type_ret[cur_level] = $1;
    }
    proc end_block
    {
        /* 记录定义函数的返回值类型 */
        type_ret[cur_level] = NULL;
        num_ret[cur_level] = 0;
        cur_level --;
    }
    {
        vm_set_jmp($<number>4, cur_block_pc, false);
        $$ = proc;
    }
    ; 

/* 参数列表与数组维度 */
arr_dim: LBRACKET LIT RBRACKET
    {

        if ($2->t != lint){
            yyerror("dimension is a non-integer");
        }
        else if($2->val <= 0){
            yyerror("dimension is a non-post");
        }
        else{
            $$ = create_type_desp(array, $2->val, 1);
        }
    }
    | arr_dim LBRACKET LIT RBRACKET
    {
        if($$->dim == MAX_ARR_DIM){
            yyerror("too many dimensions");
        }
        else if ($3->t != lint){
            yyerror("dimension is a non-integer");
        }
        else{
            $$ = $1;
            $$->shape[$$->dim] = $3->val;
            $$->dim += 1;
        }
    }
    ;
arr_idx: LBRACKET expr RBRACKET
    {   
        if($2){
            bool flag1 = err_type($2->type_info, lint, true, "dimension is a non-integer");
            if(flag1){
                $$ = create_arr_idx_s(1, ($2->islval));
            }
        }
        else{
            $$ = create_arr_idx_s(1, 0);
        }
    }
    | arr_idx LBRACKET expr RBRACKET
    {
        if($3){
            bool flag1 = err_type($3->type_info, lint, true, "dimension is a non-integer");
            if(flag1){
                $$ = $1;
                $$->islval[$$->dim] = ($3->islval);
                $$->dim += 1;
            }
        }
        else{
            $$->islval[$$->dim] = 0;
            $$->dim += 1;
        }
    }
    ;
param_list: decl_stmt
    {
        type_desp* type = table[tbl_tail].type;
        if(err_type(type, proc, false)){
            /* 从符号表最后一项中获取形参类型 */
            proc_desp* proc_info = create_proc_desp(NULL, NULL, true);
            table[tbl_tail].proc_info = proc_info;
            $$ = creat_args_s();
            $$->t_list[$$->len] = type;
            $$->len += 1;
        }
    }
    | param_list COMMA decl_stmt
    {
        type_desp* type = table[tbl_tail].type;
        if(err_type(type, proc, false)){
            proc_desp* proc_info = create_proc_desp(NULL, NULL, true);
            table[tbl_tail].proc_info = proc_info;
            $$ = $1;
            $$->t_list[$$->len] = type;
            $$->len += 1;
        }
    }
    ;
arg_list: expr
    {
        if($1){
            /* 函数参数只使用值传递，左值需要被转化成右值
                如果左值是字符串类型，需要复制串并返回新的地址
            */
            if($1->islval == 1){
                vm_gen(lod, -1, 0);
                if($1->type_info->t == lstring){
                    vm_gen(snew, 0, lstring, NULL);
                }
            }
            $$ = creat_args_s();
            $$->t_list[$$->len] = $1->type_info;
            $$->len += 1;
        }
        else{
            $$ = NULL;
        }
    }
    | arg_list COMMA expr
    {
        if($1 && $3){
            $$ = $1;
            if($3->islval == 1){
                vm_gen(lod, -1, 0);
                if($3->type_info->t == lstring){
                    vm_gen(snew, 0, lstring, NULL);
                }
            }
            $$->t_list[$$->len] = $3->type_info;
            $$->len += 1;
        }
        else{
            $$ = NULL;
        }
    }
    ;

/* 表达式语句 */
expr_stmt: expr BECOMES expr
    {
        if($1 && $3){
            /* 将栈顶元素赋值，要求左侧表达式为左值 */
            bool flag1 = type_equal($1->type_info, $3->type_info);
            bool flag2 = $1->islval;
            if(!flag1){
                yyerror("assign a different type variable");
            }
            if(!flag2){
                yyerror("assign a rval variable");
            }
            if(flag1 && flag2){
                int tbl_idx = $1->tbl_idx;
                vm_gen(sto, -1, 0, NULL, $3->islval, 0);
            }
        }
    }
    | expr
    {
        if($1){
            /* 直接弹出栈顶元素 */
            vm_gen(pop, 0, 1);
        }
    }
    ;


/* if语句 */
condition: expr
    {
        if($1){
            $$ = $1;
            err_type($1->type_info, lbool, true, "condition is not a bool expr");
        }
        else{
            $$ = NULL;
        }
    }
    ;
if_stmt: if_part_ifthen ENDSYM
    | if_part_ifthen 
    {
        $<number>$ = vm_record(false);
        vm_gen(jmp, 0, 0);
    }
    if_part_else 
    {
        vm_set_jmp($<number>2, vm_record(false));
    }
    ENDSYM;

if_part_ifthen: IFSYM LPAREN condition RPAREN THENSYM 
    {
        $<number>$ = vm_record(false);
        vm_gen(jpc, 0, 0);
    }
    LBRACE stmt_list RBRACE 
    {
        /* 还有一条jmp指令 */ 
        vm_set_jmp($<number>6, vm_record(false) + 1);
    }
if_part_else: ELSESYM LBRACE stmt_list RBRACE 

    ;
/* while语句 */
while_stmt: WHILESYM 
    {
        $<number>$ = vm_record(false);
    }
    LPAREN condition RPAREN 
    {
        $<number>$ = vm_record(false);
        vm_gen(jpc, 0, 0);
    }
    LBRACE stmt_list RBRACE
    {
        
        vm_gen(jmp, 0, 0);
        vm_set_jmp(vm_record(false)-1, $<number>2);
        vm_set_jmp($<number>6, vm_record(false));
    }
    ;
/* for语句 */
for_expr: expr_stmt
    | for_expr COMMA expr_stmt
    ;
for_substmt:  | for_expr;
for_stmt: FORSYM LPAREN for_substmt SEMICOLON
    {
        $<number>$ = vm_record(false);
    }
    condition SEMICOLON
    {
        $<number>$ = vm_record(false);
        vm_gen(jpc, 0, 0);
    }
    rec_cbuf_pc for_substmt RPAREN 
    rec_cbuf_pc LBRACE stmt_list RBRACE
    {
        vm_swap_block();
        vm_gen(jmp, 0, 0);
        vm_set_jmp(vm_record(false)-1, $<number>5);
        vm_set_jmp($<number>8, vm_record(false));
    }
    ;

/* IO语句 */
scan_item: expr
    {
        if($1){
            /* scan list 的项都应为左值 */
            bool flag1 = err_lval($1->islval, "scan expr should be lval");
            bool flag2 = err_type($1->type_info, array, false, "unable to scan array");
            if(flag1 && flag2){
                vm_gen(scn, 0, $1->type_info->t);
                vm_gen(sto, -1, 0);
            }
        }
    }
print_item: expr
    {
        if($1){
            if(err_type($1->type_info, array, false, "unable to wrt array")){
                vm_gen(wrt, 0, $1->type_info->t, NULL, $1->islval, 0);
            }
        }
    }
    ;
scan_list: scan_item
    {}
    | scan_list COMMA scan_item
    {}
    ;
print_list: print_item
    {}
    | print_list COMMA print_item
    {}
    ;
scan_stmt: SCANSYM LPAREN scan_list RPAREN
    {}
    ;
print_stmt: PRINTSYM LPAREN print_list RPAREN
    {}
    ;

/* 表达式 */
expr: and_expr
    {
        $$ = $1;
    }
    | expr OR and_expr
    {
        if($1 && $3){
            int op = opor;
            if($1->type_info->t != lbool || $3->type_info->t != lbool){
                yyerror("or on unsupported type");
            }
            else{
                $$ = gen_cmp_code(op, $3, $1);
            }
        }
        else{
            $$ = NULL;
        }
    }
    ;
and_expr: cmp_expr
    {
        $$ = $1;
    }
    | and_expr AND cmp_expr
    {
        if($1 && $3){
            int op = opand;
            if($1->type_info->t != lbool || $3->type_info->t != lbool){
                yyerror("and on unsupported type");
            }
            else{
                $$ = gen_cmp_code(op, $3, $1);
            }
        }
        else{
            $$ = NULL;
        }
    }
    ;
cmp_expr: add_expr
    {
        $$ = $1;
    }
    | cmp_expr EQ add_expr
    {
        if($1 && $3){
            int op = opeq;
            if($1->type_info->t == lstring) op = opseq;
            if(type_equal($1->type_info, $3->type_info) && $1->type_info->t != array){
                $$ = gen_cmp_code(op, $3, $1);
            }
            else{
                yyerror("cmp on unsupported types");
            }
        }
        else{
            $$ = NULL;
        }
    }
    | cmp_expr NEQ add_expr
    {
        if($1 && $3){
            int op = opneq;
            if($1->type_info->t == lstring) op = opsneq;
            if(type_equal($1->type_info, $3->type_info) && $1->type_info->t != array){
                $$ = gen_cmp_code(op, $3, $1);
            }
            else{
                yyerror("cmp on unsupported types");
            }
        }
        else{
            $$ = NULL;
        }
    }
    | cmp_expr LSS add_expr
    {
        if($1 && $3){
            int op = oplss;
            if(type_equal($1->type_info, $3->type_info) && $1->type_info->t == lint){
                $$ = gen_cmp_code(op, $3, $1);
            }
            else{
                yyerror("cmp on unsupported types");
            }
        }
        else{
            $$ = NULL;
        }
    }
    | cmp_expr LEQ add_expr
    {
        if($1 && $3){
            int op = opleq;
            if(type_equal($1->type_info, $3->type_info) && $1->type_info->t == lint){
                $$ = gen_cmp_code(op, $3, $1);
            }
            else{
                yyerror("cmp on unsupported types");
            }
        }
        else{
            $$ = NULL;
        }
    }
    | cmp_expr GTR add_expr
    {
        if($1 && $3){
            int op = opgtr;
            if(type_equal($1->type_info, $3->type_info) && $1->type_info->t == lint){
                $$ = gen_cmp_code(op, $3, $1);
            }
            else{
                yyerror("cmp on unsupported types");
            }
        }
        else{
            $$ = NULL;
        }
    }
    | cmp_expr GEQ add_expr
    {
        if($1 && $3){
            int op = opgeq;
            if(type_equal($1->type_info, $3->type_info) && $1->type_info->t == lint){
                $$ = gen_cmp_code(op, $3, $1);
            }
            else{
                yyerror("cmp on unsupported types");
            }
        }
        else{
            $$ = NULL;
        }
    }
    ;
add_expr: multi_expr
    {
        $$ = $1;
    }
    | add_expr PLUS multi_expr
    {
        if($1 && $3){
            if($1->type_info->t == lint && $3->type_info->t == lint){
                vm_gen(binop, opplus, 0, NULL, $3->islval, $1->islval);

                $$ = $1;
                $$->tbl_idx = -1;
                $$->islval = 0;

                free($3->type_info);
                free($3);
            }
            else if($1->type_info->t == lint && $3->type_info->t == lstring){
                vm_gen(mov, opmo, 2);
                vm_gen(snew, 0, lint, NULL, $1->islval);
                vm_gen(binop, opscat, 0, NULL, 0, $3->islval);
                vm_gen(mov, opmi, 1);
                
                $$ = $3;
                $$->tbl_idx = -1;
                $$->islval = 0;

                free($1->type_info);
                free($1);
            }
            else if($1->type_info->t == lstring && $3->type_info->t == lint){
                vm_gen(snew, 0, lint, NULL, $3->islval);
                vm_gen(binop, opscat, 0, NULL, 0, $1->islval);

                $$ = $1;
                $$->tbl_idx = -1;
                $$->islval = 0;

                free($3->type_info);
                free($3);
            }
            else if($1->type_info->t == lstring && $3->type_info->t == lstring){
                vm_gen(binop, opscat, 0, NULL, $3->islval, $1->islval);

                $$ = $1;
                $$->tbl_idx = -1;
                $$->islval = 0;

                free($3->type_info);
                free($3);
            }
            else{
                yyerror("plus on unsupported types");
            }
        }
        else{
            $$ = NULL;
        }
    }
    | add_expr MINUS multi_expr
    {
        if($1 && $3){
            if($1->type_info->t == lint && $3->type_info->t == lint){
                vm_gen(binop, opminus, 0, NULL, $3->islval, $1->islval);

                $$ = $1;
                $$->tbl_idx = -1;
                $$->islval = 0;

                free($3->type_info);
                free($3);
            }
            else{
                yyerror("minus on unsupported types");
            }
        }
        else{
            $$ = NULL;
        }
    };
multi_expr: unary_expr 
    {
        $$ = $1;
    }
    | multi_expr TIMES unary_expr
    {
        if($1 && $3){
            if($1->type_info->t == lint && $3->type_info->t == lint){
                vm_gen(binop, optimes, 0, NULL, $3->islval, $1->islval);

                $$ = $1;
                $$->tbl_idx = -1;
                $$->islval = 0;

                free($3->type_info);
                free($3);
            }
            else if($1->type_info->t == lstring && $3->type_info->t == lint){
                vm_gen(binop, opspow, 0, NULL, $3->islval, $1->islval);

                $$ = $1;
                $$->tbl_idx = -1;
                $$->islval = 0;

                free($3->type_info);
                free($3);
            }
            else if($1->type_info->t == lint && $3->type_info->t == lstring){
                vm_gen(mov, opmo, 2);
                vm_gen(binop, opspow, 0, NULL, $1->islval, $3->islval);
                vm_gen(mov, opmi, 1);

                $$ = $3;
                $$->tbl_idx = -1;
                $$->islval = 0;

                free($1->type_info);
                free($1);
            }
            else{
                yyerror("times on unsupported types");
            }
        }
        else{
            $$ = NULL;
        }
    }
    | multi_expr SLASH unary_expr
    {
        if($1 && $3){
            if($1->type_info->t == lint && $3->type_info->t == lint){
                vm_gen(binop, opslash, 0, NULL, $3->islval, $1->islval);

                $$ = $1;
                $$->tbl_idx = -1;
                $$->islval = 0;

                free($3->type_info);
                free($3);
            }
            else{
                yyerror("slash on unsupported types");
            }
        }
        else{
            $$ = NULL;
        }
    }
    | multi_expr MOD unary_expr
    {
        if($1 && $3){
            if($1->type_info->t == lint && $3->type_info->t == lint){
                vm_gen(binop, opmod, 0, NULL, $3->islval, $1->islval);

                $$ = $1;
                $$->tbl_idx = -1;
                $$->islval = 0;

                free($3->type_info);
                free($3);
            }
            else{
                yyerror("mod on unsupported types");
            }
        }
        else{
            $$ = NULL;
        }
    }
    ;
unary_expr: pre_expr
    {
        $$ = $1;
    }
    | NOT unary_expr
    {
        if($2){
            bool flag1 = err_type($2->type_info, lbool, true, "not a non-bool");
            if(flag1){
                vm_gen(unaop, opnot, 0, NULL, $2->islval, 0);
                $$ = $2;
                $$->tbl_idx = -1;
                $$->islval = 0;
            }
        }
        else{
            $$ = NULL;
        }
    }
    ;

pre_expr: post_expr
    {
        $$ = $1;
    }
    | INC post_expr
    {
        if($2){
            bool flag1 = err_type($2->type_info, lint, true, "inc a non-integer");
            bool flag2 = err_lval($2->islval);
            if(flag1 && flag2){
                vm_gen(unaop, opinc, 0, NULL, 1, 0);
                $$ = $2;
            }
        }
        else{
            $$ = NULL;
        }
        
    }
    | DEC post_expr
    {
        if($2){
            bool flag1 = err_type($2->type_info, lint, true, "dec a non-integer");
            bool flag2 = err_lval($2->islval);
            if(flag1 && flag2){
                vm_gen(unaop, opdec, 0, NULL, 1, 0);
                $$ = $2;
            }
        }
        else{
            $$ = NULL;
        }
    }
    | PLUS post_expr
    {
        if($2){
            bool flag1 = err_type($2->type_info, lint, true);
            if(flag1){
                /* 不做任何运算，但结果转为右值 */
                if($2->islval){
                    vm_gen(lod, -1, 0);
                }
                $$ = $2;
                $$->tbl_idx = -1;
                $$->islval = 0;
            }
        }
        else{
            $$ = NULL;
        }
    }
    | MINUS post_expr
    {
        if($2){
            bool flag1 = err_type($2->type_info, lint, true);
            if(flag1){
                vm_gen(lit, 0, -1);
                vm_gen(binop, optimes, 0, NULL, 0, $2->islval);
                $$ = $2;
                $$->tbl_idx = -1;
                $$->islval = 0;
            }
        }
        else{
            $$ = NULL;
        }
    }
    ;

post_expr: term
    {
        $$ = $1;
    }
    | post_expr INC
    {
        if($1){
            bool flag1 = err_type($1->type_info, lint, true, "inc a non-integer");
            bool flag2 = err_lval($1->islval);
            if(flag1 && flag2){
                vm_gen(unaop, opinc, 0, NULL, 1, 0);
                vm_gen(lit, 0, 1);
                vm_gen(binop, opminus, 0, NULL, 0, 1);/* 执行二元操作时，压入栈的永远是右值 */
                $$ = $1;
                $$->tbl_idx = -1;
                $$->islval = 0;
            }        
        }
        else{
            $$ = NULL;
        }
    }
    | post_expr DEC
    {
        if($1){
            bool flag1 = err_type($1->type_info, lint, true, "dec a non-integer");
            bool flag2 = err_lval($1->islval);
            if(flag1 && flag2){
                vm_gen(unaop, opdec, 0, NULL, 1, 0);
                vm_gen(lit, 0, 1);
                vm_gen(binop, opplus, 0, NULL, 0, 1);
                $$ = $1;
                $$->tbl_idx = -1;
                $$->islval = 0;
        }  
        }
        else{
            $$ = NULL;
        }
        
    }
    ;
term: factor 
    {
        $$ = $1;
    }
    | ADDR term
    {
        if($2){
            /* 取地址符只能处理左值类型 */
            /* 将栈顶元素作为一个右值 */
            if(err_lval($2->islval, "addr a rval variable")){
                $$ = $2;
                /* 表达式是一个指针类型，需要修改member_t字段 */
                type_desp* type_term = create_type_desp(pointer, -1, 1);

                /* 对数组取地址：获取数组首元素的地址 */
                if($$->type_info->t == array){
                    type_term->member_t = $$->type_info->member_t;
                    free($$->type_info);
                    $$->type_info = type_term;
                }
                /* 对指针取地址：增加指针的维度 */
                else if($$->type_info->t == pointer){
                    type_term->dim += $$->type_info->dim;
                    type_term->member_t = $$->type_info->member_t;
                    free($$->type_info);
                    $$->type_info = type_term;
                }
                /* 对普通类型取地址 */
                else{
                    type_term->member_t = $$->type_info;
                    $$->type_info = type_term;
                }
                $$->tbl_idx = -1;
                $$->islval = 0;
            }
        }
        else{
            $$ = NULL;
        }
    }
    | TIMES term
    {
        if($2){
            /* 如果栈顶是左值, 加载值至栈顶 */
            if($2->islval == 1){
                vm_gen(lod, -1, 0);
            }

            /* 
                对指针执行一层解引用
            */
            if(err_type($2->type_info, pointer, true, "derefer a non-pointer")){
                type_desp* type_term;
                $$ = $2;
                $$->islval = 1;
                $$->type_info->dim -= 1;
                if($$->type_info->dim == 0){
                    type_term = $$->type_info->member_t;
                    free($$->type_info);
                    $$->type_info = type_term;
                }
            }
        }
        else{
            $$ = NULL;
        }
    }
    ;
factor: LPAREN expr RPAREN
    {
        $$ = $2;
    }
    | ID
    {
        int tbl_idx = table_position($1, tbl_tail);
        if(err_ident(tbl_idx)){
            type_desp* type_factor;
            vm_gen(lit, 0, table[tbl_idx].addr);
            vm_gen(addr, cur_level - table[tbl_idx].level, 0);
            /* 如果是数组，表达式的值为数组首地址，此时表达式为右值 */
            if(table[tbl_idx].type->t == array){
                type_factor = create_type_desp(pointer, -1, 1);
                type_factor->member_t = type_copy(table[tbl_idx].type->member_t);

                $$ = create_expr_s(type_factor, -1);
            }
            else{
                if(err_type(table[tbl_idx].type, proc, false)){
                    type_factor = type_copy(table[tbl_idx].type);
                    $$ = create_expr_s(type_factor, tbl_idx);
                }
            }
        }
        else{
            $$ = NULL;
        }
    }
    | ID arr_idx
    {
        int tbl_idx = table_position($1, tbl_tail);
        if(err_ident(tbl_idx)){
            type_desp* var_type_info = table[tbl_idx].type;
            /* 类型检查和维度检查 */
            bool flag_arr = var_type_info->t == array;
            bool flag_ptr = var_type_info->t == pointer;

            if(flag_arr && err_dim($2->dim, var_type_info->dim)){
                /* 索引计算 */
                vm_gen(mov, opmo, $2->dim);
                if($2->dim == 1 && $2->islval[0]){
                    vm_gen(lod, -1, 0);
                }

                for(int i = 1; i < $2->dim; i++){
                    vm_gen(lit, 0, var_type_info->shape[i]);
                    vm_gen(binop, optimes, 0, NULL, 0, $2->islval[i-1]);
                    vm_gen(binop, opplus, 0, NULL, 0, $2->islval[i]);
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
                if(var_type_info->dim == $2->dim){
                    type_factor = type_copy(var_type_info->member_t);
                    $$ = create_expr_s(type_factor, tbl_idx);
                }
                else{
                    /* 如果是地址，转换成指针类型 */
                    type_factor = create_type_desp(pointer, -1, 1);
                    type_factor->member_t = type_copy(var_type_info->member_t);
                    $$ = create_expr_s(type_factor, -1);
                }
                
            }
            else if(flag_ptr && err_dim($2->dim, 1)){
                /* 索引计算 */
                vm_gen(lit, 0, table[tbl_idx].addr);
                vm_gen(addr, cur_level - table[tbl_idx].level, 0);
                vm_gen(lod, -1, 0);
                vm_gen(binop, opplus, 0, NULL, 1, $2->islval[0]);
                vm_gen(addr, cur_level - table[tbl_idx].level, 0);

                type_desp* type_factor;
                if(table[tbl_idx].type->dim == 1){
                    type_factor = type_copy(table[tbl_idx].type->member_t);
                }
                else{
                    type_factor = type_copy(table[tbl_idx].type);
                    type_factor->dim -= 1;
                }
                $$ = create_expr_s(type_factor, tbl_idx);
            }
            else{
                yyerror("index a non-array");
            }
        }
        else{
            $$ = NULL;
        }

    }
    | ID LPAREN 
    {
        /* 压入当前堆尾地址 */
        vm_gen(reg, 0, 0);
    }
    arg_list RPAREN
    {
        /* 获取函数标识符表项 */
        int tbl_idx = table_position($1, tbl_tail);
        if(err_ident(tbl_idx)){
            bool flag1 = err_type(table[tbl_idx].type, proc);
            bool flag2 = flag1 && err_args(tbl_idx, $4);
            if(flag2){
            
            /* 参数被压入辅助栈 */
            vm_gen(mov, opmo, table[tbl_idx].size + 1);

            /* cal指令计算RA，DL，SL，压入辅助栈后将所有元素弹回主栈 */
            vm_gen(cal, cur_level - table[tbl_idx].level, table[tbl_idx].addr);

            /* 语义计算结果为返回值类型，右值 */
            type_desp* type_factor = type_copy(table[tbl_idx].proc_info->ret_type);
            $$ = create_expr_s(type_factor, -1);
            }
        }
        else{
            $$ = NULL;
        }
    }
    | ID LPAREN 
    {
        /* 压入当前堆尾地址 */
        vm_gen(reg, 0, 0);
    }
    RPAREN
    {
        /* 获取函数标识符表项 */
        int tbl_idx = table_position($1, tbl_tail);
        if(err_ident(tbl_idx)){
            bool flag1 = err_type(table[tbl_idx].type, proc);
            if(flag1){
                /* 参数被压入辅助栈 */
                vm_gen(mov, opmo, table[tbl_idx].size + 1);

                /* cal指令计算RA，DL，SL，压入辅助栈后将所有元素弹回主栈 */
                vm_gen(cal, cur_level - table[tbl_idx].level, table[tbl_idx].addr);

                /* 语义计算结果为返回值类型，右值 */
                type_desp* type_factor = type_copy(table[tbl_idx].proc_info->ret_type);
                $$ = create_expr_s(type_factor, -1);
            }
        }
        else{
            $$ = NULL;
        }
    }
    | LIT
    {
        if($1){
            type_desp* type_factor = create_type_desp($1->t, -1, -1);
            if($1->t == lstring){
                /* 处理字符串类型 */
                vm_gen(snew, 0, lstring, $1->str);
            }
            else{
                /* 处理布尔/整型类型 */
                vm_gen(lit, 0, $1->val, NULL);
            }
            $$ = create_expr_s(type_factor, -1);
        }
        else{
            $$ = NULL;
        }
    };
rec_cbuf_pc:
    {
        /* 更新 block_pc_stack*/
        $$ = vm_record(); 
    }
load_params:
    {
        push_flag = !push_flag;
    }
    ;

////////////////////////////////////////////////////////
//程序部分
%%
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
        if ((finput = fopen("finput.txt", "w")) == NULL){
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