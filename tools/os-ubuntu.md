Ubuntu操作系统{#os_ubuntu}
========================

这里只讲一些和ubunut相关的笔记，如果是linux通用的问题，则在 \subpage os_linux 里说明。

\section 快捷键

1. 最大化窗口：Windows键+上箭头，（Windows+下箭头则是恢复）

2. 在相同的程序但是不同的窗口之间切换：Windows+'`'

<hr>
\section 设置自启动

本方法只对ubuntu18.04以及以上的版本。

步骤1： 新建一个脚本设置为可执行，然后添加以下内容
\code{.sh}
#!/bin/bash' 
\subsection BEGIN INIT INFO' 
# Required-Start: $local_fs $network $remote_fs' 
# Required-Stop: $local_fs $network $remote_fs' 
# Default-Start:  2 3 4 5' 
# Default-Stop: 0 1 6' 
\subsection END INIT INFO' 
cd somewhere
./pm_msft.sh"
\endcode

注意，其中注释部分是不能少的，cd到somewhere目录后，这个目录就是workding directory，直接运行该目录下的程序即可，和我们在终端操作是一样的。

步骤2：拷贝脚本到`/etc/init.d`目录下

