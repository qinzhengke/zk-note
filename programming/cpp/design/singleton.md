C++中的单例设计模式{#cpp_singleton}
================================

# 单例的作用

多线程操作同一个对象的时候，例如一个多入的InputManager

# C++单例的实现

C++单例的实现五花八门，其中基于c++11最安全、方便的形式是用局部变量来实现返回instance。

注意只有在c++11中才能这样做，因为c++11中的静态变量初始化才是线程安全的，c++11以前的版本不保证。

\code{.cpp}
#include <iostream>

class Singleton {
public:
  static Singleton& GetInstance() {
    static Singleton intance;
    return intance;
  }

  ~Singleton() = default;

private:
  Singleton() = default;

  Singleton(const Singleton&) = delete;
  Singleton& operator=(const Singleton&) = delete;
};

int main() {
  Singleton& s1 = Singleton::GetInstance();
  std::cout << &s1 << std::endl;

  Singleton& s2 = Singleton::GetInstance();
  std::cout << &s2 << std::endl;

  return 0;
}
\endcode