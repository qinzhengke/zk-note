C语言的预处理器{#c_preprocessor}
=============================


# sharp_in_macro 宏定义中的\#

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


# double_sharp 宏定义中的\#\#

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



# 检测当前操作系统

\code{cpp}
#ifdef __linux__ 
    //linux code goes here
#elif _WIN32
    // windows code goes here
#else
    cout<<"OS not supported!"<<endl;
#endif
\endcode


# 宏在c文件中的作用域

宏定义有时候不一定会在c文件中起作用，如下面的例子所示。
main.c依赖a.h和a.c，而a.h又依赖b.h，所以我们自然而然的认为在main.c里包含a.h之前加入某一个宏，它能够在a.h和b.h都生效。
在头文件中，这是没有问题的，但是对于b.c文件，就不管用了（实际工作碰到的例子），因为b.h和b.c在编译的时候他们并不依赖其他模块，所以有可能b模块比main编译的时刻还要早（编译成obj文件），另外，如果仅仅是修改了宏，a.h和a.c的内容不会改变，那么a.o根本就不会重新发生编译。

\code{cpp}
// main.c
#define TURN_OFF_DEVIL
#include <a.h>

// a.h
#include <b.h>
#ifndef TUNN_OFF_DEVIL
static int i_am_devel_1;
#endif

// b.h
// b.c
#include <b.h>
#ifndef TURN_OFF_DEVIL
int i_am_devel2;
#endif
\endcode
解决方法，使用编译工具的宏定义工具，例如cmake的
\code{c}make
target_compile_definitions(my_exe PRIVATE USE_SOMTHING=1)
\endcode

C语言中的宏{#c_macro}
==================




# 获取宏的参数个数

\code{.cpp}
#include <iostream>
#include <cmath>

#define GET_NTH_ARG(                                                                        \
    _1,  _2,  _3,  _4,  _5,  _6,  _7,  _8,  _9,  _10, _11, _12, _13, _14, _15, _16,         \
    _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32,         \
    _33, _34, _35, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, _47, _48,         \
    _49, _50, _51, _52, _53, _54, _55, _56, _57, _58, _59, _60, _61, _62, _63, _64, n, ...) n

#define GET_ARG_COUNT(...) GET_NTH_ARG(__VA_ARGS__,                     \
        64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, \
        48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, \
        32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, \
        16, 15, 14, 13, 12, 11, 10, 9,  8,  7,  6,  5,  4,  3,  2,  1)

using namespace std;

int main()
{
    int a=1,b=2,c=3;
    cout<<GET_ARG_COUNT(a,b,c);
}

\endcode

解释一下，GET_ARG_COUNT(a,b,c)被展开为GET_NTH_ARG(a,b,c,64,63 ... 3,2,1)，其中3刚好被推到了GET_NTH_ARG()宏的参数"n"的位置。
从而输出了宏参数的个数。

一般来说，这样的宏没什么用处，但是有的时候可以用于C++的静态反射，可以参考
<a href="https://netcan.github.io/2020/08/01/%E5%A6%82%E4%BD%95%E4%BC%98%E9%9B%85%E7%9A%84%E5%AE%9E%E7%8E%B0C-%E7%BC%96%E8%AF%91%E6%9C%9F%E9%9D%99%E6%80%81%E5%8F%8D%E5%B0%84/">如何优雅的实现 C++ 编译期静态反射</a>