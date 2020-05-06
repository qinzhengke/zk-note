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

\section ref_excel 引用excel表格
前言：markdown表格不支持合并单元格操作，表达能力有限，我们需要表达能力更强的html表格。
然而，手打html表格是一种低效操作，有位名人说过：“手打html表格是不可能手打的，这辈子都不可能手打的”。

下面介绍如何在doxygen中引用excel表格。

step 1: 在excel中编辑表格
step 2: 另存为->格式选择html，范围选择“已选择”->保存成html网页。
step 3: 使用文本文件打开html网页，复制<table></table>部分。
step 4：复制到doxygen源文件，注意：一定要使用\\htmlonly 和 \\endhtmlonly包裹，否则显示会出现错乱。
