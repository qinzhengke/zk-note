# 引用

## 左右值的区别

1. 从性能上讲，左右值引用没有区别，基于左右值引用的传参都可以避免拷贝。

2. 右值引用可以直接指向右值，也可以通过std::move()指向左值；而左值引用只能指向左值。

3. 常量左值引用和左值引用要分开，常量左值引用因为常量特性，是万能引用，具有特殊地位，它能指向右值，但不能修改左值内容。

|      | 左值引用 | const左值引用   | 右值引用         |
|------|----------|-----------------|------------------|
| 左值 | √        | √（但无法修改） | √（std::move()） |
| 右值 | ×        | √（但无法修改） | √                |

总的来说，右值引用在传参上更为灵活一些，但仅仅是这个原因，引入右值是没有必要的。
对于只读传参，只需要const引用即可，对于读写传参，修改传入的右值仿佛又没啥意义。


## 右值引用（移动构造）的使用场景

先说结论：当以下条件必须同时满足时，只能使用移动构造，即使用右值引用。

1. 函数需要接受右值为参数，例如临时对象'A()'，字面量'123'等，。
2. 函数需要修改传入参数的内容。

第1点很好理解，如果不能接受右值为输入，那么我们的代码必将变得很‘啰嗦’，一句push_back(A())，必定要拆成一下两句：

```cpp
A a;
v.push_back(a);
```

第2点就比较难理解，右值要么是临时变量，要么是常量，不能取地址，什么情况下，需要修改它们的内容呢？

为了说明问题，这里举了一个稍微复杂点的例子，简单的例子真的找不出来，要么看不出必要性，要么实际编译时已经被优化掉。

结构体A包含一个指向大内存（100个整型数）的指针，VectorA是一个以A为元素的动态数组，类似于std::vecotor，只不过我们把元素固定为A，大小固定为3。

让我们来看看，如果我们如何要实现VectorA的push_back()函数。


```cpp
#include <vector>
#include <string>
#include <iostream>
#include <cassert>
#include <cstring>

using std::cout;
using std::string;

struct A {
    A() {
        data = new int[100];
        data[0] = 10;
        cout << "A(" << this <<  ")" << "\n";
    }
    
    // 移动构造函数
    A(A&& temp_array) {
        data = temp_array.data;
        temp_array.data = nullptr; // 为防止temp_array析构时delete data，提前置空其data_    
        cout << "Move A(" << this <<  ")" << "\n";
    }

    // 拷贝构造函数
    A(const A& temp_array) {
        data = temp_array.data; 
        cout  << "Copy A(" << this <<  ")" << "\n";
    }
 
    ~A() {
        if (data) {
            memset(data, 0, 100 * sizeof(int)); // 大内存元素为0，表示内存已经被释放。
            delete[] data;  // 
            data = nullptr;
        }

        cout << "~A(" << this <<  ")" << "\n";
    }
 
    int *data;
};

struct VectorA {
    A *data;
    int size = 0;
    VectorA() {
        data = (A*)(new char[3*sizeof(A)]);
    }

    // 基于拷贝构造，直接将内存拷贝，避免构造
    void push_back_copy(const A &a) {
        memcpy(&data[size++], &a, sizeof(A));
    }

    // 基于移动构造，在已经申请的内存上移动构造对象
    void push_back_move(A && a) {
        new (&data[size++]) A(std::move(a));
        // new (&data[size++]) A(a); // 如果不使用move，则调用拷贝构造函数
    }
};

int main(void) {
    cout << "1 ====\n";
    {
        VectorA v;
        {
            v.push_back_copy(A());
        }
        cout << v.data[0].data[0] << "\n"; // 这块内存已经被释放，非法访问
    }
    
    cout << "2 ====\n";
    {
        VectorA v;
        {
            v.push_back_move(A());
        }
        cout << v.data[0].data[0] << "\n";
    }
    
}
```

运行结果如下：
```
1 ====
A(0x5052a0)
~A(0x5052a0)
0
2 ====
A(0x505280)
Move A(0x505520)
~A(0x505280)
10
```

先分析第一种push_back_copy()，右值对象A()构造完毕后，直接把自己所有内容拷贝到给了VectorA里，后由VectorA管理。
但是实际上，但push_back_copy()结束后，A()就会析构，并且把申请的内存给释放了。
这时候VectorA管理的内存实际上是已经被释放了的，再访问属于非法，并且得到错误的结果：0。

再分析第二种push_back_move()，右值对象A()构造完毕之后，会基于移动构造、在VectorA申请好的内存上，创建对象。
当push_back_move()结束后，A()同样会析构，但是由于之前拷贝构造的时候，已经吧data置为nullptr，所以A()不会再释放内存。
这时候VectorA就能够正确访问内存，得到正确的结果：10。

让我们来分析一下问题的本质，在拷贝构造中，旧对象是无法感知到自己被拷贝过，因为旧对象是以const引用传入的，本身内容是无法改变。
所以旧对象在析构的时候，理所当然地认为自己应该清理内存。

而在移动构造函数中，旧对象是能够感知到自己被拷贝过，知道自己应该释放还是保留内存。这就需要右值内容的一个典型应用场景！

扩展来说，这是一种编程思想，即资源所有权的转移，所有权转移过程中，必定要修改新旧双方的内容，才能让新旧双方都能感知所有权转移过程。
如果旧方刚好是右值，那么就只能使用移动构造。

## 完美转发

什么是转发？转发就是在函数中，将自己的参数传入到子函数中，我们常常会碰到一些壳函数，它们除了调用实际执行的函数外，就不做其他事情。
一个函数的实现有可能会套多层“壳”，壳函数要做的事情就是把参数传递一下而已。

我们之前说过，“右值引用其实是左值”，一旦进入函数，参数就变成了左值，可以取地址，如下代码所示

```cpp
#include <iostream>

void foo(int && a) {
    std::cout <<&a << "\n"; // 10以右值引用传入后，引用本身是左值，可以取地址。
}
 
int main(void) {
    foo(10); // 10本身是右值
}
```

运行结果

```
0x50527c
```

那么现在有一个问题，我希望右值参数进来之后，仍然能够以右值的身份传入子函数，怎么办呢？
一种思路给参数套上std::move()之后传入子函数。

那么我们再进一步提问：我希望右值进来之后，仍以右值传入子函数，并且，左值进来之后，以左值的身份传入子函数，怎么实现呢？
实际上这个问题就是完美转发的需求了，这里我们就能够引出std::forward()了。

- 首先确认一点，完美转发必须在函数模板中才能工作。
- 如果传入参数为左值，那么forward()结果是左值。
- 如果传入参数为右值，那么forward()结果为右值。

来看以下代码，foo()函数内部将参数n传入子函数bar()，有三个版本的foo()函数，前两个是函数模板：基于forward()转发的foo_forward()，直接转发的foo()；
后一个是函数（并非模板），让我们来看看到底谁能够实现完美转发。

```cpp
#include <iostream>
#include <memory>
#include <utility>

void bar(int&& n) { std::cout << "rvalue overload, n=" << n << "\n"; }
void bar (int& n)  { std::cout << "lvalue overload, n=" << n << "\n"; }
 
template<class U>
void foo_forward(U&& u) {
    bar(std::forward<U>(u));
}

template< class U>
void foo(U&& u) {
    bar(u); // 这种写法，一定调用bar(int&)
}

template< class U>
void foo_l(U&& u) {
    bar(u); // 这种写法，一定调用bar(int&)
}

template< class U>
void foo_r(U&& u) {
    bar(std::move(u)); // 这种写法，一定调用bar(int&&)
}

void foo_ins(int&& n) {
    bar(n); // 这种写法，一定调用bar(int&)
    //bar(std::move(n)); // 这种写法，一定调用bar(int&&)
}
 
int main(void) {
    std::cout <<"1111\n";
    {
        foo_forward(2); // rvalue
        int i = 1;
        foo_forward(i); // lvalue
    }
    
    std::cout<< "2222\n";
    {
        foo(2); // rvalue
        int i = 1;
        foo(i); // lvalue
    }

    std::cout <<"3333\n";
    {
        foo_ins(2); // rvalue
        int i = 1;
        foo_ins(std::move(i)); // lvalue
    }

    std::cout <<"4444\n";
    {
        foo_l(2); // rvalue
        int i = 1;
        foo_r(std::move(i)); // lvalue
    }
}
```

运行结果如下所示：
```
1111
rvalue overload, n=2
lvalue overload, n=1
2222
lvalue overload, n=2
lvalue overload, n=1
3333
lvalue overload, n=2
lvalue overload, n=1
4444
lvalue overload, n=2
rvalue overload, n=1
```

显然，只有基于forward()，并且在函数模板中实现的foo()函数才能实现完美转发。

当然，如果按照第4种写法，写两个版本foo()，分别是针对左值和右值，也可以转发，但是必须得写2个版本函数，显然不够优雅。