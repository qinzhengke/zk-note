# C++中的修饰符（qualifiera）

## extern 关键字

## 引用另外一个文件的变量或者函数

这个特性来自于C语言，参考 \ref c_extern_var 和 \ref c_extern_function


## 导出C语言模式的函数名

导出C语言模式的函数名，保证C语言编译出来的obj能够识别函数。

```cpp
#ifdef __cplusplus__
extern "C"{
#endif
int my_c_function();
#ifdef __cplusplus__
}
#endif
```