Coredump分析{#coredump}
======================


\section 方法

Step 1: shell中设置生成coredump文件。

~~~{.sh}
#设置core大小为无限
ulimit -c unlimited
#设置文件大小为无限
ulimit unlimitedh}
~~~

以上设置仅本次有效，下次重启需要重新设置。

Step 2: 

下面修改/etc/sysctl.conf文件，对产生的core文件进行配置：

在文件末尾添加下面两行

\code{.sh}
kernel.core_pattern=/tmp/core-%e
kernel.core_uses_pid = 0
\endcode

Step 3: 执行命令进行配置

~~~{.sh}
sysctl -p /etc/sysctl.conf
~~~

Step 4: 运行coredump的可执行文件，coredump之后，coredump文件保存在/tmp目录，名称形式为core-my_exe


Step 5: 使用gdb打开可执行文件和coredump文件

~~~{.sh}
gdb ./my_exe /tmp/core-my_exe
~~~

Step 6: 在gdb中使用bt(backtrace)命令来追溯问题出现在哪里。

TODO qinzhengke :给出实际的案例。