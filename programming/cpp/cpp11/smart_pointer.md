C++11的智能指针{#cpp11_smart_pointer}
===================================

\section share_ptr简单用法

\code{.cpp}
// shared_ptr constructor example
#include <iostream>
#include <memory>

struct C {int* data;};

int main () {
  std::shared_ptr<int> p1;
  std::shared_ptr<int> p2 (nullptr);
  std::shared_ptr<int> p3 (new int);
  std::shared_ptr<int> p4 (new int, std::default_delete<int>());
  std::shared_ptr<int> p5 (new int, [](int* p){delete p;}, std::allocator<int>());
  std::shared_ptr<int> p6 (p5);
  std::shared_ptr<int> p7 (std::move(p6));
  std::shared_ptr<int> p8 (std::unique_ptr<int>(new int));
  std::shared_ptr<C> obj (new C);
  std::shared_ptr<int> p9 (obj, obj->data);

  std::cout << "use_count:\n";
  std::cout << "p1: " << p1.use_count() << '\n';
  std::cout << "p2: " << p2.use_count() << '\n';
  std::cout << "p3: " << p3.use_count() << '\n';
  std::cout << "p4: " << p4.use_count() << '\n';
  std::cout << "p5: " << p5.use_count() << '\n';
  std::cout << "p6: " << p6.use_count() << '\n';
  std::cout << "p7: " << p7.use_count() << '\n';
  std::cout << "p8: " << p8.use_count() << '\n';
  std::cout << "p9: " << p9.use_count() << '\n';
  return 0;
}
\endcode

值得注意的是：

1. “<>”中的

\section 基于share_ptr的多态

使用dynamic_pointer_cast进行指针转换即可，但是要注意有一个小坑，参见 \ref dynamic_poiner_cast_debug 。


\section dynamic_pointer_cast

dynamic_pointer_cast可以把shared_ptr进行类型转换，并且保持引用计数+1，好像同一种类型一样。
dynamic_pointer_cast只针对与动态的类型，也就是多态对象的指针，如果对象的类中不包含虚函数，那么这个cast函数将不可使用。
这种情况可以使用static_pointer_cast。

\code{.cpp}
// static_pointer_cast example
#include <iostream>
#include <memory>

struct A {
  static const char* static_type;
  const char* dynamic_type;
  A() { dynamic_type = static_type; }
};
struct B: A {
  static const char* static_type;
  B() { dynamic_type = static_type; }
};

const char* A::static_type = "class A";
const char* B::static_type = "class B";

int main () {
  std::shared_ptr<A> foo;
  std::shared_ptr<B> bar;

  bar = std::make_shared<B>();

  foo = std::dynamic_pointer_cast<A>(bar);

  std::cout << "foo's static  type: " << foo->static_type << '\n';
  std::cout << "foo's dynamic type: " << foo->dynamic_type << '\n';
  std::cout << "bar's static  type: " << bar->static_type << '\n';
  std::cout << "bar's dynamic type: " << bar->dynamic_type << '\n';

  return 0;
}
\end


\section 关于shared_ptr用于函数形参的思考

shared_ptr作为形参，当实参也是shared_ptr时，使用值传递是没有问题的，传参过程中，引用计数会+1，函数结束后，引用计数-1，没什么毛病。

但是shared_ptr作为形参，同时实参是普通指针时，这样传递，就会导致函数结束时内存被释放。


\section issue_undefine_typeinfo Undefined Reference to Typeinfo

在项目中遇到了这样一个问题：C++文件编译都OK，但链接的时候报错：undefined reference to `typeinfo for xxx’。typeinfo是C++中的RTTI(RunTime Type Identification)机制中记录类型信息用的，dynamic_cast和typeid操作符会使用这些信息。

以”undefined reference to typeinfo”为关键字在网络上搜索，大多数都是说有虚函数定义了但是未实现导致的。但是我的代码显然不是这个情况。在我即将放弃的时候，终于在StackOverflow上发现有人提出，这种错误的原因也可能是混合使用了带RTTI信息和不带RTTI信息的代码导致的。对比检查，发现我的项目里的问题正是这个。最后用了一点dirty hack，解决了bug。下面就仔细分析一下”undefined reference to `typeinfo for xxx’“产生的原因。

虚函数未实现
产生”undefined reference to `typeinfo for xxx’“最常见的原因就是基类的虚函数未实现了。由于C++类的实现可以分布在多个源文件中，所以生成目标文件时，基类的虚函数没有定义是不会报错的。但是链接成可执行文件时，需要将虚函数的信息放进typeinfo中，这个时候虚函数未实现就会引发这个错误。

\code{.cpp}
class Base{
public:
    virtual func(); // 使用了dynamic_pointer_cast时，编译会出错
    virtual func() = 0; // 设置成纯虚函数，或者实现它。
}
\endcode

混用了no-RTTI代码和RTTI代码
我碰到的正是混用了no-RTTI和RTTI代码的情形。项目中我们自己写的程序必须开启RTTI，而我们使用的外部的一个库使用no-RTTI编译。我们在自己的代码中需要重载一个外部库中的带虚函数的类，结果链接的时候就出现了问题。外部库中的基类使用-fno-rtti选项编译，生成的代码没有typeinfo信息，而我们的代码使用-frtti选项编译，要求基类必须要有typeinfo信息。最后，我在编译系统中做了一些dirty hack，让那个派生类所在的源文件以-fno-rtti选项编译，解决了问题。

----------------------------------------------------------------

我遇到的问题类似,现在的项目中需要开启RTTI,链接的外部库是no-RTTI编译的,在现在的工程中重载外部库的带虚函数的类,连接的时候报错.原文说的dirty hack，是对单个文件加编译选项-fno-rtti 。因为我用的外部库是可以开启RTTI的,我用RTTI重新编译一次后,现在的工程不报错啦。


\section shared_ptr不能指向this指针

如下代码所示，本身对象b在main()函数结束之后自动析构，同时析构过程会把成员变量中的sp_this指向的对象（也就是自己）先行析构，所以最终会导致析构了两次。

\code{.cpp}
#include <iostream>
#include <memory>

using namespace std;

class B{
public:
    B(){ sp_this = shared_ptr<B>(this);}
    ~B(){ cout<<"~B"<<endl; }
    shared_ptr<B> sp_this;
};

int main ()
{
  B b;
  return 0;
}
\end

如果一定想shared_ptr指向this指针，可以这样操作

\code{.cpp}
#include <iostream>
#include <memory>

using namespace std;

class B :  public enable_shared_from_this<B>{
public:
    B(){ sp_this = shared_from_this();}
    ~B(){ cout<<"~B"<<endl; }
    shared_ptr<B> sp_this;
};

int main ()
{
  B b;
  return 0;
}
\endcode


\section shared_ptr的空指针

使用nullptr最为方便，或者"return {}"也可以。