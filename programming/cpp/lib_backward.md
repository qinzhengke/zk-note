backward：程序崩溃调试神器{#cpp_lib_backward}
===========================

backward这个库用来定位程序crash的地方，并且以源代码行号的形式给出，特别的直观，提升debug效率。
但是这个库的用法我感觉作者说的不是很清楚，这里需要以cmake工程为例子做笔记。

使用步骤

1. 安装libdw
~~~{.sh}
sudo apt-get install libdw-dev
~~~

2. 链接dw库
~~~{.sh}
target_link_libraries(exe dw)
~~~

3. 在main()函数所在的文件包含backward头文件，并且加入DW的宏定义，因为我文件内部会定义变量并且运行实例，
所以尽量不要在非main函数所在文件以外的地方调用，防止多次引用。

~~~{.cpp}
#define BACKWARD_HAS_DW 1
#include <backward.hpp>

// 下面这段必须加，否则输出结果仍然不是行号
namespace backward
{
	backward::SignalHandling sh;
}
~~~

4. Debug编译方式
别忘了一定要用Debug编译方式，否则可执行文件无法对应到行。

~~~{.cmake}
set(DEFAULT_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "{CMAKE_CXX_FLAGS} -o0 -ggbd")  #添加语句
add_compile_options(-g)   #添加语句
~~~

注意：目前发现同样的错误，有的时候backtrace会出现，有的时候则不会，非常地奇怪，原理还有待了解，
用的时候注意一下，多触发几次core dump，就有可能出现。