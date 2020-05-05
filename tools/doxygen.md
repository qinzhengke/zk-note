文档框架：doxygen{#doxygen}
=========================

\section include和snippet
使用的时候必须在EXMAPLE_PATH里加入源码出现的目录，否则doxygen无法搜索到。
doxygen会递归搜索在EXAMPLE_PATH的子目录，所以如果有多个目录下的源文件，只要添加一个父目录就行。

\section 生成latex输出
http://www.doxygen.nl/manual/output.html

\section make pdf出现 “tuba.sty not found“
sudo apt-get install texlive-latex-extra

\section section_usage section用法

\\section <name> title

其中<name>是名称，类似于编程语言里的规则，不能使用+号，也不能有空格，title就是对应的文本，就是要显示的东西。

\section dox_file dox文件语法

.dox文件是doxygen特有的文件，默认情况下当做C++语言处理，所以在dox文件中直接定义一些\\section命令是无效的，
想要使用\\section等特性，还是在.md文件中写吧。

\section doxygen_bilibili 插入哔哩哔哩视频

1. 点击视频分享，拷贝嵌入代码

2. 嵌入代码没有加入传输协议，需要手动添加“https:”

3. doxygen 使用 \\htmlonly 和 \\endhtmlonly来包裹嵌入代码。

~~~{.dox}
\htmlonly
<iframe src="https://player.bilibili.com/player.html?aid=97804448&bvid=BV1fE411w7ac&cid=166959951&page=1" scrolling="no" border="0" frameborder="no" framespacing="0" allowfullscreen="true"> </iframe>
\endhtmlonly
~~~
