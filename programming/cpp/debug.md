C++调试问题{#cpp_debug}
========================


\section 企图通过memcpy对象进行拷贝操作。

类成员对象里面如果有容器类型，就会出现coredump，具体原因待研究。

\section 模板定义中的typename和int不要搞混

模板定义中，有类型模板，也有大小模板，两者不要搞混了，在多重模板调用的时候，如果一个typename类型给了int类型，是无法编译通过的。

\code{.cpp}
template<typename T, int N>
class A{

}
\endcode


\section lvalue_bind_rvalue cannot bind non-const lvalue reference of type xxx to an rvalue of type xxx.

非常量引用一般是为了修改输入变量，但是如果输入进来的是临时变量（rvalue），那么函数结束后，临时变量会消失，这种改动就会毫无意义，编译器为了避免程序员犯错，会友好的报出编译错误。

\section dynamic_poiner_cast_debug dynamic_pointer_cast模板类要正确

\code{.cpp}
class Base {
public:
    Base (){};
protected:
    inline virtual int getInt () { return 0; }
};

class Derived : public Base {
protected:
    inline int getInt ( ) override { return 1; }

};

typedef std::shared_ptr<Base> spBase;
typedef std::shared_ptr<Derived> spDerived;

\endcode

这时候如果这样操作，则会报错

\code{.cpp}
spDerived derived = static_pointer_cast < spDerived > ( base );
\endcode

这里要注意的是，模板参数是派生类，而不是派生类的shared_ptr，千万别搞错了！

这样才是正确的

\code{.cpp}
spDerived derived = static_pointer_cast <Derived> ( base );
\endcode


\section　初始化的重要性
一个未初始化的变量，可能导致算法的不正确，要debug很久。

debug方法论，一定要全体打印，不要先入为主地认定某些东西一定正确！


\section 定义函数时命名空间使用

定义函数时，即使一开始使用了using namespace，在函数定义处仍然需要使用命名空间+"::"形式进行修饰。
实际上“using namespace”只是在使用类型时缺省命名空间，和定义是毫无关系的。

下面的代码会让系统无法区分到底要调用哪一个目标函数。

\code{.cpp}
#include <stdint.h>
#include <stdio.h>

namespace n1{
void func();
}

void func(){
    printf("This is func\n");
}

void n1::func(){
    printf("This is func in n1\n");
}

using namespace n1;

int main()
{
    func(); // 
    n1::func();
}

\endcode

运行结果：

\code{.sh}
 In function 'int main()':
20:10: error: call of overloaded 'func()' is ambiguous
20:10: note: candidates are:
8:6: note: void func()
12:6: note: void n1::func()
\endcode