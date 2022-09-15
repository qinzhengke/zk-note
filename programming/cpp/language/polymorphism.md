# C++中的多态

## keyword_override C++中的override关键字有什么作用？

override关键字在派生类中使用，告诉编译器，它所修饰的函数必需重写（override）基类的某个虚函数，否则编译器要报错。
这样强制有什么用呢？一个很大的作用（唯一的作用？）就是能在编译阶段防止拼写错误导致的重写失败问题，举个例子，如下代码所示，故意拼错派生类的函数名。

```cpp
#include <iostream>
using namespace std;
class Base{
public:
    virtual void print(){ cout<<"Base::print()"<<endl;}
};

class Derived : public Base{
public:
    // 注意下面的函数名拼写错误
#if 0
    void pirnt(){ cout<<"Derived::print()"<<endl;} // 情况一：编译器不报错，运行异常
#else
    void pirnt() override { cout<<"Derived::print()"<<endl;}　// 情况二：编译器报错
#endif
};

int main(){
    Base *p = new Derived();
    p->print();
}

```

情况一运行结果：编译器不报错，但是运行出现了错误，没出现我们希望的Derived::print()

```bash
Base::print()
```

情况二编译结果：编译器提示没有重写任何函数。
```bash
15:10: error: 'void Derived::pirnt()' marked override, but does not override
```

<b>【小贴士】</b>
别看重写失败是一个小问题，这有可能让开发者调试半天，善用override会让你减少潜在的调试时间。


## 多态中为何需要将析构函数设置为virtual类型？

如果不设置析构函数为虚函数，那么当使用基类指针指向派生类对象的方式（多态）来销毁对象时，派生类的析构函数就不会被执行。
如果派生类的析构函数承担了释放资源的任务，那么这种情况下资源将得不到释放，进而导致内存泄漏。

相关示例代码如下：
```cpp
#include <iostream>

using namespace std;

class Base{
public:
    Base(){ cout<<"Base()"<<endl; }
#if 1   
    virtual ~Base(){ cout<<"~Base()"<<endl; }   // 情况一：正确
#else
    ~Base(){ cout<<"~Base()"<<endl; }   // 情况二：错误
#endif
};

class Derived : public Base{
public:
    Derived(){ cout<<"Derived()"<<endl; }
    ~Derived() override { cout<<"~Derived()"<<endl; }
};

int main(){
    Base *p = new Derived();
}
```

情况一运行结果（正确）：
```bash
Base()
Derived()
~Derived()
~Base()
```


情况二运行结果（错误）：
```bash
Base()
Derived()
~Base()
```

<b>【小贴士】</b>
基类的析构函数名为~Base()，派生类的析构函数名为~Derived()，虽然它们的名字不同，但是它们确实可以是重写（override）的关系，代码中的“override”也证实了这一点。

<b>【小贴士】</b>
多态模式下的析构顺序：首先，根据虚函数表指针，找到了派生类的析构函数，并执行之，析构函数返回前最后一件事（比函数内最后一行代码还晚）就是执行基类的析构函数，于是得到了我们想要的结果。


## 如何优雅地定义没有成员函数的类型为虚类？

```cpp
// base.hh
class Base
{
public:
    virtual ~Base() = 0;
    ...
};

// base.cc
Base::~Base() {}
```

注意，析构函数不要定义在头文件中，否则链接的时候会报redefinition错误。


## 如何优雅的拷贝虚类对象？

往往我们拿到的对象指针是虚类，要拷贝虚类对象，直接调用虚类的拷贝构造函数是不行的，优雅的方法是在虚类中定义一个clone()函数，让派生类去实现这个函数。

```cpp
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

```


## 抽象类的构造函数不能运行成员虚成员函数

其实原因很简单，当派生类在构造过程中，自己到底是什么类型是不知道的，从而不知道要调用那种类型的成员函数。
只有构造完毕才能知道，实际上虚函数表指针也是在构造函数中进行赋值的。

```cpp
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
```


## 基类和派生类可以定义相同名称的成员

如果不是明确的需要基类和派生类各自需要同名称的变量，普通情况下，千万不要在派生类里重复定义变量，因为两者的含义并不相同，会导致结果不是我们想要的。

```cpp
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

```


## override_overload 重写(override)父类的重载(overload)函数

```cpp
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

```


## 重写函数可以子类型

重写函数不一定要返回类型和参数类型完全一致，这些类型的派生类也是可以接受并认为是派生的。

```cpp
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
```

这样的好处是：派生类重写成员函数的时候，不在需要内部一个强行的指针转换，在重写的成员函数数量比较多的时候，还是挺能节省时间的。

遗憾的是，如果这些指针被shared_ptr封装，就不能这样写了。

```cpp
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
```


## 基于基类指针获取子类的size？

没有直接的方法，但是可以在基类定义一个虚函数size()，子类实现就好了。

```cpp
struct base {
  virtual size_t size() const =0;
  virtual ~base() { }
};

template<typename T> 
struct intermediate : base {
  virtual size_t size() const { return sizeof(T); }
};

struct derived : intermediate<derived> 
{ };
```