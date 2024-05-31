#include "vm.hpp"
#include <stdlib.h>
#include <memory.h>
#include <string.h>

char fct_str[VM_FCT_N][10] = {
    "lod", "lit", "sto", "cal", "jmp", "jpc", 
    "addr",
    "wrt", "scn",
    "binop",
    "unaop",
    "snew",
    "reg",
    "push",
    "pop", 
    "mov",
    "ret", 
};

ins code[VM_INST_MAX]; /* 存放虚拟机代码的数组 */
int vm_pc; /* 当前指令下标 */
int vm_code_cnt; /* 虚拟机指令数量 */

ins code_buf[VM_INST_MAX]; /* 暂存虚拟机代码的数组 */
int vm_buf_pc; /* 暂存虚拟机代码的数组的尾指针 */
int block_pc_stack[MAX_NEST_DEPTH*3]; /* buffer中每个block起始位置的下标，该数组会在分析函数和for语句时被使用，同一层不会超过3次 */
int block_pc_top;

int vm_stack[2][VM_STACK_MAX]; /* 虚拟机的栈，每个数据均占4字节；*/
int vm_top[2];/* 虚拟机的栈顶指针 */
int p_stk;/* 当前操作的栈 */

object* vm_heap[VM_HEAP_MAX]; /* 虚拟机的堆，仅用于存放字符串；*/
int vm_base, vm_tail; /* 当前过程基址，堆尾*/
char vm_strbuf[VM_STRBUF_MAX]; /* 输入缓冲区，用于scan */

/* 初始化虚拟机 */
void vm_init(){
    memset(code, 0, VM_INST_MAX*sizeof(ins));
    memset(code_buf, 0, VM_INST_MAX*sizeof(ins));
    memset(block_pc_stack, 0, MAX_NEST_DEPTH*3*sizeof(int));
    memset(vm_stack, 0, 2*VM_STACK_MAX*sizeof(int));
    memset(vm_top, 0, 2*sizeof(int));

    vm_stack[0][2] = VM_INST_MAX - 1;
    vm_top[0] = INI_PUSH_SIZE;

    vm_pc = 0;
    vm_buf_pc = 0;

    block_pc_top = 0;
    vm_code_cnt = 0;
    p_stk = 0;

    vm_base = 0;
    vm_tail = 0;
}

/* 清理当前函数使用的字符串，在函数返回时调用 */
int vm_heap_clear(int front, int idx, int* tail){
    int start = front;
    /* idx >= 0 说明返回值是字符串类型 */
    if(idx >= 0){
        object* tmp = vm_heap[front];
        vm_heap[front] = vm_heap[idx];
        vm_heap[idx] = tmp;
        start = front + 1;
    }
    for(int i = start; i < *tail; i++){
        free(vm_heap[i]->str_data);
        free(vm_heap[i]);
    }
    *tail = start;
    return front;
}

/* 设置main跳转地址 */
void vm_set_jmp(int p, int addr, bool isbuf){
    if(isbuf) code_buf[p].op2 = addr - p;
    else code[p].op2 = addr - p;
}

/* 生成一条指令 */
void vm_gen(enum fct ins, int op1, int op2, char* s, int lval1, int lval2){
    code_buf[vm_buf_pc].f = ins;
    code_buf[vm_buf_pc].op1 = op1;
    code_buf[vm_buf_pc].op2 = op2;
    code_buf[vm_buf_pc].str = s;
    code_buf[vm_buf_pc].lval1 = lval1;
    code_buf[vm_buf_pc].lval2 = lval2;
    vm_buf_pc++;
}

/* 交换两个block的顺序 */
void vm_swap_block(){
    if(block_pc_top < 2) return;
    int p1 = block_pc_stack[block_pc_top - 2];
    int p2 = block_pc_stack[block_pc_top - 1];
    block_pc_top -= 2;
    ins* tmp = (ins*) malloc(sizeof(ins)*(vm_buf_pc - p1));
    memcpy(tmp, &code_buf[p1], sizeof(ins)*(vm_buf_pc - p1));
    for(int i = 0; i < vm_buf_pc - p2; i++){
        code_buf[i+p1] = tmp[i+p2-p1];
        // printf("%d<=%d\n", i+p1, i+1+p2-p1);
    }
    for(int i = 0; i < p2 - p1; i++){
        code_buf[i+vm_buf_pc-p2+p1] = tmp[i];
        // printf("%d<=%d\n", vm_buf_pc-p2+p1+i, i);
    }
    free(tmp);
}

/* 记录code_buf的尾指针位置 */
int vm_record(bool push){
    if(push){
        block_pc_stack[block_pc_top++] = vm_buf_pc;
    }
    return vm_buf_pc;
}
/* 将一个block从code_buf输出到code */
void vm_output(){
    for(int i = block_pc_stack[block_pc_top-1]; i < vm_buf_pc; i++){
        code[vm_code_cnt++] = code_buf[i];
    }
    vm_buf_pc = block_pc_stack[block_pc_top-1];
    block_pc_top--;
}


void print_codeline(int line, ins* a, FILE* fcode = stdout){
    fprintf(fcode, "%d\t", line);
    fprintf(fcode, "%8s\t", fct_str[a->f]);
    fprintf(fcode, "%d\t",a->op1);
    fprintf(fcode, "%d\t",a->op2);
    fprintf(fcode, "%c%c", a->lval1?'*':'-', a->lval2?'*':'-');
    if(a->str) fprintf(fcode, "%s",a->str);
    fprintf(fcode, "\n");
}
void vm_print_code(FILE* fcode){
    fprintf(fcode, "\nline\tins\t\top1\top2\tstr\tislval\t\n");
    for(int i = 0; i < vm_code_cnt; i++){
        print_codeline(i, &code[i], fcode);
    }
    fprintf(fcode, "\n");
}
void vm_print_storage(int cmd, FILE* foutput){
    if(cmd == '1'){
        fprintf(foutput, "\nstack0\n");
        for(int i = 0; i < vm_top[0]; i++){
            fprintf(foutput, "%d\n", vm_stack[0][i]);
        }
    }
    else if(cmd == '2'){
        fprintf(foutput, "\nstack1\n");
        for(int i = 0; i < vm_top[1]; i++){
            fprintf(foutput, "%d\n", vm_stack[1][i]);
        }
    }
    else if(cmd == '3'){
        fprintf(foutput, "\nstr\n");
        for(int i = 0; i < vm_tail; i++){
            fprintf(foutput, "%s\n", vm_heap[i]->str_data);
        }
    }
    else if(cmd == '4'){
        fprintf(foutput, "\nregs\n");
        fprintf(foutput, "pc:%d\n", vm_pc);
        fprintf(foutput, "code_cnt:%d\n", vm_code_cnt);
        fprintf(foutput, "base:%d\n", vm_base);
        fprintf(foutput, "pstk:%d\n", p_stk);
        
    }
}


int get_base(int L){
    int b = vm_base;
    while(L > 0){
        b = vm_stack[0][b];
        L -= 1;
    }
    return b;
}

void vm_step(FILE* inf, FILE* outf){
    ins* i = &code[vm_pc];
    int* cur_stack = &vm_stack[p_stk][0];
    int* cur_top = &vm_top[p_stk];
    switch (i->f){
        case lit:{
            cur_stack[(*cur_top)++] = i->op2;
            break;
        }
        case lod:{
            if(i->op1 == -1){
                /* 绝对地址指的是主栈的地址 */
                // printf("lod_debug: %d\n", vm_stack[0][vm_top[0]-1]);
                int adr = cur_stack[--(*cur_top)];
                cur_stack[(*cur_top)++] = vm_stack[0][adr];
            }
            break;
        }
        case sto:{
            if(i->op1 == -1){
                if(i->lval1 == 1){
                    cur_stack[(*cur_top) - 1] = vm_stack[0][cur_stack[(*cur_top) - 1]];
                }
                /* 绝对地址指的是主栈的地址 */
                int n = cur_stack[--(*cur_top)];
                int adr = cur_stack[--(*cur_top)];
                // printf("sto_debug: %d-%d\n", n, adr);
                vm_stack[0][adr] = n;
            }
            break;
        }
        case jmp:{
            vm_pc += i->op2 - 1;
            break;
        }
        case jpc:{
            int n = cur_stack[--(*cur_top)];
            if(n == 0) vm_pc += i->op2 - 1;
            break;
        }
        case addr:{
            int b = get_base(i->op1);
            int n = cur_stack[--(*cur_top)];
            cur_stack[(*cur_top)++] = b + n;
            break;
        }
        case scn:{
            fprintf(outf,"\ninput:\n");
            if(i->op2 == lint){
                fscanf(inf, "%d", cur_stack+((*cur_top)++));
            }
            else if(i->op2 == lstring){
                    fscanf(inf, "%s", vm_strbuf);
                    object* obj = (object*)malloc(sizeof(object)); 
                    int len = strlen(vm_strbuf);
                    obj->str_data = (char*) malloc(len);
                    strcpy(obj->str_data, vm_strbuf);
                    obj->str_len = len;

                    vm_heap[vm_tail] = obj;
                    cur_stack[(*cur_top)++] = vm_tail++;
            }
            else if(i->op2 == lbool){
                    fscanf(inf, "%s", vm_strbuf);
                    if(strcmp(vm_strbuf, "true") == 0) {cur_stack[(*cur_top)++] = 1;}
                    else if(strcmp(vm_strbuf, "false") == 0) {cur_stack[(*cur_top)++] = 0;}
                    else {
                        fprintf(outf, "an invalid bool input\n");
                        cur_stack[(*cur_top)++] = 0;
                    }
            }
            else{}
            break;
        }
        case wrt:{
            // printf("wrt_debug:%d-%d\n", *cur_top, cur_stack[(*cur_top) - 1]);
            if(i->lval1 == 1){
                cur_stack[(*cur_top) - 1] = vm_stack[0][cur_stack[(*cur_top) - 1]];
            }
            if(i->op2 == lint){
                fprintf(outf, "%d", cur_stack[--(*cur_top)]);
            }
            else if(i->op2 == lbool){
                fprintf(outf, "%s", cur_stack[--(*cur_top)]? "true":"false");
            }
            else if(i->op2 == lstring){
                int p = cur_stack[--(*cur_top)];
                fprintf(outf, "%s", vm_heap[p]->str_data);
            }
            else if(i->op2 == pointer){
                fprintf(outf, "%d", cur_stack[--(*cur_top)]);
            }
            else{
                fprintf(outf, "try to print unsupport type\n");
            }
            break;
        }
        case cal:{
            cur_stack[(*cur_top)++] = vm_pc + 1;
            cur_stack[(*cur_top)++] = vm_base;
            cur_stack[(*cur_top)++] = get_base(i->op1);
            vm_base = vm_top[0];

            int* s1 = vm_stack[0], *s2 = vm_stack[1];
            int* p1 = &vm_top[0], *p2 = &vm_top[1];
            // printf("cal_debug:");
            while(*p2 > 0){
                s1[(*p1)++] = s2[--(*p2)];
                // printf("%d---", s1[*(p1)-1]);
            }
            // printf("\n");
            p_stk = 0;

            vm_pc = i->op2 - 1;
            
            break;
        }
        case ret:{
            /* 记录返回值和返回地址 */
            int ret_val = cur_stack[(*cur_top) - 1];
            bool is_str = i->op2 == lstring;
            vm_pc = cur_stack[vm_base+2] - 1;

            /* 清理过程中使用过的字符串空间 */
            int heap_p = cur_stack[vm_base+3];
            vm_heap_clear(heap_p, is_str?ret_val:-1, &vm_tail);
            if(is_str){
                ret_val = heap_p;
            }

            /* 修改栈顶指针和基址*/
            *cur_top = vm_base;
            vm_base = cur_stack[vm_base+1];
            cur_stack[(*cur_top)++] = ret_val;
            // printf("ret_debug:%d-%d-%d\n", *cur_top, vm_base, vm_pc);
            break;
        }
        case push:{
            *cur_top += i->op2;
            // fprintf(outf, "push_debug:%d\n", *cur_top);
            break;
        }
        case pop:{
            *cur_top -= i->op2;
            break;
        }
        case mov:{
            int* s1 = vm_stack[0], *s2 = vm_stack[1];
            int* p1 = &vm_top[0], *p2 = &vm_top[1];
            if(i->op1 == opmo){
                for(int ite = 0; ite < i->op2; ite++){
                    s2[(*p2)++] = s1[--(*p1)];
                }
                p_stk = 1;
            }
            else if(i->op1 == opmi){
                for(int ite = 0; ite < i->op2; ite++){
                    s1[(*p1)++] = s2[--(*p2)];
                }
                p_stk = 0;
            }
            else {}
            break;
        }
        case binop:{
            int a1, a2;
            // printf("bin_debug:%d-%d\n", cur_stack[(*cur_top) - 1], cur_stack[(*cur_top) - 2]);
            if(i->lval1 == 1){
                cur_stack[(*cur_top) - 1] = vm_stack[0][cur_stack[(*cur_top) - 1]];
            }
            if(i->lval2 == 1){
                cur_stack[(*cur_top) - 2] = vm_stack[0][cur_stack[(*cur_top) - 2]];
            }
            a1 = cur_stack[--(*cur_top)];
            a2 = cur_stack[--(*cur_top)];
            // printf("bin_debug:%d-%d\n", a1, a2);
            switch (i->op1){
                case opplus:
                    cur_stack[(*cur_top)++] = a2 + a1;
                    break;
                case opminus:
                    cur_stack[(*cur_top)++] = a2 - a1;
                    break;
                case optimes:
                    cur_stack[(*cur_top)++] = a2 * a1;
                    break;
                case opslash:
                    if(a1 == 0) fprintf(outf, "run time error: slash 0");
                    cur_stack[(*cur_top)++] = a2 / a1;
                    break;
                case opmod:
                    cur_stack[(*cur_top)++] = a2 % a1;
                    break;
                case opeq:
                    cur_stack[(*cur_top)++] = a2 == a1;
                    break;
                case opneq:
                    cur_stack[(*cur_top)++] = a2 != a1;
                    break;
                case oplss:
                    cur_stack[(*cur_top)++] = a2 < a1;
                    break;
                case opleq:
                    cur_stack[(*cur_top)++] = a2 <= a1;
                    break;
                case opgtr:
                    cur_stack[(*cur_top)++] = a2 > a1;
                    break;
                case opgeq:
                    cur_stack[(*cur_top)++] = a2 >= a1;
                    break;
                case opand:
                    cur_stack[(*cur_top)++] = a2 && a1;
                    break;
                case opor:
                    cur_stack[(*cur_top)++] = a2 || a1;
                    break;
                /* 字符串拼接 */
                case opscat:{
                    object *o1 = vm_heap[a2], *o2 = vm_heap[a1], *obj = (object*)malloc(sizeof(object));
                    obj->str_len = o1->str_len + o2->str_len;
                    obj->str_data = (char*) malloc(obj->str_len + 1);
                    strcpy(obj->str_data, o1->str_data);
                    // printf("%d, %s\n", a2, o1->str_data);
                    strcpy(obj->str_data+o1->str_len, o2->str_data);
                    // printf("%d, %s\n", a1, o2->str_data);
                    vm_heap[vm_tail] = obj;
                    cur_stack[(*cur_top)++] = vm_tail++;
                    break;
                }
                /* 字符串相等与不相等判断 */
                case opseq:{
                    object *o1 = vm_heap[a2], *o2 = vm_heap[a1];
                    bool ret = strcmp(o1->str_data, o2->str_data) == 0;
                    cur_stack[(*cur_top)++] = ret;
                    break;
                }
                case opsneq:{
                    object *o1 = vm_heap[a2], *o2 = vm_heap[a1];
                    bool ret = strcmp(o1->str_data, o2->str_data) != 0;
                    cur_stack[(*cur_top)++] = ret;
                    break;
                }   
                /* 字符串幂 */
                case opspow:{
                    object *o1 = vm_heap[a2], *obj = (object*)malloc(sizeof(object));
                    obj->str_len = o1->str_len * a1;
                    obj->str_data = (char*) malloc(obj->str_len + 1);
                    for(int i = 0; i < a1; i++){
                        strcpy(obj->str_data+i*o1->str_len, o1->str_data);
                    }
                    vm_heap[vm_tail] = obj;
                    cur_stack[(*cur_top)++] = vm_tail++;
                    break;
                }
                default:
                    break;
            }
            break;
        }
        case unaop:{
            switch(i->op1){
                case opinc:
                    vm_stack[0][cur_stack[(*cur_top) - 1]] += 1;
                    break;
                case opdec:
                    vm_stack[0][cur_stack[(*cur_top) - 1]] -= 1;
                    break;
                case opnot:
                    if(i->lval1 == 1){
                        cur_stack[(*cur_top) - 1] = vm_stack[0][cur_stack[(*cur_top) - 1]];
                    }
                    cur_stack[(*cur_top) - 1] = !cur_stack[(*cur_top) - 1];
                    break;
            }
            break;
        }
        case snew:{
            object* obj = (object *)malloc(sizeof(object));
            
            int a1;
            if(i->lval1 == 1){
                cur_stack[(*cur_top) - 1] = vm_stack[0][cur_stack[(*cur_top) - 1]];
            }

            switch(i->op2){
                case lstring:{
                    if(i->str != NULL){
                        obj->str_data = (char*) malloc(strlen(i->str)+1);
                        strcpy(obj->str_data, i->str);
                        obj->str_len = strlen(obj->str_data);
                        vm_heap[vm_tail] = obj;
                        cur_stack[(*cur_top)++] = vm_tail++;
                    }
                    else{
                        int adr = cur_stack[(*cur_top) - 1];
                        obj->str_data = (char*) malloc(strlen(vm_heap[adr]->str_data)+1);
                        strcpy(obj->str_data, vm_heap[adr]->str_data);
                        obj->str_len = strlen(obj->str_data);

                        vm_heap[vm_tail] = obj;
                        cur_stack[(*cur_top) - 1] = vm_tail++;
                    }
                    break;
                }
                case lint:{
                    obj->str_data = (char*) malloc(MAX_STR_LEN);
                    sprintf(obj->str_data, "%d", cur_stack[(*cur_top) - 1]);
                    obj->str_len = strlen(obj->str_data);

                    vm_heap[vm_tail] = obj;
                    cur_stack[(*cur_top) - 1] = vm_tail++;
                }
                default:
                    break;
            }
            
            // printf("snew_debug:%s-%s-%d\n", i->str, vm_heap[vm_tail-1]->str_data, vm_tail-1);
            break;
        }
        case reg:{
            cur_stack[(*cur_top)++] = vm_tail;
            // printf("reg_debug:%d\n", cur_stack[(*cur_top)-1]);
            break;
        }
        default:{
            break;
        }
    }
    vm_pc ++;
}

void vm_execute(FILE* inf, FILE* outf){
    while(vm_pc < vm_code_cnt){
        // printf("%d\n", vm_pc);
        vm_step(inf, outf);
    // printf("%d--%d\n", vm_stack[0][21], vm_top[0]);
    }
}