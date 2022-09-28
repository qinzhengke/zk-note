# std::vector

## 能否以数组为元素？

答案是不能，但是可以使用std::array作为元素。


## emplace_back 和 push_back的区别？

1. push_back()会构造一个临时对象，然后将对象（优先）移动或者拷贝到vector末元素尾部，而emplace_back()直接在vector末元素尾部构造。
2. push_back()要输入一个对象作为参数，而emplace_back()能实现相同的功能外，还支持输入 '对象的构造函数的参数列表'，即在emplace_back()内部构造对象。

```cpp
#include <vector>
#include <string>
#include <iostream>
#include <cassert>

struct President{
    std::string name;
    std::string country;
    int year;
 
    President(std::string p_name, std::string p_country, int p_year)
        : name(std::move(p_name)), country(std::move(p_country)), year(p_year) {
        std::cout << "I am being constructed.\n";
    }
    President(President&& other)
        : name(std::move(other.name)), country(std::move(other.country)), year(other.year) {
        std::cout << "I am being moved.\n";
    }
    ~President(){
        std::cout<< "~President()"<<std::endl;
    }
    President& operator=(const President& other) = default;
};
 
int main(){
    std::vector<President> elections;
    std::cout << "emplace_back:\n";
    auto& ref = elections.emplace_back("Nelson Mandela", "South Africa", 1994);
    assert(ref.year == 1994 && "uses a reference to the created object (C++17)");
 
    std::vector<President> reElections;
    std::cout << "\npush_back:\n";
    reElections.push_back(President("Franklin Delano Roosevelt", "the USA", 1936));

    std::cout<< "End of main" << std::endl;

    // 不支持这种操作
    // reElections.push_back("Franklin Delano Roosevelt", "the USA", 1936);
}
```

输出：
```
emplace_back:
I am being constructed.

push_back:
I am being constructed.
I am being moved.
~President()
End of main
~President()
~President()
```

可以看到，push_back()多执行了一次拷贝构造函数和析构函数，emplace_back则不会。

## 返回对象的函数只执行一次

我们通常使用vector<xxx>a(n,b)来初始化vector，但是如果b是一个函数的时候，它只计算一次，然后把结果赋值给所有成员。
而不是运行多次，这里的逻辑千万不要理解错了。

如下代码所示，运行结果并不是“0,1,2...”，而是全0。
```cpp
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
```

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