C语言中的函数{#c_function}
=======================

\section 可变长参数函数

\subsection 可变长参数函数的转发

前言：函数A接收可变长参数，有的时候我们希望把可变长的参数直接传入函数A内部的sprintf和printf，
例如我既想把输出保存起来，也想同时打印到屏幕，我姑且称之为“转发”，该如何操作呢？

方法：如下代码所示，vprintf也可以改成vsprintf、vfprintf。

\code{cpp}
#include <stdarg.h>
#include <stdio.h>

void Error(const char* format, ...){
    va_list argptr;
    va_start(argptr, format);
    vprintf(format, argptr);
    va_end(argptr);
}

int main(){
    int code = -1;
    Error("This is an error, code:%d!\n", code);
}

\endcode

<hr>
\subsection 函数指针常量怎么定义？
\code{c}
// normal pointer to function
int (*func)(int);

// pointer to const function -- not allowed
int (const *func)(int);

// const pointer to function. Allowed, must be initialized.          
int (*const func)(int) = some_func;

// Bonus: pointer to function returning pointer to const
void const *(*func)(int);

// triple bonus: const pointer to function returning pointer to const.
void const *(*const func)(int) = func.
\endcode

<hr>
\subsection 函数指针作为函数传参

\code{c}
typedef void (*callback_function)(void); // type for conciseness

callback_function disconnectFunc; // variable to store function pointer type

void D::setDisconnectFunc(callback_function pFunc)
{
    disconnectFunc = pFunc; // store
}

void D::disconnected()
{
    disconnectFunc(); // call
    connected = false;
}
\endcode

<hr>
\subsection 二维数组的传参和“引用”
\code{cpp}
void func_a(int a[][10])
{
...
}
// 或者
void func_b(int (*a)[10])
{
...
}

void main()
{
  int a[5][10];
  int (*b)[10] = a;
}
\endcode
注意：第二种方式中的`*b`必须使用括号括起来，否则就变成了一维的数组指针了。

【疑问】感觉二维数组的坑很多，函数传参有特殊语法，后期和其他扩展类型协同时也存在问题，例如Eigen::Matrix、JSON的数组等，还是用一维数组比较省事，自行映射理论上耗时也是一致的。