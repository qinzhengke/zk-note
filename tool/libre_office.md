开源办公软件：Libre Office{#libre_office}
======================================


LibreOffice是一款开源、免费，而且非常好用的Office软件，配合Texmath插件0.44画图很流啤。

## LibreOffice可以插入Latex公式吗？

回答：可以，使用TexMath插件，非常给力。

## TexMath插件0.46版本慢

每打开一次Texmath编辑器，都要加载符号列表UI，这个UI很鸡肋，没啥用，但是加载起来贼慢。
唯一的办法是使用0.44版本的插件，这个版本已经很完美了，没发现过什么bug。

## 打开TexMath编辑器报错

有时候公司电脑安全软件抽风，总是会对C盘做一些还原操作，会触发LibreOffice的某个bug，打开TexMath编辑器，会报错并且弹出VBA调试。
报错内容是，怎么办呢？

```
BASIC runtime error - "exception occured", Type: com.sun.star.container.NoSuchElementException.
```

回答：删除下面的文件夹然后重新安装TexMath。

```
C:\Users\Paul\AppData\Roaming\libreoffice
```

## Draw工具如何以圆心为基准点画圆弧？

回答：画圆弧的时候按住alt键即可，如果同时按住alt和shift键，就会画圆弧。
注意：一定要画完整个圆弧再松alt键，否则一松开alt键，软件还是会按照默认方式（边框）画圆弧。

## Draw工具的箭头尺寸如何缩小？

回答：右键选择`线条`，然后就可以编辑箭头大小。

## Draw工具使用什么字体，中英混合会比较好？

回答：微软雅黑
