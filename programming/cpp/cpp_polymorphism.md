C++中的多态{#cpp_polymorphism}
============================

\section 如何优雅地定义没有成员函数的类型为虚类？

\code{.cpp}
// base.hh
class Base
{
public:
    virtual ~Base() = 0;
    ...
};

// base.cc
Base::~Base() {}
\endcode

注意，析构函数不要定义在头文件中，否则链接的时候会报redefinition错误。

<hr>
\section 如何优雅的拷贝虚类对象？

往往我们拿到的对象指针是虚类，要拷贝虚类对象，直接调用虚类的拷贝构造函数是不行的，优雅的方法是在虚类中定义一个clone()函数，让派生类去实现这个函数。

\code{.cpp}
class Base {
public:
  virtual Base* Clone() = 0;
};

class Derivedn : public Base {
public:
  //This is OK, its called covariant return type.
  Derivedn* Clone() {
    return new Derivedn(*this);
  }
private:
  Derivedn(const Derivedn) : ... {}
};

\endcode