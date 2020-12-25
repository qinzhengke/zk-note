C语言构建常见问题{#c_compile_faq}
==============================


\section 编译常见问题


\subsection c_redeclaration 重复声明

具体报错形式：

\code{bash}
error: redeclaration of 'xxx'
\endcode

常见排查步骤：

- 确保头文件加入#ifdef #define #endif，保证内容只定义一次。


\subsection gcc不识别_Bool以及bool类型
_Bool以及其封装bool，是C99才引入的，如果编译器不识别，有可能是C99选项没有打开，打开C99的方法根据构建系统而不同，需要搜索。

【参考】\ref c99_bool


\subsection 加入static修饰符才能提示“unused”

很奇怪，如果不加static修饰符，编译器是不会提示“unused”的警告的，原因是未解之谜。

\code{c}
static const char * a = "abcdef"; // 警告
static const char a[] = "abcdef"; //OK
\endcode


\subsection no_rule_make_target No rules to make target
这个提示表示编译模块时时找不到源代码，检查一下C和C++文件是否在构建脚本中添加进去了。


\subsection c_include_opencv core.hpp header must be compiled as c++
在c文件中include Opencv的hpp头文件就会出现这个问题，因为编译c文件时是使用C编译器编译的，而.c文件是不认识.hpp文件的，实现的方式就是c编译器并未定义__cplusplus宏。按照道理说c文件不应该包含h文件，最好把c文件改成cpp文件。如果硬是不改，在VisualStudio中可以设置compile as c++。在别的环境就不知道怎么搞了。

\htmlonly
<hr style="height:20px;border-width:0;color:gray;background-color:gray">
\endhtmlonly

\section 常见链接问题


\subsection c_mul_def multiple definition?

注意，链接时的重复定义和编译时的重复声明不同，清楚的区分能够让我们更快速的定位问题。

编译时期的重复声明问题参见 \ref c_redeclaration

常见的排查步骤：

- 确保函数或者变量没有重复定义，如果非要使用同名函数，加入static修饰符，只供本地函数使用。
- 确保没有在头文件中定义变量，如果定义了变量，那么该头文件被多次include的时候，变量会重复的定义。
- 【罕见】确保没有发生该情况：一个工程对同一个源文件编译了多次（源文件改变了位置，但是构建工具没有清理缓存），Linker会发现工程内多个同名.o文件，自然里面的变量也会重复定义。

【注意】在头文件中定义变量是不科学的做法，哪怕加入了static防止重复定义，每一个包含了该头文件的源文件生成的obj都会包含该变量，增加了编译时间，并且浪费内存。


\subsection unresolved_external_symbol unresolved external symbol？

常见的排查步骤：

- 确保变量名称拼写无误。
- 确保包含该变量的源文件被加到工程中。
- 确保不同源文件使用了相互兼容的字符编码（最好是相同的编码）。
- 确保包含变量的二进制库的版本和当前编译器版本匹配，包括平台（x86或者x64）、版本（vc08，vc10，vc12，vc14）以及是否调试（debug和release）。
- 如果使用了QtCreator，尝试着显式地qmake一下，然后再rebuild。


\subsection 动态库链接静态库报错

完整的报错信息为：

\code{bash}
relocation R_X86_64_PC32 against symbol `_ZGVZN4pcpp8LoggerPP11getInstanceEvE8instance' can not be used when making a shared object; recompile with -fPIC
\endcode

这个问题原因一般是动态库链接了静态库导致的，一般来说动态库是不会链接静态库的，因为动态库遵循引用的规范，不会把静态库塞到自身之中。
如果是引用别人的库，那要改成引用shared库，如果是引用自己的库，那么在add_library中增加“SHARED”修饰。

\subsection cpp_issue_pthread undefined reference to symbol 'pthread_create@@GLIBC_2.2.5'

使用pthread库，需要编译配置中显示设置。

\code{cmake}
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

\endcode


\subsection cpp_issue_dso_missing DSO missing from command line

链接过程中，试图通过中间静态库链接进行传递，对于版本<2.2的ld，是可以的，但是对于>=2.2版本的ld，就是不行的。

什么是链接传递，举下面的例子：

1. 一个shared libA，定义了foo()函数
2. 一个静态库libB，显示链接了libA，
3. 另一个可执行文件binC，显示地链接libB

那么问：可执行文件binC能否调用foo()函数。


