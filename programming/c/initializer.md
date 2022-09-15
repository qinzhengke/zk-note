# 初始化

## Designated Initializers

C语言中结构体在C99中的一种新的初始化语法，叫做designated initializer，即指定初始化，之前我一直找不到正式的名称，我一直称之为点变量初始化。。。

```cpp
MY_TYPE a = { .flag = true, .value = 123, .stuff = 0.456 };
```
```cpp
type struct
{
  int a1;
  float a2;
}A;
typedef struct
{
  A sa;
  int b1;
  float b2;
}B;
B bs[2] = 
{
  {
    {
      a1 = 0,
      a2 = 0.0,
    }
    b1 = 0,
    b2 = 0.0,
  },
  {
    {
      a1 = 1,
      a2 = 1.0,
    }
    b1 = 1,
    b2 = 1.0,
  }
}
```
这个东西与编译器无关，是C99的标准，gcc想用的话必须开启选项 -std=c99

【注意】Designated Initializers是C语言的语法，不是C++的语法，至少C++17还没有包含，在C++代码中不要混用！

【注意】如果使用QtCreator构建工程，那么默认使用g++编译，然而如果designated initializer语句是在cpp文件里，g++是不支持的，不管CXX_FLAG 是不是加了c++11，或者C_FLAG 加了99，或者加入extern "C"，都不行， extern "C"只是指示c++编译器把函数编成C语言可识别的符号表，并不支持这种赋值语法。

但是我发现了一种方法，就是强行改cpp后缀为c，这样g++就会调用gcc来编译c文件。