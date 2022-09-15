# C++11增加初始化方式

## 花括号初始化列表

花括号初始化列表，英文表达是“brace-enclosed initializer list”，是使用花括号来直接对结构体进行初始化的一种方法，减少代码量的编写。

在对象的构造过程以及函数传参过程中都可以使用。

注意，顺序靠后的参数可以缺省。

具体实例如下代码所示。

```cpp
#include <iostream>
#include <string>
using namespace std;

struct A{
    int x;
    float y;
    std::string z;
};

void print(A a){
    cout<<"{"<<a.x<<","<<a.y<<","<<a.z<<"}"<<endl;
}

int main(){
    A a{1,2.1,"three"};
    print(a);
    print({2,4.2,"six"});
    print({1,2.1});
    // print({1,"three"});  // 编译错误，
}

```

运行结果：

```bash
{1,2.1,three}
{2,4.2,six}
{1,2.1,}
```

