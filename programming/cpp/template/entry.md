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

\section 模板的分离式编译

模板分离式编译技术能够避免方法的实现放在头文件中，有以下好处：

1. 加快编译速度，
2. 避免将实现暴露给用户，减少用户查看头文件的负担，也可以实现保密。

具体方法如下代码所示，即枚举可能实例化的类型，然后预先实例化。
当然这种方法只对一些原生类型能用，因为原生类型的数量少，是能够枚举出来的。
如果类型是自定义的类，则无法做到。

下面给出了三种形式的模板在源文件中实例化的案例，分别是模板函数、模板类、以及类中的模板函数成员。

\code{cpp}
// module.hh
// 模板函数
template<typename T>
T add(T a, T b);

// 模板类
template<class T>
struct A{
    T add(T a, T b);
};

// 类成员模板函数
struct B{
    template<typename T>
    T add(T a, T b);
};
\endcode

\code{cpp}
// module.cc
#include "module.hh"

template<typename T>
T add(T a, T b){
    return a+b;
}

template float add<float>(float a, float b);
template double add<double>(double a, double b);


template<class T>
T A<T>::add(T a, T b){
    return a+b;
}

template float A<float>::add(float a, float b);
template double A<double>::add(double a, double b);

template<typename T>
T B::add(T a, T b){
    return a+b;
}

template float B::add(float a, float b);
template double B::add(double a, double b);
\endcode

\code{cpp}
// main.cc
#include <cstdio>
#include "module.hh"

int main(void){
    float x=1.0, y=2.0;
    double z=3.0, w=4.0;
    printf("add<float>(): %.2f\n", add<float>(x,y));
    printf("add<double>(): %.2f\n", add<double>(z,w));

    A<float> af;
    printf("a<float>.add(): %.2f\n", af.add(x,y));
    A<float> ad;
    printf("a<double>.add(): %.2f\n", ad.add(z,w));

    B b;
    printf("b.add<float>(): %.2f\n", b.add<float>(x,y));
    printf("b.add<double>(): %.2f\n", b.add<double>(z,w));
}

\endcode


\section 模板类的继承

\section 模板类的常见误区