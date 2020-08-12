C++可执行文件在Linux的发布{#cpp_linux_deploy}
==========================================


\section 方法一：笨办法

Step 1: 准备虚拟机

准备一台目标平台虚拟机，不要装任何库，模拟用户的环境。

Step 2: 拷贝可执行文件

把可执行文件拷贝到虚拟机中，例如虚拟机的~/my_app/my_exe

Step 3: 列出依赖的so文件

使用下面的命令运行程序

\code{.sh}
LD_LIBRARY_PATH=$(pwd)/lib ./my_exe
\endcode

前半句的意思是指定程序搜索so文件的额外路径，可执行文件会在对应的目录下搜索so文件，也就是my_app/lib目录，当然这个目录可以随意设置，只有里面有so文件就行。

运行my_exe后，一般情况下都会报错，提示找不到对应的so库。
这时候在主机里（也就是编译程序的环境）查找对应的so文件。
推荐使用“locate”命令查找，特别快，如下面的例子。

\code{.sh}
locate libopencv_core.so.3
\endcode

如果找到了多处so文件实例，（通常一些portable程序会自带so文件，正如我们要发布的那样），尽量使用系统目录下的，下面举几个例子。

\code{.sh}
/usr/lib
/usr/local/lib
/usr/lib/x86_x64-linux-gnu
\endcode

Step 4: 拷贝so文件再运行

把对应的so文件拷贝到虚拟机的~/my_app/lib/目录，并且按照step 3运行程序。

这时候会发现，刚才的缺失对应so文件的提示消失了，但是大部分情况下，系统会提示缺少其他的so文件。
这时候需要重复进行Step 3 到Step 4，直到程序可以运行为止。

Step 5: 建立run.sh脚本运行程序

让用户记住如此特殊的运行命令是不友好的，建立一个run.sh让用户一键运行。

\code{.sh}
#!/bin/bash
LD_LIBRARY_PATH=$(pwd)/lib ./my_exe
\endcode

后记：
这方法的“笨”在于需要重复查找缺失的so文件，并且只能串行查找，稍微花点时间。