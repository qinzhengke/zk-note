lib_backward{#lib_backward}
===========================

backward这个库用来定位程序crash的地方，并且以源代码行号的形式给出，特别的直观，提升debug效率。
但是这个库的用法我感觉作者说的不是很清楚，这里需要以cmake工程为例子做笔记。

使用步骤

1. 安装libdw
~~~{.sh}
sudo apt-get install libdw-dev
~~~
2. add_executable指令加入启动backward库的宏
~~~{.sh}
add_executable(exe main.cpp ${BACKWARD_ENABLE})
~~~

3. 链接dw库
~~~{.sh}
target_link_libraries(exe dw)
~~~

4. 在包含backward头文件的时候加入DW的宏定义
~~~{.cpp}
#define BACKWARD_HAS_DW 1
#include <backward.hpp>

// 下面这段必须加，否则输出结果仍然不是行号
namespace backward
{
	backward::SignalHandling sh;
}
~~~

5.Debug编译方式
别忘了一定要用Debug编译方式，否则可执行文件无法对应到行。
