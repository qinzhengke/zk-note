std::vector{#cpp_vector}
========================


# 能否以数组为元素？

答案是不能，但是可以使用std::array作为元素。

# 初始化

## 返回对象的函数只执行一次

我们通常使用vector<xxx>a(n,b)来初始化vector，但是如果b是一个函数的时候，它只计算一次，然后把结果赋值给所有成员。
而不是运行多次，这里的逻辑千万不要理解错了。

如下代码所示，运行结果并不是“0,1,2...”，而是全0。
\code{cpp}
#include <iostream>
#include <vector>
using namespace std;

int create(){
    static int x = 0;
    return x++;
}

int main()
{
    vector<int> x(10,create());
    for(auto xx : x){
        cout<<xx<<endl;
    }
}
\endcode

运行结果：

```
0
0
0
0
0
0
0
0
0
0
```