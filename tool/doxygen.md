# 文档框架：doxygen

# include和snippet
使用的时候必须在EXMAPLE_PATH里加入源码出现的目录，否则doxygen无法搜索到。
doxygen会递归搜索在EXAMPLE_PATH的子目录，所以如果有多个目录下的源文件，只要添加一个父目录就行。

# 生成latex输出
http://www.doxygen.nl/manual/output.html

# make pdf出现 “tuba.sty not found“
sudo apt-get install texlive-latex-extra

# section_usage section用法

\# <name> title

其中<name>是名称，类似于编程语言里的规则，不能使用+号，也不能有空格，title就是对应的文本，就是要显示的东西。

# dox_file dox文件语法

.dox文件是doxygen特有的文件，默认情况下当做C++语言处理，所以在dox文件中直接定义一些\#命令是无效的，
想要使用\#等特性，还是在.md文件中写吧。

# doxygen_bilibili 插入哔哩哔哩视频

1. 点击视频分享，拷贝嵌入代码

2. 嵌入代码没有加入传输协议，需要手动添加“https:”

3. doxygen 使用 \\htmlonly 和 \\endhtmlonly来包裹嵌入代码。

code{dox}
\\htmlonly
<iframe src="https://player.bilibili.com/player.html?aid=97804448&bvid=BV1fE411w7ac&cid=166959951&page=1" scrolling="no" border="0" frameborder="no" framespacing="0" allowfullscreen="true"> </iframe>
\\endhtmlonly
```

# ref_excel 引用excel表格
前言：markdown表格不支持合并单元格操作，表达能力有限，我们需要表达能力更强的html表格。
然而，手打html表格是一种低效操作，有位名人说过：“手打html表格是不可能手打的，这辈子都不可能手打的”。

下面介绍如何在doxygen中引用excel表格。

step 1: 在excel中编辑表格

step 2: 另存为->格式选择html，范围选择“已选择”->保存成html网页。

step 3: 使用文本文件打开html网页，复制<table></table>部分。

step 4：复制到doxygen源文件，注意：一定要使用\\htmlonly 和 \\endhtmlonly包裹，否则显示会出现错乱。


# numbered_eq 带编号的公式
Doxygen使用了MathJax来渲染公式，实际上Doxygen设置编号也就是MathJax设置编号。

step 1: 建立一个js文件，存放MathJax的配置，如下所示：

\verbatim
// latex_support.js
MathJax.Hub.Config({
    extensions: ["tex2jax.js"],
    jax: ["input/TeX", "output/HTML-CSS"],
    tex2jax:{
        inlineMath: [["\\(", "\\)"]],
        displayMath: [['$$','$$'],["\\[","\\]"]],
        processEscapes: true
    },
    "HTML-CSS": {fonts: ["TeX"]},
    TeX: {
        extensions: ["AMSmath.js", "AMSsymbols.js", "noErrors.js", "noUndefined.js"],
        equationNumbers: {autoNumber: "all"},
    }
});
\endverbatim

具体的原理好像是先转成latex，具体太复杂，官网讲公式编号配置的篇幅用十几次滚轮都看不完，就不去深究了。

step 2: 在doxygen配置文件中引用该js文件，如下所示：

```bash
MATHJAX_CODEFILE = ./latex_support.js
```

然后就可以愉快的使用带编号的公式。


# ref_eq 引用公式

\verbatim
    $$
        \label{eq}
        E = mc^2
    $$
    \\eqref{eq}
\endverbatim

注意是引用处使用双反斜杠。


# raw_format 原始格式输出

使用 \\verbatim 和 \\endverbatim包裹。


# majax_issue github page上MaJax公式没有正确渲染

明明离线版本Majax公式能够正常渲染，但是传到github page上就不行了？
一个很重要的原因就是Doxyfile里面Majax的CDN地址使用http作为传输协议，github page上应该是禁止了http协议，导致CDN没法正常访问。

解决办法很简单，只要把CDN地址中的“http”改成“https”即可。


# boldsymbol_vs_mathbf boldsymbol和mathbf的区别。

boldsymbol和mathbf都是latex中粗体的语法，但是boldsymbol明显要比textbf好看很多，如下图所示
<center>
![](./figures/boldsymbol_vs_mathbf.png)
</center>


# dfrac_vs_frac dfrac vs frac

dfrac明显尺寸要还原得更好一些，如下图所示。
![](./figures/dfrac_vs_frac.png)


# 生成参考文献

step 1: 安装texbib工具

```
sudo apt-get install texlive-bibtex-extra
```

step 2: 在doxygen配置文件中添加要显示的bib文件

```
CITE_BIB_FILES         = cv/cv.bib
```

step 3: 在bib文件中放置参考文件引用代码，引用代码可以从各种文献搜索引擎找到，例如谷歌学术。

```
// cv/cv.lib

@inproceedings{rosten2006machine,
  title={Machine learning for high-speed corner detection},
  author={Rosten, Edward and Drummond, Tom},
  booktitle={European conference on computer vision},
  pages={430--443},
  year={2006},
  organization={Springer}
}
```

step 4: 在doxygen文档处使用 \\cite 进行引用。

\verbatim
图：FAST角点原理示意图，图像来源为 \cite rosten2006machine 。
\endverbatim

结果如图所示：

![](./figures/doxygen_cite_bib_01.png)

![](./figures/doxygen_cite_bib_02.png)

# 灵活使用自定义css文件调整细节

使用“doxygen -w”来生成默认的css模板，然后在Doxyfile中配置“HTML_EXTRA_STYLE_SHEET”参数指定刚才生成的css文件。

然后就可以愉快的修改里面的内容了。

## 调整title的尺寸

在css文件中搜索“projectname”，调整里面的font的比例就可以了，默认是300%，贼大。