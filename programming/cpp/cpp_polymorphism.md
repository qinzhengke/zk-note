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

<hr>
\section 基类和派生类可以定义相同名称的成员

如果不是明确的需要基类和派生类各自需要同名称的变量，普通情况下，千万不要在派生类里重复定义变量，因为两者的含义并不相同，会导致结果不是我们想要的。

\code{.cpp}
#include <iostream>

using namespace std;

class A{
public:
    int x = 5;
};

class B : public A{
public: 
    int x = 10;
};

int main()
{
    B b;
    cout<<"B.x="<<b.x<<endl;
    A *a = &b;
    cout<<"A->x="<<a->x<<endl;
}

\endcode

<hr>
\section 重写(override)父类的重载(overload)函数

\code{.cpp}
// Example program
#include <iostream>
#include <string>

using namespace std;

class Base{
public:
    virtual void func(int a) = 0;
    virtual void func(float a) = 0;
    virtual void func(int a, int b) = 0;
};

class Derived : Base{
public:
    void func(int a) { cout<<"int func 1"<<endl; }
    void func(float a) { cout<<"float func 1"<<endl; }
    void func(int a, int b) {cout<<"int func 1,2"<<endl;}
};

int main()
{
    Derived d;
    d.func(1);
    d.func(1.0);    //调用会出错，不调用就没事。
    d.func(1,2);
}

\endcode

<hr>
\section 重写函数可以子类型

重写函数不一定要返回类型和参数类型完全一致，这些类型的派生类也是可以接受并认为是派生的。

\code{.cpp}
#include <iostream>
#include <string>
using namespace std;

class Base{
public:
    virtual Base * clone() = 0;
};

class Derived : Base{
public:
    Derived(){cout<<"Derived()"<<endl;}
    Derived * clone() override { return new Derived();} //确实是override
};

int main()
{
    Derived d;
    d.clone();
}
\endcode

这样的好处是：派生类重写成员函数的时候，不在需要内部一个强行的指针转换，在重写的成员函数数量比较多的时候，还是挺能节省时间的。

遗憾的是，如果这些指针被shared_ptr封装，就不能这样写了。

\code{.cpp}
#include <iostream>
#include <string>
using namespace std;

class Base{
public:
    virtual shared_ptr<Base> * clone() = 0;
};

class Derived : Base{
public:
    Derived(){cout<<"Derived()"<<endl;}
    shared_ptr<Derived> * clone() override { return shared_ptr<Dervied>(new Derived());} //这样写编译报错，
};

int main()
{
    Derived d;
    d.clone();
}
\endcode