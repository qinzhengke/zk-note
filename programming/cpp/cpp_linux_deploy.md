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

<hr>
\section 方法二：聪明一点的方法

Step 1: 使用ldd工具查出所有需要的so文件

\code{.sh}
ldd /bin/ls
linux-vdso.so.1 =>  (0x00007fff87ffe000)
libselinux.so.1 => /lib/x86_64-linux-gnu/libselinux.so.1 (0x00007ff0510c1000)
librt.so.1 => /lib/x86_64-linux-gnu/librt.so.1 (0x00007ff050eb9000)
libacl.so.1 => /lib/x86_64-linux-gnu/libacl.so.1 (0x00007ff050cb0000)
libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007ff0508f0000)
libdl.so.2 => /lib/x86_64-linux-gnu/libdl.so.2 (0x00007ff0506ec000)
/lib64/ld-linux-x86-64.so.2 (0x00007ff0512f7000)
libpthread.so.0 => /lib/x86_64-linux-gnu/libpthread.so.0 (0x00007ff0504ce000)
libattr.so.1 => /lib/x86_64-linux-gnu/libattr.so.1 (0x00007ff0502c9000)
\endcode

Step2: 剩下的步骤和上一节方法一致。

<hr>
\section 方法三：更聪明的办法

通过sed工具，可以过滤出我们想要的文件名列表。

\code{.sh}
ldd /bin/ls | sed -e 's/\t//' | sed -e 's/.*=..//' | sed -e 's/ (0.*)//' | grep -e '/.*'
\endcode

sed -e 's/\t//' 意思是把制表符\t给删除（替换成无），语法和vim的替换是一样的。
sed -e 's/.*=..//' 意思是把“=”前面的所有字符和“=”后面的两个字符删除掉（替换成无）。
sed -e 's/ (0.*)//' 意思是" (0x-----)"都给删除掉。
grep -e '/.*'　表示选择以'/'为开头的字符串，也就是说把linux-vdso.so.1去掉，因为这个虚拟的库文件，由kenel提供，不需要拷贝。如果坚持拷贝的话，会出现找不到文件的提示。

输出
\code
linux-vdso.so.1
/lib/x86_64-linux-gnu/libselinux.so.1
/lib/x86_64-linux-gnu/libc.so.6
/lib/x86_64-linux-gnu/libpcre.so.3
/lib/x86_64-linux-gnu/libdl.so.2
/lib64/ld-linux-x86-64.so.2
/lib/x86_64-linux-gnu/libpthread.so.0
\endcode

Step2: 批量拷贝
\code{.sh}
ldd /bin/ls | sed -e 's/\t//' | sed -e 's/.*=..//' | sed -e 's/ (0.*)//' | xargs -I {} cp {} ~/softs/cc/lib/
\endcode

xargs是linux的一个工具，可以通过管道"|"获取文件列表，然后批量执行cp操作，其中“xarg -I {}”表示使用列表中的内容替换后面实际执行命令的{}的内容，类似于编程语言中的参数传递。

注意：拷贝的过程中会报出linux-vdso.so.1文件找不到，不用去管他，一般的linux系统都会有这个文件，不需要拷贝。