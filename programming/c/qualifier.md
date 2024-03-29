# C语言中的修饰符（qualifier）

C语言中的修饰符是指除了类型之外，用来描述变量的一些属性的关键字，例如：

- const
- static
- extern
- regsiter
- volatie
  


## const修饰符

- 修饰变量，则该变量在生命周期内无法被改变。
- 修饰指针，根据const的位置而不同：在“type*”之前，表示不允许修改指向的内容，在“type*”之后，表示不允许改变指向，当然如果const同时出现在两处，说明指针既不能改指向，也不能改内容。能同时应用在函数传参中。

【例】示例代码如下
```cpp
#include <stdio.h>
int main() {
    int a[5] = {1, 2, 3, 4, 5};
    const int *p = a;
    p = a+1;
    printf("*p = %d\n", *p);
    // *p = 99; // 编译报错，不允许修改指向的内容
    
    int * const q = a;
    *q = 99;
    printf("*q = %d\n", *q); 
    // q = a+1; // 编译报错，不允许修改指向

    const int * const r = a;
    // r = a + 1;   // 编译报错，不允许修改指向
    // *r = 99;     // 编译报错，不允许修改指向的内容
}
```

【注意】const修饰函数体是C++才有的特性，这里只讨论C语言的内容，不要给搞混了。

## const函数不能返回引用

如题，const函数不能返回引用，因为这样的行为会给予用户代码修改内部变量的机会，同样不符合const的原则。
下面的代码展示了这样错误的案例，编译器会报错。

```cpp

struct A{
    int data[1];
    int & x() const{
        return data[0];
    }
};

int main()
{
    A a;
    a.x() = 2;
    printf("%d\n", a.x(0));
}

```

编译器报错如下所示：
```bash
 In member function 'int& A::x() const':
8:22: error: invalid initialization of reference of type 'int&' from expression of type 'const int'
```

所以，const修饰函数，影响仅仅是承诺函数体不修改成员变量是不够的，还有不能返回成员变量的引用。


## const常量和字面常量（Literal）的区别

C99中，全局变量和静态变量初始化必须使用字面常量，例如“1，0xff”以及枚举。
const修饰的常量是二等公民，不能用于初始化，坑了我很久。

例如
```cpp
// C99
const int a =10;
int b = a; // 错误，const变量也不能用于全局变量初始化。
```

【扩展】在C++中，非字面常量也有办法能够应用到初始化。

 
# extern修饰符

extern修饰符有以下作用：

- 引用另外一个文件的变量
- 引用另外一个文件的函数

默认的全局变量都开放成全局可用变量，其他模块想用该变量只需要extern声明即可。
例如a.c文件中定义int a = 0; b.c文件中声明extern int a;
那么b文件中使用的变量其实就是a文件中的变量
a.c文件和b.c文件不需要什么相互依赖的关系，也和头文件无关。

只需要在链接过程能够找到extern的来源即可。
