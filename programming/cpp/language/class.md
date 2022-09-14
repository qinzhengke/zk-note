# C++中的类和对象{#cpp_class}

## cpp_class_static_member 类中的静态成员变量的定义

\code{cpp}
#include <cstdio>

class A{
    public:
    A(){}
    static int count;
};
int A::count = 0;

int main(void){
    A a;
    printf("%d\n",a.count);
    return 0;
}
\endcode

## base_member_in_constructor 派生类构造函数中不能调用基类成员函数

\code{cpp}

// Example program
#include <iostream>
#include <string>

class Base{
    void print(){
        printf("Base::print()\n");
    }
};

class Derived{
public:
    Derived(){
        print();
    }
};

int main()
{
    Derived a;
}

\endcode

运行结果：

\code
 In constructor 'Derived::Derived()':
14:15: error: 'print' was not declared in this scope
\endcode



# 基类和派生类的构造与析构函数执行顺序是什么？

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

>【小贴士】情况二告诉我们，派生类的析构函数有的时候不会被调用，如果有一些资源要在派生类析构时释放，那么在这种情况下，资源无法释放，导致内存泄露。
正确的使用方法是将析构函数设置为虚函数，这样即使是多态方式调用，析构函数也会被正确调用，参见\ref virtual_deconstructor


# 引用作为类的成员变量

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

# 匿名struct和匿名union

很多时候，当我们定义类型时，我们希望拥有多种方式对同一个地址的变量进行引用。
qq
举个例子，比如四元数，我们即希望能使用数组的方式来引用，这样方便内存拷贝以及循环操作，同时，我们也希望使用w,x,y,z方式引用它，这样更直观，因为w,x,y,z的名称有自解释能力。

uinon能够提供这个功能，union在C语言标准里是可以匿名的，这没什么问题。

匿名的好处是不需要在额外命名，父结构体直接引用子union的成员。

union要配合strcut使用，才能得到我们期望的效果，但是struct能否是匿名的呢？

这个问题似乎有不同的答案，stackoverflow上有一个回答，说是可以编译运行，但是编译器会警告，说是不兼容C语言。

> stackoverflow提问: Why does C++ disallow anonymous structs?

但是我使用cpp.sh（c++98）进行了实验，是完全没有问题的，警告也没有。

\code{.cpp}
#include <cstdio>
struct Vec {
  union {
    struct {
      float x,y,z;
    };
    float m[3];
  };
};

int main(){
    Vec v;
    v.x = 10;
    v.y = 1.2;
    v.z = -2.5;
    printf("%f,%f,%f", v.m[0], v.m[1], v.m[2]);
}
\endcode

运行结果：

\code
10.000000,1.200000,-2.500000 
\endcode

所以，个人认为结论是可以直接用。