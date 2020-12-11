C++模板{#cpp_template}
====================

\subpage cpp11_common_type

\section 定义模板函数

\section 定义模板类

\subsection 模板类中的模板成员函数

有的时候，我们想要模板类A<T>的成员函数中，让另一个模板类B，使用当前类的类型B<T>，即模板类型的传递，这是否可行呢？

答案是可行的，具体做法如下实例代码所示：

\code{.cpp}
#include <cstdio>

template<typename T>    // 必须为模板类，否则报错
class B;                // 声明必须存在，否则报错

template<typename T>
struct A{
    T x;
    B<T> toB(){
        B<T> b;
        b.y = x * 10;   // 罕见的，B::y的定义可以放在后面
        return b;
    }
    B<T> toC(){
        return 0.0/0.0;
    }
};

template<typename T>
struct B{
    T y;
};

int main(){
    A<float> a = {1.0};
    B<float> b = a.toB();
    printf("b.y=%.f", b.y);
}

\endcode

运行结果：

\code
b.y=10 
\endcode

这里有意思的是，在模板函数A::toB()中，使用了模板类B的成员变量y，而此时，y是还“没有”定义的，因为它的定义在下方。

但是模板类B的声明却必须先“存在”。

如果把这些模板类换成普通的类型，编译就会报错。

具体的原因涉及到模板编译的过程，以后有时间再仔细研究吧。

\section 模板类的继承

\section 模板类的常见误区