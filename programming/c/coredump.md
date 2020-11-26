Coredump分析{#coredump}
======================


\section 方法

Step 0: CMakeLists.txt加入debug配置

\code{.cmake}
set(DEFAULT_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "{CMAKE_CXX_FLAGS} -o0 -ggbd")  #添加语句
add_compile_options(-g)   #添加语句
\endcode

Step 1: shell中设置生成coredump文件。

\code{bash}
#设置core大小为无限
ulimit -c unlimited
#设置文件大小为无限
ulimit unlimitedh}
\endcode

以上设置仅本次有效，下次重启需要重新设置。

Step 2: 

下面修改/etc/sysctl.conf文件，对产生的core文件进行配置：

在文件末尾添加下面两行

\code{.sh}
kernel.core_pattern=/tmp/core-%e
kernel.core_uses_pid = 0
\endcode

Step 3: 执行命令进行配置

\code{bash}
sysctl -p /etc/sysctl.conf
\endcode

Step 4: 运行coredump的可执行文件，coredump之后，coredump文件保存在/tmp目录，名称形式为core-my_exe


Step 5: 使用gdb打开可执行文件和coredump文件

\code{bash}
gdb ./my_exe /tmp/core-my_exe
\endcode

Step 6: 在gdb中使用bt(backtrace)命令来追溯问题出现在哪里。

TODO qinzhengke :给出实际的案例。

<hr>
\section 问题定位到库文件中

当coredump出现在库文件中时，如果没有在编译库文件的时候加入debug设置，那么backtrace给出的结果也只能定位到哪个so文件。

如果想要定位到哪个库的源文件的哪一行，需要编译库文件的时候也加入debug配置

\code{.cmake}
set(DEFAULT_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "{CMAKE_CXX_FLAGS} -o0 -ggbd")  #添加语句
add_compile_options(-g)   #添加语句
\endcode

<hr>
\section coredump的几大主要原因

1. 数组越界

2. 非法指针引用

3. 类成员函数调用

这个案例很诡异，真实遇见过，一个未初始化的类指针，执行了成员函数，错误并没有在一开始调用就出现，而是出现在函数内部，递归调用子函数时才报出来。

如果没有全局观，那么就会很难理解为什么调用一个函数就会出错，并且根本没有进入出错的函数。

没有进去就报错，这点很容易理解，函数指针都是错误的，肯定进不去就会报错。

最不可理解的就是一开始调用没有出错，这点还无甚解。

\code{.cpp}
class A{
public:
    void sub_func(){}

    func(){
        sub_func(); // 这里报错，还没进去就报错了。
    }
};

A *pa;  // 注意，这里没有分配内存
pa->func();　// 这里没报错
\endcode