# 关键字constexpr


## 如何在类中声明constexpr常量？

先把正确的写法放上来：

```cpp
#include <iostream>
using namespace std;

struct A{
    static constexpr int x = 10;
};

constexpr int A::x;

int main(){
    A a;
    cout<<"a.x="<<a.x<<",sizeof(A)="<<sizeof(A)<<endl;
}
```

运行结果：

```bash
a.x=10,sizeof(A)=1
```

有三点需要注意，三者缺一不可：

1. 要使用static来修饰
2. 类内部只是声明，要在类的外部进行定义
3. 初始化只能在类中的声明部分进行，不能在类外的定义进行。

首先，在类中基于constexpr声明的常量必需是static的，这是因为constexpr必需在编译阶段就能使用，而普通非static量是存放在内存当中的，显然编译时期是无法访问的。

由于是static类型，只在class里只有声明是无法使用变量的，会出现“undefined symbol”问题，所以需要在源文件中添加常量的定义。

但是源文件中constexpr的常量定义又有点不一样，它的初始化必须在类中的声明里加入，定义的地方反而是没有进行初始化的。

通过sizeof(A)我们可以看到，类A的大小只有1个字节，说明它是一个空类。（空类为了其对象能够寻址会保证至少1个字节占用）
