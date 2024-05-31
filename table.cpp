#include "table.hpp"
#include <stdlib.h>
#include <string.h>

table_item table[MAX_ID_TABLE_LEN]; /* 符号表 */
int tbl_tail;             /* 符号表当前尾指针 */
int tbl_id_addr[MAX_NEST_DEPTH]; /* 当前深度的过程的变量偏移 */
char tbl_id[MAX_ID_LEN];  /* 存储编译过程中出现的标识符 */

type_desp* create_type_desp(int t, int shape0, int dim){
    type_desp* ret = (type_desp*) malloc(sizeof(type_desp));
    ret->t = t;
    ret->shape[0] = shape0;
    ret->dim = dim;
    ret->member_t = NULL;
    return ret;
}

proc_desp* create_proc_desp(args_s* param_info, type_desp* ret_type, bool isparam){
    proc_desp* ret = (proc_desp*) malloc(sizeof(proc_desp));
    ret->is_param = isparam;
    ret->param_info = param_info;
    ret->ret_type = ret_type;
    return ret;
}

expr_s* create_expr_s(type_desp* type_info, int tbl_idx){
    expr_s* ret = (expr_s*) malloc(sizeof(expr_s));
    ret->tbl_idx = tbl_idx;
    ret->type_info = type_info;
    if(tbl_idx == -1) ret->islval = 0;
    else ret->islval = 1;
    return ret;
}

arr_idx_s* create_arr_idx_s(int dim, int islval){
    arr_idx_s* ret = (arr_idx_s*) malloc(sizeof(arr_idx_s));
    for(int i = 0; i < MAX_ARR_DIM; i++) ret->islval[i] = 0;
    ret->dim = dim;
    ret->islval[0] = islval;
    return ret;
}

args_s* creat_args_s(){
    args_s* ret = (args_s*) malloc(sizeof(args_s));
    ret->len = 0;
    return ret;
}

type_desp* type_copy(type_desp* t){
    if(t == NULL) return NULL;
    type_desp* ret = (type_desp*)malloc(sizeof(type_desp));
    memcpy(ret, t, sizeof(type_desp));
    if(t->member_t != NULL){
        ret->member_t = type_copy(t->member_t);
    }
    return ret;
}

int type_equal(type_desp* t1, type_desp* t2){
    if(t1 == NULL || t2 == NULL) return 0;
    if(t1->t != t2->t) return 0;
    
    if(t1->t == pointer){
        bool a = t1->member_t->t == t2->member_t->t;
        bool b = t1->dim == t2->dim;
        return a && b;
    }
    else if(t1->t == array){
        return type_equal(t1->member_t, t2->member_t);
    }
    else if(t1->t == proc){
        /* 不对proc类型的标识符进行类型检查 */
        return 0;
    }
    return 1;
}

int get_type_size(type_desp* type){
    if(type->t != array) return 1;
    else{
        int ret = 1;
        for(int i = 0; i < type->dim; i++){
            ret *= type->shape[i];
        }
        return ret;
    }
}

void table_init(){
    tbl_tail = -1;
    tbl_id[0] = 0;
    for (int i = 0; i < MAX_NEST_DEPTH; i++) tbl_id_addr[i] = INI_PUSH_SIZE;
    memset(table, 0, MAX_ID_TABLE_LEN*sizeof(table_item));
}

/* 在符号表中加入一个标识符 */
void table_enter(type_desp* type, char* ident, int level, int size, int* tbl_tail){
    int new_tbl_tail = *tbl_tail+1;
    if(new_tbl_tail == MAX_ID_TABLE_LEN){
        printf("Too many identities");
        return;
    }
    table[new_tbl_tail].type = type;
    table[new_tbl_tail].proc_info = NULL;
    table[new_tbl_tail].level = level;
    table[new_tbl_tail].size = size;
    table[new_tbl_tail].addr = tbl_id_addr[level];
    tbl_id_addr[level] += size;
    strcpy(table[new_tbl_tail].name, ident);
    *tbl_tail = new_tbl_tail;

    // printf("enter: %s\n", ident);
    return;
}

/* 在符号表中查找标识符 */
int table_position(char *id, int tbl_tail){
    // printf("try to find %s from %d\n", id, tbl_tail);
    int i = tbl_tail;
    while(i >= 0){
        if(strcmp(id, table[i].name) == 0){
            return i;
        }
        else{
            i--;
        }
    }
    return -1;
}

void table_clear(int* tbl_tail, int level){
    int i = *tbl_tail;
    while(table[i].level == level) i--;
    *tbl_tail = i;
}

/* 打印类型信息 */
void type_print(type_desp* type, FILE* ftable){
    if(type == NULL) return;
    if(type->t == pointer){
        type_print(type->member_t, ftable);
        for(int i = 0; i < type->dim; i++) fprintf(ftable, "*");
    }
    else if(type->t == array){
        type_print(type->member_t, ftable);
        for(int i = 0; i < type->dim; i++) fprintf(ftable, "[%d]", type->shape[i]);
    }
    else if(type->t == proc){
        fprintf(ftable, "proc");
    }
    else if(type->t == lbool){
        fprintf(ftable, "lbool");
    }
    else if(type->t == lint){
        fprintf(ftable, "lint");
    }
    else if(type->t == lstring){
        fprintf(ftable, "lstring");
    }
    return;
}

/* 打印符号表信息 */
void table_print(FILE* ftable){
    
    fprintf(ftable, "\ncurrent table:\n");
    fprintf(ftable, "line\tid\t\tlevel\taddr\tsize\ttype\tproc_info\t\n");
    for(int i = 0; i <= tbl_tail; i++){
        fprintf(ftable, "%d\t%s\t\t%d\t%d\t%d\t", i, table[i].name, table[i].level, table[i].addr, table[i].size);
        type_print(table[i].type, ftable); 
        fprintf(ftable, "\t");
        if(table[i].proc_info && !table[i].proc_info->is_param){
            args_s* params = table[i].proc_info->param_info;
            fprintf(ftable, "(");
            for(int i = 0; i < params->len; i++){
                type_print(params->t_list[i], ftable);
                fprintf(ftable, ",");
            }
            fprintf(ftable, ") -> ");
            type_print(table[i].proc_info->ret_type, ftable);
        }
        fprintf(ftable, "\n");
    }
    fprintf(ftable, "\n");
} 