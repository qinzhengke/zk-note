# C++11中的其他有用特性

## using 代替 typedef

using代替typedef的好处是可以支持模板。

```cpp
// C++11写法
template <size_t N>
using Vector = Matrix<N, 1>;

// C++03写法
template <size_t N>
struct Vector
{
    typedef Matrix<N, 1> type;
};
```


## 使用constexpr作为类的成员变量

没有什么困难，就是要注意constexpr修饰类成员变量时，类成员变量必须是static的，否则编译器会报错。

我的理解是既然是编译期常量，那么每个实例对应该常量的值应该是相同的，实际上等效成一个static类型，如果每个实例都存储相同的值，必然会造成空间浪费。
所以直接constexpr成员变量为static类型。

【例】如下代码所示：

```cpp
#include <iostream>
class A{
public:
    static constexpr int X = 10; // 去掉static，编译报错。
    void print(){ std::cout<<"X="<<X<<std::endl;}
};

int main(int argc, char* argv[]){
    A a;
    a.print();
}
```