Linux{#os_linux}
=====================

这个页面主要记录一些Linux操作系统的知识


\section 综合篇

\subsection 文件权限

参考下图：
![](./figures/../../../figures/file-llls22.jpg)

补充：对于文件来说，“x”表示是否可执行，但是对于文件夹来说，“x”代表是否可以进入。如果发现了“cd”到某个目录失败提示无权限，那就是这个原因。

\subsection ls输出的绿色背景文件是什么意思？

![](./figures/../../../figures/linux_ls_green.png)

表示除了owner和group之外，其他人员也拥有写的权限，是不合理的，保持其他人仅可读是多用户共享文件的合理设置。

\subsection 如何非阻塞运行gui程序？
问题：如果直接在终端运行某个gui程序，终端会阻塞直到gui关闭，例如运行gitk。
那么如何运行程序，并且不阻塞终端呢？
解法：

\code{bash}
gitk &
\endcode


\subsection 如何查看目录和文件占用磁盘大小？

\code{bash}
du -h -d 1 ./
\endcode

du表示disk usage，-h参数表示使用人类可读的单位，即KB,MB,GB为单位，如果不设置这个参数，得到的数字很奇怪，也不是以byte为单位的。-d表示递归的深度，如果只看当前目录，选择1，后面./就是目录位置。

这样的方式只会显示文件夹的大小，如果要显示文件的大小，用下面这个命令。

\code{bash}
du -h -d 1 ./*
\endcode


\sbusection 如何拷贝符号链接文件本身？

如果使用cp工具来拷贝符号链接(symbolic link)，那么最终拷贝的是符号链接指向的文件，有的时候我们只想拷贝符号链接文件本身。

例如使用cmake&make工具链来构建的库文件，一般会得到符号链接a.so和真正的库文件a.so.1.0.0，在打包的时候，我们仍然想要保持符号链接＋文件本身的使用方式。

如何实现呢？

很简单：cp -P


\subsection 如何查看哪些端口号被监听？

基于网络socket通信的时候，一个端口只能被一个应用程序监听，（实际上，端口的内在定义就是应用程序的标识）。

所以，在监听一个端口之前，如果这个端口已经被监听，那么再次监听会失败。

我们通常会想查看当前端口被那个应用程序监听，如何实现呢？

一种方式如下所示：
\code{.sh}
sudo lsof -i -P -n | grep LISTEN
\endcode


\subsection ls工具如何显示文件的全路径？

众所周知，ls工具会输出文件列表，但是有的时候我们想要的是文件的全路径，如何实现呢？

\code{.sh}
$ ls -d $PWD/*
\endcode


\subsection 如何查看文件的后面n行？

\code{bash}
tail -n 5 ./filename.txt
\endcode

\subsection  如何循环执行某个命令？

\code{bash} 
while : ; do tail -n 5 ./filename.txt; sleep 1; done;
\endcode

\subsection  ln -s 命令无效？
1. 使用ln -s命令，一定要输入目标文件的绝对路径，而不是相对路径！
2. 如果目标文件在NTFS磁盘上，那么这种连接是无效的！

\subsection  find命令
怎么查找名称带有“[”和“]”的文件？
\code{bash}
find . -name "\[xxx\]"
\endcode
find工具认为中括号有特定含义，即寻找从哪个字母到哪个字母开始的文件，如果确实要使用中括号，就应该使用转义字符。

\subsection  find + rm 命令
使用find + rm 命令可以轻松删除特定文件，而且先find一遍，可以看看是否会误删其他文件，确认没问题之后再使用rm，即安全也高效。

\code{bash}
#第一遍先看看会删除哪些文件
find . -name xxx
#确认没问题后，第二遍删除
find . -name xxx | xargs rm
\endcode

 压缩文件夹与解压文件

打包并压缩一个文件夹：

\code{.sh}
tar -zcf ${folder}.tar.gz ${folder}
\endcode{.sh}

解压一个tar.gz压缩包到当前目录：

\code{.sh}
tar -xf myfile.tar.gz
\endcode

如果压缩的时候是一个文件夹压缩的，那么解压出来的也是一个文件夹。


\subsection 几种Linux发送网络包的命令

UDP包：
\code{bash}
echo "This is my data" > /dev/udp/127.0.0.1/1053
\# 其中127.0.0.1就是IP地址，1053就是端口
\endcode

http包
\code{bash}
curl "http://127.0.0.1:9009/echo" -d "Hello, world"
\# 其中127.0.0.1是IP地址，9090是端口
\endcode



\subsection Linux上环境变量的配置

Linux操作系统下三种配置环境变量的方法　　 

现在使用linux的朋友越来越多了，在linux下做开发首先就是需要配置环境变量，下面以配置java环境变量为例介绍三种配置环境变量的方法。

1. 修改/etc/profile文件

如果你的计算机仅仅作为开发使用时推荐使用这种方法，因为所有用户的shell都有权使用这些环境变量，可能会给系统带来安全性问题。

(1)用文本编辑器打开/etc/profile

(2)在profile文件末尾加入：

\endcode
JAVA_HOME=/usr/share/jdk1.5.0_05
PATH=$JAVA_HOME/bin:$PATH
CLASSPATH=.:$JAVA_HOME/lib/dt.jar:$JAVA_HOME/lib/tools.jar
export JAVA_HOME
export PATH
export CLASSPATH
\endcode

(3)重新登录

a. 你要将 /usr/share/jdk1.5.0_05jdk 改为你的jdk安装目录

b. linux下用冒号“:”来分隔路径

c. `$PATH / $CLASSPATH / $JAVA_HOME` 是用来引用原来的环境变量的值,在设置环境变量时特别要注意不能把原来的值给覆盖掉了，这是一种常见的错误。

d. CLASSPATH中当前目录“.”不能丢,把当前目录丢掉也是常见的错误。

e. `export`是把这三个变量导出为全局变量。

f. 大小写必须严格区分。

2. 修改.bashrc文件　　

这种方法更为安全，它可以把使用这些环境变量的权限控制到用户级别，如果你需要给某个用户权限使用这些环境变量，你只需要修改其个人用户主目录下的.bashrc文件就可以了。

(1)用文本编辑器打开用户目录下的.bashrc文件

(2)在.bashrc文件末尾加入：　　

\endcode
set JAVA_HOME=/usr/share/jdk1.5.0_05
export JAVA_HOME
set PATH=$JAVA_HOME/bin:$PATH
export PATH
set CLASSPATH=.:$JAVA_HOME/lib/dt.jar:$JAVA_HOME/lib/tools.jar
export CLASSPATH
\endcode

(3)重新登录

3. 直接在shell下设置变量

不赞成使用这种方法，因为换个shell，你的设置就无效了，因此这种方法仅仅是临时使用，以后要使用的时候又要重新设置，比较麻烦。

只需在shell终端执行下列命令：

\endcode
export JAVA_HOME=/usr/share/jdk1.5.0_05
export PATH=$JAVA_HOME/bin:$PATH
export CLASSPATH=.:$JAVA_HOME/lib/dt.jar:$JAVA_HOME/lib/tools.jar 
\endcode

\subsection  查看滚动的log文件
\code{bash}
tail -f xxx.log
\endcode
参数“-f”表示“follow”，即当文件有更新的时候，追加打印更新的内容，这样一来，看log文件就和命令运行程序一样了。

\subsection  busybox
这是一个工具的集合，很多比较简陋的嵌入式linux环境无法提供太多的工具，busybox就作为一个工具箱，提供丰富的工具支持，例如上述的tail命令。

\subsection  script中的source
script中的source命令是无效的！
所以，如果想要在script中source ~/.bashrc，也是无效的。
有点奇怪，有些说法是calling bash不受callee bash的影响。
所以想要source ~/.bashrc可以这样

\code{bash}
# Add something to ~/.bashrc
echo MY_VARIABLE > ~/.bashrc
exec bash
\endcode
\subsection # /bin/sh&M bad interpreter
直接原因是第一行结尾字符有问题，一般的源头是bash的脚本文件在Windows被重新编辑过，Windows和Linux下的换行是不同的字符表达的，回到Linux时就可能出错。

\subsection # .netrc文件注册网络账户和密码
很多web应用需要用户提供账户和密码，如果一开始没有在web端设置好，或者说没有办法在web端设置，例如gerrit上http形式的仓库URL，交互程序一般会阻塞并弹出输入账户和密码的提示。
这样的交互有2点不好。
1.有些账户密码很长，还不能够自行设置，没错，说的就是gerrit的anomynous http地址，每次都要从gerrit上复制密码，降低效率。
2.自动化脚本，无法执行交互。
那有没有办法解决这个问题呢？其实是可以的，使用Linux自带的feature：.netrc文件，文件在`~`目录下
下面就是.netrc文件的内容示例。
\code{bash}
machine github.com login qinzhengke password xxxxx
machine 192.168.0.5 login qinzhengke password xxxx
\endcode


\subsection 无法delete到回收站
无法通过delete将NTFS磁盘中的文件移动到回收站
Gnome中无法通过delete将NTFS磁盘中的文件移动到回收站，只能完全删除？
解决方法：
在/etc/fstab文件中加入uid=1000，例如：

\code
/dev/sda5   /home/zrinker/Work/ ntfs uid=1000,default 0 0 
\endcode

注意，uid=1000是和default在通过逗号放在一起的，中间不能有空格，因为它们组成了一个参数，uid就是第一个用户的ID，一般是1000，如果不是第一个用户，则通过echo $UID来获取uid，修改完后重新启动即可。



\subsection Ubuntu中的apt-get命令
apt-get是Debian系列操作系统的软件安装工具，这个工具可以连接网络已有的软件库，下载并安装所有的开源软件包，下面记录常用的命令和选项。
1. install是最常用的，直接下载安装软件包并且下载安装依赖的所有其他软件包。
2. Install -d 可以用于下载二进制软件包而不安装。
3. 下好的软件包放在/var/cache/apt/archives里，而正在下载的软件包放在 /var/cache/apt/archives/partial。
4. 使用apt-get clean可以清空上面两个文件夹。



\subsection Ubuntu自带的任务管理器
Monitor，只要在super键后输入monitor就可以查找得到了。 


\subsection 修改Linux的root密码
登录root账户后，使用passwd root命令来更改密码



\subsection ssh完整登录命令（账户、主机、端口都有）
ssh root@104.224.156.77 -p 27217



\section 软件篇



\subsection 常用软件列表
- Sogou Pinyin
第一步当然是安装一个合适的中文输入法了。作为国内比较良心作品，sogou拼音相当不错。

- Qt
Qt是跨平台C++的集成开发环境里几乎最好的。

- TexLive & TexStudio
搞学术用的，用LaTex写论文，TexStudio+TexLive非常好用，跨平台这点很难得。

- Spyder
Python的集成开发环境，支持调试，可以算是最好的。

- Virtualbox

- Windows还是必须得要的，QQ、doc文档这些只能在Windows下使用。如果不装双系统，那么就装一个虚拟机吧，特别是挂QQ的时候。

- Wiz Note
这个笔记软件特有的Linux客户端，Linux用户真是感动，特别是最近更新了插入代码，码农用起来算爽。

- Chrome
这个浏览器不说了，最好用的浏览器了。

- Gimp
类似于Photoshop的图片处理软件，用于图像分析和修改，很实用的工具。



\subsection Ubuntu下安装Shadowsocks
通过PPA源安装，仅支持Ubuntu 14.04或更高版本。

\code
sudo add-apt-repository ppa:hzwhuang/ss-qt5
sudo apt-get update
sudo apt-get install shadowsocks-qt5
\endcode

\subsection Ubuntu下安装Qt
从官网上下载.run文件之后，使用chmod u+x xxx.run命令来将文件变成可执行文件，然后再使用./xxx.run来运行安装程序。

![](files/安装Qt.png "安装Qt")



\subsection Linux上软件源码编译通用方法
首先需要配置make文件所需要的平台相关变量，例如有什么其他依赖的库？这些库的路径是什么？
例如安装GSL库第一步：“./configure“，configure里包含着安装的目录在哪里，一般命名为install_prefix，Linux下默认安装在/usr/local目录里。

然后执行make命令，make自动寻找当前目录下的MakeFile文件，调用编译器对源代码进行编译。

最后使用make install来将编译好的二进制库文件复制到安装目录，并且添加环境变量。

卸载库：以GSL为例，只要在源码目录运行sudo make uninstall即可。



\subsection 给Ubuntu系统安装宋体字体

Windows平台下，“宋体”、“微软雅黑”、“Courier New(编程字体)”用的比较多，看的也习惯了。那如何在 Ubuntu下也安装这些字体呢？

操作步骤如下：

- 第一步：从 Windows 7 系统下字体文件夹（C:\Windows\Fonts） ，拷贝如下文件到当前Ubuntu用户目录 ～/123
宋体：simsunb.ttf 和 simsun.ttc

微软雅黑：msyhbd.ttf
Courier New：courbd.ttf、courbi.ttf、couri.ttf 和 cour.ttf
WPS Office 所需字体：wingding.ttf、webdings.ttf、symbol.ttf、WINGDNG3.TTF、WINGDNG2.TTF、MTExtra.ttf

- 第二步：新建字体存放目录 windows-font

\code
sudo mkdir /usr/share/fonts/truetype/windows-font
\endcode

- 第三步：拷贝字体到wiondow-font目录下

\code	
sudo cp /home/php-note/123/* /usr/share/fonts/truetype/windows-font
\endcode

- 第四步：修改权限，并更新字体缓存

\code	
sudo chmod -R 777  /usr/share/fonts/truetype/windows-font
cd /usr/share/fonts/truetype/windows-font
sudo mkfontscale
sudo mkfontdir
sudo fc-cache -fv
\endcode

- 第五步：重启下系统吧！
\code	
sudo reboot
\endcode



\subsection 中文输入法安装
Gnome 3.10下的中文输入法：
	Linux下的输入法不同于Windows，Linux的输入法分为：输入法框架+输入法两个部分，首先我们需要安装好输入法框架，然后在基于不同的框架安装不同的输入法。
	Linux的输入法框架主要有两种：ibus和fcitx，ibus即俗称的小企鹅输入法，是Ubuntu自带的输入法框架，但是网络上普遍认为ibus并不是很好用，很多情况都使用fcitx取代ibus，搜狗输入法就是一个典型的例子，它只支持fcitx框架。
	之前在Ubuntu13.10里装过fcitx-sougoupinyin输入法，还挺好用的，现在转换到了Ubuntu Gnome 14.04，用的是Gnome 3.10，需要重新安装输入法。一开始我仍然想安装fcitx，但是发现gnome 3.10 无法识别fcitx的输入法（ibus的输入法就能正常的识别）。
	尝试了一下决定还是使用ibus吧（后来发现ibus也没那么不好用），下面是ibus拼音输入法的安装流程：
1. 一般情况下Ubuntu发行版都是默认已经安装了ibus的，如果没有安装，就执行下面语句进行安装：sudo apt-get install ibus
2. 安装ibus拼音输入法或者ibus谷歌拼音输入法，下面两者二选一（我觉得谷歌拼音输入法不是很好用，可能是受了搜狗输入法的影响吧）：sudo apt-get install ibus-pinyin 或者 sudo apt-get install ibus-googlepinyin
3. 在Gnome里设置“输入源”，是的没错，这里是”输入源“而不是”输入法“，在Gnome 3.10 中，在Setting --- Region & Language --- Input Sources里，选择左下角的加号添加输入源，找到Chinese开头的几个输入法，例如下图，我安装了ibus-pinyin 和ibus-googlepinyin，在下图中分为Chinese(Bopomofo)和Chinese(GooglePinyin)，注意Chinese选项默认就有，但是实际上这个是输入不了中文的。

![](files/选择中文输入.png "选择中文输入法")

4. 注销后重新登录即可，另外Gnome里默认的切换输入法的快捷键是super+space而不是ctrl+space。


\subsection Ubuntu中的Apt-get命令

apt-get是Debian系列操作系统的软件安装工具，这个工具可以连接网络已有的软件库，下载并安装所有的开源软件包，下面记录常用的命令和选项。
1. install是最常用的，直接下载安装软件包并且下载安装依赖的所有其他软件包。
2. Install -d 可以用于下载二进制软件包而不安装。
3. 下好的软件包放在/var/cache/apt/archives里，而正在下载的软件包放在 /var/cache/apt/archives/partial。
4. 使用apt-get clean可以清空上面两个文件夹。


\subsection flameshot-Ubuntu下一个超强截图工具
安装
\code{bash}
sudo apt-get install flameshot
\endcode

使用：
1. windows键，输入keyboard，进入键盘设置
2. 将print(print screen)键解绑，然后添加新的print键快捷操作“flameshot gui”
3. 按print键，享受截图、涂鸦、注释的快感吧。


\section 搜狗输入法切换全角半角以及中英标点符号

在linux上，一般输入法会默认全角，打出来的英文字符很大，而且不是普通的英文字符，无法被匹配到，这时候需要切换为半角，不太懂全角的意义是什么。

全角半角切换方法：shift+space

切换中英标点符号方法：ctrl+.



\section 启用ssh服务

前言：为了安全，一般桌面版的linux操作系统是默认不安装ssh服务的，如果要基于ssh连接到某台桌面linux电脑，需要手动设置。

方法：

\code{.sh}
sudo apt update
# 安装ssh-server
sudo apt install openssh-server
# 自启动ssh服务
sudo systemctl status ssh
# 设置防火墙白名单，否则无法连接
sudo ufw allow ssh
\endcode


\section 使用scp发送文件

\code{.sh}
scp my_file username@192.168.1.101:~/Downloads
\endcode


\section 根据进程的名称来kill

前言：kill命令没有根据进程的名字来杀死进程的方法，需要自行写几句脚本根据进程名称找出PID，然后在杀死，具体方法如下所示。

\code{.sh}
pid=$(top -n 1 -b | grep -e 'node' | grep -e '[0-9]*' -o | head -1)
if [ ! -z "$pid" ]
then
kill $pid
echo "node stopped"
fi
\endcode

其中：

(1) `top -n 1 -b` 表示显示所有进程，并且不进入交互界面，直接printf所有进程。

(2) `grep -o` 表示把所有匹配出来的结果列出来，列出时每一个match的内容会占一行。

(3) `head -1` 表示取第一行的内容。