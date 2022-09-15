# C语言编译报错速查


## c_redeclaration 重复声明

具体报错形式：

```bash
error: redeclaration of 'xxx'
```

常见排查步骤：

- 确保头文件加入#ifdef #define #endif，保证内容只定义一次。


## gcc不识别_Bool以及bool类型
_Bool以及其封装bool，是C99才引入的，如果编译器不识别，有可能是C99选项没有打开，打开C99的方法根据构建系统而不同，需要搜索。

【参考】\ref c99_bool


## 加入static修饰符才能提示“unused”

很奇怪，如果不加static修饰符，编译器是不会提示“unused”的警告的，原因是未解之谜。

```cpp
static const char * a = "abcdef"; // 警告
static const char a[] = "abcdef"; //OK
```


## No rules to make target
这个提示表示编译模块时时找不到源代码，检查一下C和C++文件是否在构建脚本中添加进去了。


## core.hpp header must be compiled as c++
在c文件中include Opencv的hpp头文件就会出现这个问题，因为编译c文件时是使用C编译器编译的，而.c文件是不认识.hpp文件的，实现的方式就是c编译器并未定义__cplusplus宏。按照道理说c文件不应该包含h文件，最好把c文件改成cpp文件。如果硬是不改，在VisualStudio中可以设置compile as c++。在别的环境就不知道怎么搞了。

## multiple definition?

注意，链接时的重复定义和编译时的重复声明不同，清楚的区分能够让我们更快速的定位问题。

编译时期的重复声明问题参见 \ref c_redeclaration

常见的排查步骤：

- 确保函数或者变量没有重复定义，如果非要使用同名函数，加入static修饰符，只供本地函数使用。
- 确保没有在头文件中定义变量，如果定义了变量，那么该头文件被多次include的时候，变量会重复的定义。
- 【罕见】确保没有发生该情况：一个工程对同一个源文件编译了多次（源文件改变了位置，但是构建工具没有清理缓存），Linker会发现工程内多个同名.o文件，自然里面的变量也会重复定义。

【注意】在头文件中定义变量是不科学的做法，哪怕加入了static防止重复定义，每一个包含了该头文件的源文件生成的obj都会包含该变量，增加了编译时间，并且浪费内存。


## unresolved external symbol？

常见的排查步骤：

- 确保变量名称拼写无误。
- 确保包含该变量的源文件被加到工程中。
- 确保不同源文件使用了相互兼容的字符编码（最好是相同的编码）。
- 确保包含变量的二进制库的版本和当前编译器版本匹配，包括平台（x86或者x64）、版本（vc08，vc10，vc12，vc14）以及是否调试（debug和release）。
- 如果使用了QtCreator，尝试着显式地qmake一下，然后再rebuild。




