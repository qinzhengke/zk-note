Ubuntu18.04{#os_ubuntu}
========================

这里只讲一些和ubunut相关的笔记，如果是linux通用的问题，则在 \subpage os_linux 里说明。

\section 快捷键

1. 最大化窗口：Windows键+上箭头，（Windows+下箭头则是恢复）

2. 在相同的程序但是不同的窗口之间切换：Windows+'`'


\section 设置自启动

Ubuntu18.04开始，系统开机启动方式有了较大改动，本方法只对ubuntu18.04以及以上的版本。

步骤1： 新建一个脚本设置为可执行，然后添加以下内容
\code{.sh}
#!/bin/bash 
# BEGIN INIT INFO
# Required-Start: $local_fs $network $remote_fs
# Required-Stop: $local_fs $network $remote_fs
# Default-Start:  2 3 4 5
# Default-Stop: 0 1 6
# END INIT INFO
cd somewhere
./pm_msft.sh"
\endcode

注意，其中注释部分是不能少的，cd到somewhere目录后，这个目录就是workding directory，直接运行该目录下的程序即可，和我们在终端操作是一样的。

步骤2：拷贝脚本到`/etc/init.d`目录下

步骤3:
```
systemctl enable my_script
```


\section ubuntu_issue_stuck ubuntu18.04卡在登录界面，鼠标动不了

又遇到一个坑点，重启后卡在登录界面，鼠标和键盘没有响应，界面本身是没有卡死的，因为界面上的时间会发生变化。

网上的一个解决方法，也不知道原理，不想探究了。

1. 先回到操作系统选择界面，选择recovery模式登录
2. recovery模式默认不开网络，需要在recovery界面选择“enable network”
3. 选择“root”，进入系统。
4. 输入以下命令

```
sudo apt install xserver-xorg-input-all
```