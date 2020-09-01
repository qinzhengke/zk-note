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

<hr>
\section 抽象类的构造函数不能运行成员虚成员函数

其实原因很简单，当派生类在构造过程中，自己到底是什么类型是不知道的，从而不知道要调用那种类型的成员函数。
只有构造完毕才能知道，实际上虚函数表指针也是在构造函数中进行赋值的。

\code{.cpp}
struct Abstract {
    virtual void pure() = 0;
    virtual void foo() {
        pure();
    }
    Abstract() {
        foo();
    }
    ~Abstract() {
        foo();
    }
};

struct X : public Abstract {
    virtual void pure() { cout << " X :: pure() " << endl; }
    virtual void impure() { cout << " X :: impure() " << endl; }
};
int main() {
    X x;
}
\endcpp