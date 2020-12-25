std::ref()函数{#cpp11_std_ref}
=============================

std::ref()是C++11引入的一个函数，整体用法和&差不多，但是返回的是一个“reference_wrapper”类型的对象。

reference_wrapper至少有以下用处：

- 任何类型在经过std::ref()函数后都返回reference_wrapper类型，它可以被枚举，可以构成数组，而“&”做不到这点。有点类似于std::function一样。

\code{cpp}
#include <iostream>
#include <functional>  // for reference_wrapper
using namespace std;

int main()
{
    int x=5, y=7, z=8;

    //  错误，不能将引用构建成数组。
    // int& arr[] {x,y,z};

    // 正确，
    reference_wrapper<int> arr[] {x,y,z};
    for (auto a: arr)
        cout << a << " ";
    return 0;
}
\endcode

> 来源：Stack Overflow 提问：C++ Difference between std::ref(T) and T&?