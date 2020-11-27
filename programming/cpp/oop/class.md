C++中的类和对象{#cpp_class}
========================

\section 基类和派生类的构造与析构函数执行顺序是什么？

情况一：在普通情况，即非多态方式销毁对象下，

构造函数：先执行基类的构造函数，后执行派生类的构造函数。

析构函数：先执行派生类的析构函数，后执行基类的构造函数。

情况二：在多态方式销毁对象下，
构造函数：和情况一相同。
析构函数：只运行基类的析构函数，不运行派生类的构造函数。

\code{cpp}
#include <iostream>
using namespace std;
class Base{
public:
    Base(){ cout<<"Base()"<<endl; }
    ~Base(){ cout<<"~Base()"<<endl; }
};

class Derived : public Base{
public:
    Derived(){ cout<<"Derived()"<<endl; }
    ~Derived(){ cout<<"~Derived()"<<endl; }
};

int main(){
#if 0   // 情况一
    Derived a;
#else   // 情况二
    Base *p = new Derived();
    delete p;
#endif
}

\endcode

情况一运行结果：

\code{bash}
Base()
Derived()
~Derived()
~Base()
\endcode

情况二运行结果：

\code{bash}
Base()
Derived()
~Base()
\endcode

【小贴士】情况二告诉我们，派生类的析构函数有的时候不会被调用，如果有一些资源要在派生类析构时释放，那么在这种情况下，资源无法释放，导致内存泄露。
正确的使用方法是将析构函数设置为虚函数，这样即使是多态方式调用，析构函数也会被正确调用，参见\ref virtual_deconstructor

<hr>
\section 引用作为类的成员变量

引用是可以作为类的成员变量的，注意在构造函数的初始化列表里初始化要绑定的变量即可，注意const修饰符。

【例】如下代码所示：

\code{cpp}
#include <iostream>
using namespace std;
class A{
public:
   A(const int& x) : x_(x) {}
   void print() { cout << x_ << endl; }
   const int& x_;
};

int main(int argc, char* argv[]){
   int x = 5;
   A a(x);
   a.print();
   x = 6;
   a.print();
}
\endcode

运行结果为：

\code{bash}
5
6
\endcode