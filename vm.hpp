#pragma

#include "table.hpp"

#define VM_INST_MAX 5000 // 虚拟机指令上限
#define VM_STACK_MAX 50000 // 虚拟机栈上限
#define VM_HEAP_MAX 10000 // 虚拟机堆上限
#define VM_STRBUF_MAX 1000 // 字符串缓冲区长度
#define VM_FCT_N 20 // 虚拟机指令集数量
#define VM_OP_N 30 // 虚拟机指令集数量

/* 虚拟机指令助记符： 指令集已经重新设计 */
enum fct {
    lod, lit, sto, cal, jmp, jpc, 
    addr,
    wrt, scn, // 输出/输入指令
    binop, // 算术指令， 加减乘除 
    unaop, // 逻辑运算指令
    snew, // 字符串加载指令
    reg, // 字符串操作指令，拼接，重复，比较
    push, // 压栈指令，用于为数组分配空间
    pop, // 弹出栈/偏移量栈栈顶的指令，用于翻译赋值表达式和数组寻址
    mov,
    ret, // 结束函数调用
};
enum opcode {
    opplus, opminus, optimes, opslash, opmod, opeq, opneq, oplss, opleq, opgtr, opgeq, opand, opor, opnot,
    opaddr, opptr, opinc, opdec, opscat, opspow, opseq, opsneq, opmo, opmi, 
};

typedef struct string_object{
    char* str_data;
    int str_len;
} object;


/* 虚拟机代码结构: 不存在过程嵌套，不再使用层次差 */
typedef struct instruction {
	enum fct f;   /* 虚拟机代码指令 */
    int op1;      /* 操作数1，可能为层差，运算符*/
	int op2;      /* 操作数2，可能为地址，元素数量，类型，重复次数*/
    char *str;    /* 操作数2，文本*/
    int lval1;
    int lval2;
} ins;

extern ins code[VM_INST_MAX]; /* 存放虚拟机代码的数组 */
extern char fct_str[VM_FCT_N][10];
extern int vm_code_cnt; /* 当前指令下标 */
extern int vm_base, vm_tail;
extern int vm_pc;

/* 初始化虚拟机 */
void vm_init(); 
/* 设置jmp, jpc指令跳转地址 */
void vm_set_jmp(int p, int addr, bool isbuf = true); 
/* 添加一条新指令 */
void vm_gen(enum fct ins, int op1, int op2, char* s = NULL, int lval1 = 0, int lval2 = 0); 

// void vm_load_ins(FILE* inf); /* 将指令文件载入虚拟机中 */
// void vm_save_ins(FILE* outf); /* 将虚拟机中的指令保存到文件中 */
/* 单步执行当前指令 */
void vm_step(FILE* inf, FILE* outf); 
/* 执行整个程序 */
void vm_execute(FILE* inf, FILE* outf); 


/* 记录当前code buffer尾指针位置
    如果push为真，则将尾指针压入block_pc_stack中
*/
int vm_record(bool push = true); 
/* 交换code buffer中尾部两个block的代码，用于for循环语句的实现 */
void vm_swap_block();
/*将函数体代码从code buffer输出到code数组*/
void vm_output(); 

/* 打印一行虚拟代码*/
void code_print(int line, ins* a, FILE* fcode);
/* 打印虚拟代码*/
void vm_print_code(FILE* fcode); 
void vm_print_storage(int p, FILE* foutput); 