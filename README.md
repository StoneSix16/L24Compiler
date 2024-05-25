# L24Compiler
A compiler and interpreter based for L24 Language. 一个L24语言的编译解释程序。

## 语法
### ENBF
```
<program> = "main" "{" <stmt-list> "}"
<proc> ::= "("[<param-list>]")" "{"<stmt-list> <ret>"}"
<ret> ::= "return" <expr> ";"

<stmt-list> ::= {<stmt>}
<stmt> ::= <decl_stmt> ";"
         | <expr-stmt> ";"
         | <if-stmt> ";"
         | <while-stmt>
         | <for-stmt>
         | <scan-stmt> ";"
         | <print-stmt> ";"

<type-specifier> ::= <type>{<pointer>} 
<pointer> ::= "*"{<pointer>}
<type> ::= "string"
         | "int"
         | "bool"

<declarator> ::=  <ident>{<arr-dim>}
<arr-dim> ::= "["<number>"]"{<arr-dim>}
<decl-stmt> ::= <type-specifier> <declarator>
              | <type-specifier> <declarator> <proc>

<arr-idx> ::= "["<expr>"]"{<arr-idx>}

<param-list> ::= <decl-stmt>
               | <param-list> {"," <param-list>}
<arg-list> ::= <expr>
             | <arg-list> "," <expr>

<expr-stmt> ::= <expr> "=" <expr> | <expr>
<if-stmt> ::= "if" "("<expr>")" "then" "{"<stmt-list>"}" "end"
            |"if" "("<expr>")" "then" "{"<stmt-list>"}" "else" "{"<stmt-list>"}" "end"
<while-stmt> = "while" "("<expr>")" "{"<stmt-list>"}"
<for-stmt> = "for" "(" [<expr-stmt>]{,<expr-stmt>}";" [<expr>]";" [<expr>]{,<expr>}")" "{"<stmt-list>"}"
<scan-stmt> = "scan" "(" <ident> {"," <ident>} ")"  
<print-stmt> = "print" "(" <expr> {"," <expr>} ")" 

<expr> ::= <or-expr>
<or-expr> ::= <and-expr>
            | <or-expr> "||" <and-expr>
<and-expr> ::= <cmp-expr>
             | <and-expr> "&&" <cmp-expr>
<cmp-expr> ::= <add-expr>
             | <cmp-expr> ("=="|"!="|"<"|"<="|">"|">=") <add-expr>
<add-expr> ::= <multi-expr>
             | <add-expr> ("+"|"-") <multi-expr>
<multi-expr> ::= <unary-expr>
               | <multi-expr> ("*"|"/") <unary-expr>
<unary-expr> ::= <pre-expr>
               | "!" <pre-expr>
<pre-expr> ::= <post-expr>
               | ("++"|"--"|"+"|"-") <term>
<post-expr> ::= <term>
               | <post-expr> ("++"|"--")
<term> ::= <factor>
         |("*"|"&") <term> 
<factor> ::= "("<expr>")"
         | <ident> {<arr-idx>}  
         | <ident> "("[<arg-list>]")"
         | <literal>

<literal> ::= <boolean> | <number> | <string>
```

### 语法特点
- 支持字符串，整形和布尔基本类型。
- 支持指针和多维静态数组。
- 支持最大3层的过程嵌套，过程支持参数值传递，必须有返回值(因为没有定义void类型)。
- 实现指针和数组的隐式类型转换

## 存储形式

解释程序建立两个栈S1, S2，指令数组Code和一个字符指针数组Strs。S1储存程序执行产生的变量，S2用于计算数组下标索引和参数压栈，Strs用于储存出现的字符串，此外还有一个指针p用于指示当前操作的栈。

程序变量可以是指针，数组，整型，布尔值或字符串。栈中的指针，整型，布尔值和字符串都用int32存储，其中字符串的值表示其在Strs中的下标索引。数组对应栈中一段连续的空间，数组类型变量无法赋值，作为实参传递或被设置为函数返回值。

栈中的值可分为左值和右值，左值元素的内容是值所在的绝对地址，右值元素的内容是值本身。引入左值和右值使得取地址、数组赋值和解引用操作更加方便。可以通过lod将左值变为右值，不过为了减少生成的指令数，在生成指令时用额外的值告知了解释程序如何读取栈元素。

在每次函数调用结束后，Strs需要释放不再使用的字符串空间。每次函数调用前，解释程序将当Strs的尾指针tail连同实参一并压入栈中，函数返回后，解释程序释放tail以后的字符串指针。如果函数的返回值是一个字符串，则将其保留在尾指针处。

## 指令设计
### 指令集
L: 层差，当前层与标识符所在层的差值；L=-1表示使用绝对地址

A：指令地址或立即数

op：运算类型

islval1: 栈顶元素为左值

islval2: 次栈顶元素为左值

- ADDR L 0

  层差为L，使用栈顶元素为地址偏移，计算其在栈中的绝对位置。
  栈元素不变。
- LOD L 0

  层差为L，使用栈顶元素为地址偏移，将对应存储单元存入栈顶。
  栈元素不变。
- STO L 0

  层差为L，使用次栈顶元素为地址偏移，将栈顶元素存入对应存储单元。
  栈元素-2。
- CAL L A

  依次压入RA，DL，SL元素，弹出当前栈内所有元素至另一个栈。最后跳转至指令地址A。
  当前栈元素清空，另一个栈元素增加对应数量。
- RET 0 type

  弹出三个元素，根据返回地址从函数返回。
  栈元素-3。
- JMP 0 A

  无条件跳转至指令地址A
  栈元素-1。
- JPC 0 A

  条件跳转，若栈顶值为0，则跳转至指令地址A
  栈元素-1。
- LIT 0 A

  将立即数A存入栈顶
  栈元素+1。
- SNEW 0 type str

  将字符串str存入堆，将str的地址存入栈顶
  栈元素+1。

  如果str为空指针，则根据type读取栈顶元素并生成字符串，修改栈顶元素为复制串的地址
  栈元素不变。
- REG 0 0

  压入vm_tail寄存器的值
  栈元素+1
- BINOP op 0

  同类型元素执行二元运算，包括算术运算和逻辑比较和运算
  栈元素-1。
- UNAOP op 0

  同类型元素执行一元运算，包括算术运算，逻辑非，取地址和解引用
  栈元素不变。
  解引用和自增运算返回左值形式。
- WRT 0 type

  根据type读取栈顶元素并输出
  栈元素-1。
- SCN 0 type

  根据type解析输入文本并将值存放在栈顶元素指示的绝对地址
  栈元素+1。
- PUSH 0 size

  压入size个元素
  栈元素+size。
- POP 0 size

  弹出size个元素
  栈元素-size。
- MOV op size

  弹出size个元素并储存在另一个栈中，op决定弹出方向，接受弹出元素的栈成为接下来被操作的栈。
### 指令生成

代码生成的处理总体上类似于pl0编译器，在某些方面有部分改动。

#### 语句的代码生成

- 声明：由于执行语句可能在声明语句之前，编译程序不在执行过程前为所有变量开辟空间，而是在每一条声明后通过push开辟相应的所需空间。需要注意的是，定义过程时的参数声明不会生成push指令，实参会在过程被调用时压入栈中。

#### 表达式的代码生成
在生成代码前，要先检查表达式的语义是否合法，并检查项是否为左值。

- 变量访问：编译程序首先查询变量在符号表中的位置，获取层差和相对基地址的偏移，生成计算绝对地址的代码，栈顶元素为变量的绝对地址。

- 指针与数组转换：类似c程序，如果一个数组类型的变量的索引小于其声明时的维度，该变量被看作一个指针；如果一个指针类型的变量有一维索引，该变量被看作一个数组并读取相应位置的值。

- 字面量：如果是布尔或整型，则直接在栈中压入相应常数；如果是字符串，则先在Strs上添加新的指针，再将指针的下标压入栈中。

- 解引用和取地址：解引用只需要修改指针层数或使用lod读取值。取地址只能在左值上进行，只需要将栈元素修改成右值类型，不需要生成代码。

- 自增运算：自增只能在左值上运行。前缀自增的结果是左值类型，后缀自增的结果是右值类型。

#### 过程（函数）的代码生成

- 函数调用：调用函数时，会先将实参按定义顺序压入S1，然后通过mov将实参与tail转移到S2。随后生成cal指令，将实参和其他信息从S2转移回S1。cal执行后，S1中从基址至栈顶依次为SL，DL，RA，按定义顺序排列的实参。实参均为右值类型。

  如果形参是字符串，若实参为字面量，则先更新Strs，再将下标作为实参传递；若实参为变量，则在Strs中复制相应的字符串，并将新指针的下标作为实参传递。
  
  如果形参是数组，数组在实参传递过程会转换为指针，因此函数对数组的修改会直接作用在传递的实参上，导致函数外变量内容的改变。
  
  如果函数内的变量（参数和局部变量）与函数外变量重名，访问时会优先访问函数内变量。

- 函数返回：函数执行完毕后，生成ret指令将实参和其他信息弹出，修改pc指针的值并将返回值保存在栈顶。如果不使用函数的返回值，解释程序会生成一条pop指令。

- 函数嵌套：由于声明和执行语句没有明确先后顺序，在生成执行语句的中间代码后可能遇到新的函数声明。为此，编译程序将函数中函数声明部分的代码放在函数体指令的前半部分，将函数中其他的声明和执行语句的指令放在后半部分。具体说，编译程序使用一个buffer储存生成的代码，并标记每个函数体的开始指令地址buf_pc，并在生成ret指令后，将当前buffer中从buf_pc到末端的指令转移到Code中。

## 语义分析
### 类型与函数描述
使用枚举id_type存储基本类型
```
enum id_type {
    lint, lbool, lstring, // 可用于字面量
    pointer, array, proc
};
```
为了分析表达式的类型，使用结构体type_desp描述一个表达式的类型
```
/* 类型的描述符，针对指针和数组 */
typedef struct type_description{
    int t; /* 自身的类型，可能是id_type中除proc的一种 */
    int shape[MAX_ARR_DIM]; /* 数组的形状或索引 */
    int dim; /* 数组的维度或指针的层数 */
    struct type_description* member_t; /* 指针/数组装载对象的类型 */
} type_desp;
```
另外，为了描述作为函数或函数参数的标识符的信息，使用了结构体proc_desp描述函数的信息
```
/* 函数的描述符 */
typedef struct proc_description{
    bool is_param; /* 是否是函数参数 */
    args_s* param_info; /* 参数列表信息 */
    type_desp* ret_type; /* 返回值类型 */
} proc_desp;
```
### 语义值类型
不同文法符号有不同的语义值类型
```
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
```
### 符号表
符号表结构如下
```
/* 符号表结构 */
struct table_item {
    char name[MAX_ID_LEN];     /* 标识符 */
    type_desp *type;  /* 变量类型 */
    proc_desp *proc_info; /* 如果不是函数或函数参数，则为NULL */
    int level; /* 层次 */
    int addr;  /* 标识符偏移地址或函数入口 */
    int size; /* 占用的空间，函数的size为函数参数列表占用的空间 */
};
```
### 语义计算

表达式的语义值计算包括类型和左值计算。左值计算规则如下
- 变量表达式一定为左值
- 函数参数，返回值一定为右值
- 一元运算中，前缀自增运算，指针解引用为左值，其余为右值。另外，自增运算，取地址要求对象必须为左值。
- 二元运算结果均为右值
- scan语句中的表达式必须为左值

数组索引的语义值在每次分析到新维度时更新

参数列表的语义值在每次分析到新参数时更新

## 使用

1. 生成可执行文件
```
git clone https://github.com/StoneSix16/L24Compiler.git
cd ./L24Compiler/
make
```

2. 使用命令行执行程序
```
L24Parser.exe <source code file>
```
输入``e``，执行整个测试程序
输入``s``，按步执行一条指令
输入``1~4``，打印当前的堆栈信息
输入``q``，退出L24Parser

3. 使用GUI执行程序（TODO）
```
python GUI.py
```



