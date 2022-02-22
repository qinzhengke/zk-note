# cmake调试问题



## cmake_define CMakeLists.txt误定义了宏

add_compile_definitions错误的把exe文件名加入了，实际上这个配置函数不需要指定可执行文件，所以my_exe被错误定义为宏，所以后面用到my_exe类型的变量、命名空间都会报错。

```
add_compile_definitions(DEF=1)  // 正确
add_compile_definitions(my_exe DEF=1)  //错误
```


## 单步调试自动调到程序结束

这种问题很诡异，非常地不合理，有一种可能：构建系统紊乱。
我们经常会使用git在不同分支之间切换，而build目录一般被我们ignore掉，所以很多别的分之的obj文件或者so文件被保存了下来，但是实际上是和我们当前的版本是不一致的。
因为obj文件和代码行号不对应，这就会导致单步调试错误地跳转。

解决方法：清除build目录重新编译。
