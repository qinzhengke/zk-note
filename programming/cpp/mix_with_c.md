C++与C语言混合编程{#cpp_mix_with_c}
================================

<hr>
\section C语言调用C++模块

<hr>
\subsection C语言调用C++函数的正确姿势
大家都知道，C++调用C语言模块很容易，就是在函数生命的地方使用#ifdef \_\_cplusplus extern "C" #endif 包起来就行。
但是有的时候需要在C语言环境中调用C++的模块，虽然这种调用感觉很不合理，但是仍然不可避免，例如组里的顶层框架代码是C，我们自身的模块是C++，这时候必须要把C++模块塞到C模块里。
首先，c++模块头文件需要把函数声明用extern "C"包裹，
其次，c模块在调用之前，使用extern重新声明该函数，相当于声明了两遍。另外c模块绝对不要include该c++模块的头文件，否则会报错。
最后，c模块正常调用c++模块函数。

其实到这里，我并不太理解为什么第一步要做，如果第二步直接重新声明了c++函数，并且压根没有include c++模块的头文件，那调用过程根本和c++头文件没有关系。
然而实际测试时，如果c++头文件去掉了extern "C"，还是会出现C linkage的问题。
这说明，extern "C"的包裹，虽然只是针对声明进行的，但是它会实际影响到定义部分，会让定义部分使用C的方式生成函数符号名。
那最后留一个疑问，如果extern "C"包裹的C++函数有重载，那么会发生什么呢？

\endcode
//C++头文件 cppExample.h
#ifndef CPP_EXAMPLE_H
#define CPP_EXAMPLE_H
extern "C" int add( int x, int y );
#endif

//C++实现文件 cppExample.cpp
#include "cppExample.h"
int add( int x, int y )
{
　return x + y;
}

/* C实现文件 cFile.c
/* 这样会编译出错：#include "cExample.h" */
extern int add( int x, int y );
int main( int argc, char* argv[] )
{
　add( 2, 3 );
　return 0;
}
\endcode
