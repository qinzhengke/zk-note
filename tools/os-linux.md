# Linux

这个页面主要记录一些Linux操作系统的知识

## 如何非阻塞运行gui程序？
问题：如果直接在终端运行某个gui程序，终端会阻塞直到gui关闭，例如运行gitk。
那么如何运行程序，并且不阻塞终端呢？
解法：

~~~{.sh}
gitk &
~~~

## 如何查看目录和文件占用磁盘大小？

~~~{.sh}
du -h -d 1 ./
~~~

du表示disk usage，-h参数表示使用人类可读的单位，即KB,MB,GB为单位，如果不设置这个参数，得到的数字很奇怪，也不是以byte为单位的。-d表示递归的深度，如果只看当前目录，选择1，后面./就是目录位置。

这样的方式只会显示文件夹的大小，如果要显示文件的大小，用下面这个命令。

~~~{.sh}
du -h -d 1 ./*
~~~

## 如何查看文件的后面n行？

~~~{.sh}
tail -n 5 ./filename.txt
~~~

## 如何循环执行某个命令？

~~~{.sh} 
while : ; do tail -n 5 ./filename.txt; sleep 1; done;
~~~

## ln -s 命令无效？
1. 使用ln -s命令，一定要输入目标文件的绝对路径，而不是相对路径！
2. 如果目标文件在NTFS磁盘上，那么这种连接是无效的！

## find命令
怎么查找名称带有“[”和“]”的文件？
```sh
find . -name "\[xxx\]"
```
find工具认为中括号有特定含义，即寻找从哪个字母到哪个字母开始的文件，如果确实要使用中括号，就应该使用转义字符。

## find + rm 命令
使用find + rm 命令可以轻松删除特定文件，而且先find一遍，可以看看是否会误删其他文件，确认没问题之后再使用rm，即安全也高效。
```.sh
#第一遍先看看会删除哪些文件
find . -name xxx
#确认没问题后，第二遍删除
find . -name xxx | xargs rm
```

## 查看滚动的log文件
```.sh
tail -f xxx.log
```
参数“-f”表示“follow”，即当文件有更新的时候，追加打印更新的内容，这样一来，看log文件就和命令运行程序一样了。

## busybox
这是一个工具的集合，很多比较简陋的嵌入式linux环境无法提供太多的工具，busybox就作为一个工具箱，提供丰富的工具支持，例如上述的tail命令。

## script中的source
script中的source命令是无效的！
所以，如果想要在script中source ~/.bashrc，也是无效的。
有点奇怪，有些说法是calling bash不受callee bash的影响。
所以想要source ~/.bashrc可以这样

```.sh
# Add something to ~/.bashrc
echo MY_VARIABLE > ~/.bashrc
exec bash
```
### /bin/sh&M bad interpreter
直接原因是第一行结尾字符有问题，一般的源头是bash的脚本文件在Windows被重新编辑过，Windows和Linux下的换行是不同的字符表达的，回到Linux时就可能出错。
