Ubuntu操作系统{#os_ubuntu}
========================

这里只讲一些和ubunut相关的笔记，如果是linux通用的问题，则在 \subpage os_linux 里说明。

\section 快捷键

1. 最大化窗口：Windows键+上箭头，（Windows+下箭头则是恢复）

2. 在相同的程序但是不同的窗口之间切换：Windows+'`'


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


\subsection 磁盘开机自动挂
磁盘开机挂载问题，Linux开机后，除了“/”目录和swap，其他的分区是不会自动挂载的，就像如果开机后第一次点击原Windows下的D盘，那么打开的时间会稍微久一些，而且文档管理的图标上会有一个小三角（Ubuntu）表示已经挂载。
我将log和stuffs分别做了快捷链接，但是由于这两个文件处于我的Work分区，开机没有挂载，所以开机后直接在terminal里输入快捷链接，还不能打开log和sutffs文件，所以现在必须试试开机挂载了。打开etc/fstab这个文件

Step 1: 查看磁盘的UUID

方法一：Ubuntu下使用“Disks”工具，Windows键+“disks”，可以查看某个磁盘的UUID

方法二：使用gparted查看磁盘UUID

Step 2: 编辑fstab文件

\code
sudo vim /etc/fstab
\endcode

在最后一行加入如下配置：

\code
UUID=xxxxxxx       /media/Work/    auto    defaults 0       2
\endcode

【注意】“defaults”值不要拼错了，有“s”，最后一个参数要填“2”，表示普通分区，并非Linux启动分区，这两设置不对，有可能无法开机。

【注意】如果错误设置无法开，不用惊慌，只要在Ubuntu启动界面，进入recovery模式，然后进入root模式，使用vim编辑/etc/fstab文件即可。


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