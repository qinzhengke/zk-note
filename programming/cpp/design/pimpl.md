# 技巧：Pointer to Implementation

Pointer to Implementation (PImpl) 是一种设计技巧(idiom)，使用这种模式，在开发库的过程中，我们可以去掉头文件中的私有变量和执行依赖，仅保留接口。

cppreference对于PImpl的解释：https://en.cppreference.com/w/cpp/language/pimpl

PImpl至少有以下几个好处：

1. 隐藏实现，减少用户阅读头文件的负担
2. 编译隔离，当库的实现改变时，只需要重新编译库即可，不需要编译用户代码，因为头文件没有任何改动。
3. 避免库的依赖传递到用户代码，即用户代码直接依赖“库的依赖”。

我们先说实现，然后再解释上面的好处。


## 实现

```bash
#include <iostream>
#include <memory>

// Header for user.
class A{
    class Impl; // 嵌入式的class声明：Impl
    std::unique_ptr<Impl> impl; // Impl的指针，最好使用unique_ptr    
public:
    void print();
    A();
    ~A();
};

// cpp file.
// #include "3rd_party_header.hh"
class A::Impl{
public:
    void print(){
        std::cout<<"x="<<x<<std::endl;
    }
private:
    int x = 10;  // private member.
    // third_party_type_t obj;  // 第三方库提供的类型
};

A::A() : impl{std::make_unique<Impl>()} {}
A::~A() = default;  // 一定要定义，否则unique_ptr会报错。
void A::print(){
    impl->print();
}

int main()
{
    A a;
    a.print();
}
```

首先，用户只能看到A的定义部分，A::Impl的定义的实现都是看不到的，到达了我们的第一个目的。

其次，因为A的定义只包含接口部分，只要接口不动，不管实现怎么改动，用户代码都不会被重新编译，在一些库和用户代码同时开发的情况下，能加快编译速度。

最后，我们来看一下第3点好处，首先，什么是依赖传递到用户代码？
我们在代码中加入了第三方头文件以及它提供的类型third_party_type_t，我们需要使用这种类型。
（这里我们用注释注掉了，因为这样代码才能编译运行）
如果我们不用PImpl方式，那么third_party_type_t类型必需定义在接口头文件中，对应的“3rd_party_header.h”也必需定义在接口头文件中。
这样，只要我们只要包含接口文件，就必须在编译期间提供第三方库的路径。

有人会说：包含就包含呗，有什么大不了的，反正都得用。

为了解释，我们这举个例子，假设我们的库包含好几个模块，统一的头文件内容为：

```cpp
// my_lib.h
#include "module_a.h"   // 依赖第三方库x
#include "module_b.h"   // 依赖第三方库y
#include "module_c.h"   // 依赖第三方库z
```

此时此刻，用户代码只需要“module_b”的功能，并且提供了y的编译配置，但是此时一旦包含整个“my_lib.h”，就会因为缺少第三方库x和z的路径而导致编译报错。

显然，此时让用户代码去提供x和z的编译配置肯定是不科学的，都没用到这些功能，为啥要管他，而且，万一模块很多呢，岂不是浪费时间？

另一种方法就是用户自己去甄别需要的模块，然后去包含，在这里就是“#include "module_b.h"”，
但是，我们在编写库的时候，假设库有很多模块，我们更希望的是提供一个统一的头文件，而不是提供很多个单独的头文件，让用户花时间去学习和选择。

用了PImpl，我们就可以在提供统一一个头文件的情况下，让用户只用自己关心的功能，不要关心其他模块。


## 实现PImpl细节

【事项1】使用std::unique_ptr来定义指针，不用裸指针的原因就不用说了，2020年了，再用裸指针就说不通了。
为什么要用unique_ptr，这是因为我们希望接口类和实现的类是意义绑定的，让别的实例指向实现没有任何意义。

【事项2】使用std::unique_ptr时，接口类的析构函数必需显式定义，即下面这句话是必需的。

```cpp
A::~A() = default;
```

如果不这样做，单独编译库是没问题的，但是库在使用的时候，就会报错。这是因为需要给编译器一个定义unique_ptr析构函数的地方。