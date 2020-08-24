C++ debug教训{#cpp_debug}
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

