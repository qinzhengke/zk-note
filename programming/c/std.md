# C语言标准库

## 标准库的实现版本

C语言标准库也只是一群大佬定义的一种标准，并没有要求谁或者限制谁来实现，自然和编译器一样，也会有许多的版本。

让我们来看看标准库有哪一些版本。

- glibc：GNU提供的C标准库
- BSD libc：BSD提供的C标准库
- Microsoft C run-time library：微软提供的标准库属于Visual C++的一部分。
- Bionic：谷歌为Android操作系统开发的标准库。


## printf 中的uint64整数
使用printf过程中，一定要写对%字符串，之前遇到过一个问题，使用%d来打印一个int64的数是错误的，例如下列代码

```cpp
uint64_t a = 1000;
printf("%d\n",a); // 错误
```

实际上，%u，%ld，%lu都不对，在曾经一个ARM平台编译器上，正确的结果是%llu，因为在该平台上，sizeof(long)=sizeof(int)=4，而uint64_t真正的类型是long long，在不同平台时，要注意类型字节数可能不一样。


## 怎样printf size_t类型才不会报warning
```cpp
// C89
size_t foo;
...
printf("foo = %lu\n", (unsigned long) foo);
// For C99 and later, use %zu:

size_t foo;
...
printf("foo = %zu\n", foo);
```

## 彩色规范打印

```cpp
#indef PRINT_ERROR
#define PRINT_ERROR(...)    \
  do{                       \
    printf("\x1B[41;37m");  \
    printf("[ERROR]");      \
    printf("\x1B[0m");      \
    printf(__VA_ARGS__);    \
    }                       \
  while(0)
#endif
  
#ifndef PRINT_INFO
#define PRINT_INFO(...)     \
  do{                       \
    printf("\1xB[42;37m");  \
    printf("[INFO] ");      \
    printf(__VA_ARGS__);    \
    }                       \
  while(0)
#endif

```

## FLT_EPSILON宏和DBL_EPSILON 

问题：做除法的时候，我们知道分母不能为0，那么如何检查呢？ “if(a!=0.0f)”这种小学生的错误就不要犯了。

解决方法：一般来说我们需要拿到一个很小的数，如果分母的绝对值小于这个数，就认为分母为零。
最适合的方法是使用<float.h>提供的FLT_EPSILON和DBL_EPSILON。

```cpp
#include <stdio.h>
#include <float.h>

int main(int argc, char* argv[]){
    printf("FLT_EPSILON=%.20f\n", FLT_EPSILON);
    printf("DBL_EPSILON=%.20f\n", DBL_EPSILON);
}
```

运行结果为：

```bash
FLT_EPSILON=0.00000011920928955078
DBL_EPSILON=0.00000000000000022204
```


## 判断数字是否是nan

isnan()函数。