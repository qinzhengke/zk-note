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