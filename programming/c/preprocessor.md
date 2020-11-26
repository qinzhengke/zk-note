C语言的预处理器{#c_preprocessor}
=============================

<hr>
\section sharp_in_macro 宏定义中的\#

“\#”（stringizing）是字符串化操作符。其作用是：将宏定义中的传入参数名转换成用一对双引号括起来参数名字符串。
其只能用于有传入参数的宏定义中，且必须置于宏定义体中的参数名前。

如：

\code{cpp}
#include <stdio.h>

#define PRINT(instr) printf("the input string is:%s\n",#instr)
#define CONVERT(instr) #instr

int main(){
    PRINT(abc); // 在编译时将会展开成：printf("the input string is:/t%s/n","abc");
    PRINT( abc );
    PRINT( abc def );
    char * str=(char*)CONVERT(abc); 
    printf("str=%s\n", str); // char * str= (char*)"abc"；
}
\endcode

运行结果为：
\code{bash}
the input string is:abc
the input string is:abc
the input string is:abc def
str=abc
\endcode

【注意】预处理器对于对空格的处理逻辑如下：

1. 忽略传入参数名前面和后面的空格。

   如：str=FUNC1(   abc )； 将会被扩展成 str="abc"；

2. 当传入参数名间存在空格时，编译器将会自动连接各个子字符串，用每个子字符串中只以一个空格连接，忽略其中多余一个的空格。

   如：str=exapme( abc    def); 将会被扩展成 str="abc def"；

<hr>
\section double_sharp 宏定义中的\#\#

“\#\#”的名称是（token-pasting）符号连接操作符，其作用是将两个宏字符串拼接在一起。

【问题】两个宏字符串拼在一起有什么用处？

其中一个用处就是能够为变量提供统一的前后缀，例如想要给x,y,z变量都加上前缀“a_”，直接写在代码里，会重复很多次。
而且哪一天需要改前缀的名称时，需要修改很多处，此时可以使用\#\#来给变量自动加上前缀，具体代码如下所示。

\code{cpp}
#include <stdio.h>
#define PREFIX_A(n) a_##n
#define PREFIX_B(n) b_ ## n

int main(){
    int PREFIX_A(x) = 10;
    int PREFIX_A(y) = 20;
    int PREFIX_B(x) = 30;
    printf("a_x=%d, a_y=%d, b_x=%d\n", a_x, a_y, b_x);
}
\endcode

【注意】

1.当用\#\#连接形参时，\#\#前后的空格可有可无。

2.连接后的实际参数名，必须为实际存在的参数名或是编译器已知的宏定义。


<hr>
\section 检测当前操作系统

\code{cpp}
#ifdef __linux__ 
    //linux code goes here
#elif _WIN32
    // windows code goes here
#else
    cout<<"OS not supported!"<<endl;
#endif
\endcode