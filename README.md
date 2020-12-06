# zk-note 一个基于Doxygen的Web笔记本

## 依赖

- sudo apt-get install doxygen graphviz

## 构建html文件

\code{bash}
\# 第一次构建，需要加上--all参数，--all有以下操作：
\# 1. 拷贝MathJax文件夹，拷贝MathJax非常耗费时间，不建议每次都拷贝
./build.sh --all

\# 如果已经使用build.sh --all构建过，后面只需要build.sh即可
./build.sh
\endcode

输出文档入口在/build/html/index.html
