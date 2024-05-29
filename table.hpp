#pragma

#include <stdio.h>

#define MAX_ID_TABLE_LEN 400
#define MAX_ID_LEN 20
#define MAX_STR_LEN 300
#define MAX_ARG_LEN 10 /* 参数列表的最大长度 */
#define MAX_ARR_DIM 10 /* 数组最大维度*/
#define MAX_NEST_DEPTH 4 /* 最大嵌套深度*/

#define INI_PUSH_SIZE 4 /* 过程调用时开辟的栈空间，用于存放返回地址等信息 */
#define MAX_ADDR 2048     /* 地址上界*/
#define MAX_LEVEL 3      /* 最大允许过程嵌套声明层数*/
#define MAX_VM_CODE 500     /* 最多的虚拟机代码数 */
#define MAX_STACK_SIZE 700 /* 运行时数据栈元素最多为500个 */

/* id 的类型 */
enum id_type {
    lint, lbool, lstring, // 可用于字面量
    pointer, array, proc
};

/* 类型的描述符，针对指针和数组 */
typedef struct type_description{
    int t; /* 自身的类型，可能是id_type中除proc的一种 */
    int shape[MAX_ARR_DIM]; /* 数组的形状或索引 */
    int dim; /* 数组的维度或指针的层数 */
    struct type_description* member_t; /* 指针/数组装载对象的类型，可能是指针，布尔，串或整形 */
} type_desp;

/* 字面量的语义值类型 */
typedef struct literal_semantic {
    char* str; /* 如果字面量是字符串，则str为对应的指针，否则为NULL */
    int val; /* 字面量的值 */
    enum id_type t; /* 字面量的类型 */
} literal_s;

/* 表达式的语义值类型 */
typedef struct expression_semantic {
    type_desp* type_info; /* 类型信息 */ 
    int islval; /* 是否是左值 */
    int tbl_idx; /* 如果是左值，其在符号表中的位置（无用） */
}expr_s;

/* 数组索引的语义值类型 */
typedef struct arridx_semantic{
    int dim; /* 索引维度 */
    int islval[MAX_ARR_DIM]; /* 相应维度的索引值是否是左值 */
} arr_idx_s;

/* 函数参数列表的语义值类型 */
typedef struct args_semantic{
    type_desp* t_list[MAX_ARG_LEN]; /* 参数列表类型 */
    int len; /* 参数列表长度 */
} args_s;

/* 函数的描述符 */
typedef struct proc_description{
    bool is_param;
    args_s* param_info;
    type_desp* ret_type;
} proc_desp;


/* 符号表结构 */
struct table_item {
    char name[MAX_ID_LEN];     /* 标识符 */
    type_desp *type;  /* 变量类型 */
    proc_desp *proc_info;
    int level; /* 层次 */
    int addr;  /* 标识符偏移地址或函数入口 */
    int size; /* 存储过程占用的栈空间大小，数组大小/函数变量占用 */
};

extern table_item table[MAX_ID_TABLE_LEN]; 
extern int tbl_tail;
extern int tbl_id_addr[MAX_NEST_DEPTH]; 
extern char tbl_id[MAX_ID_LEN];

type_desp* create_type_desp(int t, int shape0, int dim);
proc_desp* create_proc_desp(args_s* param_info, type_desp* ret_type, bool isparam = false);
arr_idx_s* create_arr_idx_s(int dim, int islval);
expr_s* create_expr_s(type_desp* type_info, int tbl_idx);
args_s* creat_args_s();

/* 深拷贝类型 */
type_desp* type_copy(type_desp* t);
/* 类型比较 */
int type_equal(type_desp* t1, type_desp* t2);
/* 获取该类型所占的空间大小 */
int get_type_size(type_desp* type);

/* 初始化 */
void table_init();
/* 在符号表中加入一个标识符 */
void table_enter(type_desp* type, char* ident, int level, int size, int* tbl_tail); 
/* 从尾指针开始，在符号表中查找标识符 */
int table_position(char *id, int tbl_tail); 
/* 清楚当前层的符号表 */
void table_clear(int* tbl_tail, int level);
/* 打印类型信息 */
void type_print(type_desp* type, FILE* ftable = stdout);
/* 打印符号表信息 */
void table_print(FILE* ftable = stdout); 
