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

<hr>
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