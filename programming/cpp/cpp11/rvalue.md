右值{#rvalue}
===========

\section move_constructor 移动构造函数

The move constructor is typically called when an object is initialized (by direct-initialization or copy-initialization) from rvalue (xvalue or prvalue) (until C++17)xvalue (since C++17) of the same type, including

- initialization: T a = std::move(b); or T a(std::move(b));, where b is of type T;
- function argument passing: f(std::move(a));, where a is of type T and f is void f(T t);
- function return: return a; inside a function such as T f(), where a is of type T which has a move constructor.

以下几种情况，移动构造函数会被隐式删除

- 类中存在非静态的不可被移动的成员，包括被删除、无法访问（私有）、存在歧义。
- 类的基类是不可移动的，包括被删除、无法访问（私有）、存在歧义的移动构造函数。
- 类的基类存在被删除的或者是无法访问（私有）的析构函数。
- T is a union-like class and has a variant member with non-trivial move constructor.

Deleted implicitly-declared move constructor
The implicitly-declared or defaulted move constructor for class T is defined as deleted if any of the following is true:

- T has non-static data members that cannot be moved (have deleted, inaccessible, or ambiguous move constructors);
- T has direct or virtual base class that cannot be moved (has deleted, inaccessible, or ambiguous move constructors);
- T has direct or virtual base class with a deleted or inaccessible destructor;
- T is a union-like class and has a variant member with non-trivial move constructor.