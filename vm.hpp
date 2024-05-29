#pragma

#include "table.hpp"

#define VM_INST_MAX 5000 // 虚拟机指令上限
#define VM_STACK_MAX 50000 // 虚拟机栈上限
#define VM_HEAP_MAX 10000 // 虚拟机堆上限
#define VM_STRBUF_MAX 300 // 字符串缓冲区长度
#define VM_FCT_N 20 // 虚拟机指令集数量
#define VM_OP_N 30 // 虚拟机指令集数量

/* 虚拟机指令助记符： 指令集已经重新设计 */
enum fct {
    lod, lit, sto, cal, jmp, jpc, 
    addr,
    wrt, scn, 
    binop,
    unaop,
    snew,
    reg,
    push,
    pop,
    mov,
    ret,
};
enum opcode {
    opplus, opminus, optimes, opslash, opmod, opeq, opneq, oplss, opleq, opgtr, opgeq, opand, opor, opnot,
    opaddr, opptr, opinc, opdec, opscat, opspow, opseq, opsneq, opmo, opmi, 
};
extern char fct_str[VM_FCT_N][10];

typedef struct string_object{
    char* str_data;
    int str_len;
} object;


/* 虚拟机代码结构 */
typedef struct instruction {
	enum fct f;   /* 虚拟机代码指令 */
    int op1;      /* 操作数1，可能为层差，运算符*/
	int op2;      /* 操作数2，可能为地址，元素数量，类型，重复次数*/
    char *str;    /* 操作数2，文本*/
    int lval1;
    int lval2;
} ins;

extern ins code[VM_INST_MAX];
extern char fct_str[VM_FCT_N][10];
extern int vm_code_cnt;
extern int vm_base, vm_tail;
extern int vm_pc;

/* 初始化虚拟机 */
void vm_init(); 
/* 设置jmp, jpc指令跳转地址 */
void vm_set_jmp(int p, int addr, bool isbuf = true); 
/* 添加一条新指令 */
void vm_gen(enum fct ins, int op1, int op2, char* s = NULL, int lval1 = 0, int lval2 = 0); 


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
void print_codeline(int line, ins* a, FILE* fcode);
/* 打印虚拟代码和堆栈信息 */
void vm_print_code(FILE* fcode); 
void vm_print_storage(int p, FILE* foutput); 