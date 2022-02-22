C语言中的函数{#c_function}
=======================

# 可变长参数函数

## 可变长参数函数的转发

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


## 函数指针常量怎么定义？
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


## 函数指针作为函数传参

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


## 二维数组的传参和“引用”
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


# 函数的返回值


## c_return_array 返回数组

首先，C语言是不能直接返回数组的，但是可以选择以指针的形式返回数组的首地址。

但是要注意，不能返回函数内部临时数组，因为这些数组在函数返回之后声明周期就结束了，其对应的内存已经释放，如果再次使用，有可能会数据的非法访问甚至篡改。

虽然这些地址仍然有可能访问，甚至值仍然是正确的。

\code{cpp}
#include <stdio.h>

char * func(){
    char *s = "abc";
    printf("%p",s);
    return s;   // 错误的做法
}

char * func2(){
    char s[10];
    sprintf(s, "%s", "edf");
    return s;
}

int main()
{
    char * s = func();
    printf("%p,%s\n", s, s);
    char * s2 = func2();
    printf("%p,%s\n", s2, s2);
}

\endcode

运行结果：

\code{bash}
0x400704
0x400704,abc
0x75f07da87620
0x75f07da87620,def
\endcode

由此可见，虽然func()中的s="abc"，以及func2()中的s[10]，是在栈上开辟的内容，虽然退出后仍可访问并且内容仍然正确，但是这部分内容已经被释放，有可能被其他程序占用。

可以使用以下几种方式“返回”数组：

1. 使用malloc()在堆上建立数组并返回，但是要记得free()，
2. 使用static修饰符在堆上建立数组并返回
3. 使用传参的方式将数组传进来并修改。