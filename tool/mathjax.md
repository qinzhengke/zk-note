# MathJax：公式渲染工具

MathJax是一个提供公式渲染的工具，它的语法和Latex一样，但是不依赖于latex，不需要安装重量级的tex包（例如texlive和texlive-extra）。

MathJax的引用方式有两种：在线的CDN访问以及离线版本引用。


## Doxygen的MathJax本地化

在线的CDN访问并不是很靠谱，有的时候第三方CDN的连接很慢，甚至无法连接，另外，有的时候，我们在无网络的地方本地编辑，也希望能够渲染公式。

所以，我们仍然需要离线本地化的MathJax引用方式。

对于Doxygen来说，引用本地MathJax主要有以下步骤：

Step 1: 下载并精简MathJax

从官网上下载MathJax，注意，当前（2020-12）最新的MathJax3.0后使用typescript来渲染公式，而不再是javascript，而Doxygen只能支持后者，所以我们只能下载MathJax2的最新版本，我下载的是2.7.8。

MathJax的磁盘占用虽然不大，但是文件数量很多，一共3万多个小文件，这会使得后面的拷贝和“git add”操作特别的慢，严重影响开发效率，我们需要对其进行精简。

(1) 删除fonts/HTML-CSS下除了Tex的所有其他字体，我们不需要那么多字体

(2) 删除fonts/HTML-CSS/Tex下，除了woff的所有其他版本，我们只需要一种字体版本即可，光fonts/HTML-CSS/Tex/png 目录下就有2万9千个小文件。

(3) 删除localization目录下的所有文件夹，我们不需要那么多语言支持，系统默认使用英语。

(4) 删除unpack目录，里面的内容明显是整个MathJax根目录的重复。

(5) 删除jax/output目录下除了HTML-CSS的其他目录和文件，我们不需要其他的输出方式。

(6) 删除jax/output/HTML-CSS/fonts目录下，除了Tex的其他文件夹和文件，同样地，我们只需要一种字体就够了。

经过以上步骤精简后，MathJax的磁盘空间从65MB降低到了12MB，文件数量从3万2千降低到了401。


【小贴士】本仓库的3rd-party目录下就有我已经精简好的MathJax版本，至少对于本仓库doxygen项目，是可以正常使用的。

Step 2: Doxyfile配置

在Doxyfile中进行mathjax的相关配置：

```
USE_MATHJAX            = YES
MATHJAX_FORMAT         = HTML-CSS
MATHJAX_RELPATH        = ./MathJax-2.7.8
```

首先自然是打开MathJax总开关，然后设置MathJax的输出格式，再配置MathJax的路径。

注意，“./MathJax-2.7.8”路径的含义是：html目录下的index.html必须在同级目录下找到“MathJax-2.7.8”这个文件夹。
即MathJax文件夹必须拷贝到html目录下。

Step 3: 拷贝MathJax文件夹

想要输出的相关html文件能够访问MathJax，必须拷贝代码仓库的MathJax到build/html/目录下。

结合Doxygen的构建，我们可以使用单独的脚本来实现doxygen的文档构建、图片拷贝以及MathJax的拷贝，如下代码所示：

```bash

#!/bin/bash

ls -d ./build/html/* | grep -v MathJax | xargs rm -rf

echo "[zk] Building html ..."
doxygen Doxyfile

echo "[zk] Copying images ..."
cp -r figures build/html/

if [$# -gt 0 ]
then 

    if [$1 == '--all' ]
    then
        echo "[zk] Copying MathJax ..."
        cp -r ./3rd-party/MathJax-2.7.8 ./build/html/
    fi

fi


rm -rf ../qinzhengke.github.io/*

cp -r build/html/* ../qinzhengke.github.io/

echo "[zk] All done!"
```
