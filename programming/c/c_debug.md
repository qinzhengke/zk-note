C语言调试问题{#c_debug_lesson}
======================


# printf打印64位整型

不要使用"%d"或者"%u"，这会使得printf只输出变量的低位。

一定要使用"%lu"和"%ld"进行打印。

别看这错误很小，但是可能让你很长一段时间以为自己的数据来源错了。

# float或者double类型打印

默认的"%f"只显示前6个有效数字，某些时候会截断我们的数值，当我们数字数值较大，且精度要求较高时，一定要注意这点。


# 函数对象（指针）作为实参的的时候一定要其形参是否正确。

libevent库中的evtimer_set宏接收3个参数，如下所示，第一个参数是struct event 对象，第2个参数是回调函数，第三个是万能参数。

\code{.cpp}
#define evtimer_set(ev, cb, arg)	event_set((ev), -1, 0, (cb), (arg))
\endcode

这里要注意两点，本身宏的参数要正确传递，同时回调函数的形参本身也要定义正确，因为这个evtimer_set宏对回调函数是有要求的，回调函数一定要有这样的形式，如下所示。

\code{.cpp}
void (*)(evutil_socket_t, short, void *)
\endcode

总结：函数对象（指针）作为实参时，一定要注意函数对象（指针）的形参是否正确。

另外要认真看错误提示，看看提示的是外函数参数不正确，还是函数对象本身参数不正确。

要多多使用函数对象，或者函数指针。


# 头文件循环引用

如果存在头文件循环引用，A.h用了类型B.h的B，而B也用了A.h的类型A，那么将会导致的问题：有一个类型将会未定义。
如下代码所示，引用的链为A.h->B.h->A.h...，实际上，由于头文件一开始定义了ifndef，导致头文件在一次.o编译中只会被展开一次。
所以实际的链为A.h->B.h，在B.h中类型A是没有定义的，因为A.h的第一句话就是展开B.h。

\code{.cpp}
// A.h
#include "B.h"
struct A{int a;}
B getB();

// B.h
#include "A.h"
struct B{int b;}
A getA();
\endcode

解决方法一：在一个公共头文件中定义类型A和类型B，

解决方法二：在A.h中声明class B，在B.h中声明class A，当然这种方式是不能引用成员变量的。

\code{.cpp}
// A.h
#include "B.h"
struct A{int a;}
B getB();

// B.h
#include "A.h"
struct B{int b;}
A getA();
\endcode


# 操作符优先级导致的bug

\code{.cpp}
uint8_t a = 0x0;
if(a & 0x01 == 0){
    printf("a[0]=0\n");
}
\endcode

&的优先级比==要低，所以0x01==0会被先执行，为false，即0，最后整个结果输出为0。

有的编译器会直接优化这种恒为0的代码，所以单步调试的时候，这段代码会被跳过。