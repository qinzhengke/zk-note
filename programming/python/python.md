编程语言：Python{#python}
===========================

此页面记录一些有用的标准python语法。


\section for循环中同时使用序号和值
一般情况下，for循环支持使用序号（像c++一样）或者使用值来遍历数组，但是如果需要同时用值和序号的时候有没有优雅的表达呢？答案是有的，使用enumerate语句。

~~~{.py}
x = [1, 4, 9 16]
for i,x in enumerate(x):
	print(i, x)
~~~

\section 数组和常数相加
使用Python内置数组是做不到的，会报错，但是转换成np.array之后就可以了。

\section 生成等差数列
~~~
#方法一：给定起点、终点和差
numpy.arange(0, 1, 0.001) # [0,1)区间，以0.01为步长
#方法二：给定起点、终点和点数
numpy.linspace(0,1,100) #[0,1)区间取100个数
~~~

### 读取csv文件

读取csv文件的方式有很多种，标准库、numpy、panda都能提供，但是我觉得最方便的还是numpy提供的方法：

```python
from numpy import genfromtxt
my_data = genfromtxt('my_file.csv', delimiter=',')
```



### 读取csv文件

读取csv文件的方式有很多种，标准库、numpy、panda都能提供，但是我觉得最方便的还是numpy提供的方法：

```python
from numpy import genfromtxt
my_data = genfromtxt('my_file.csv', delimiter=',')
```



### No module name "xxx"

明明在解释器里能import的模块为什么在.py文件中无法找到呢？可能是因为python2和python3的原因，python2的解释器能找到对应模块，但是该模块没有安装python3的版本，如果用python3运行脚本自然无法找到。

### notepad++简单grep工具
有些嵌入式的程序无法在线debug，只能通过输出log的方式调试。但是如果一个程序有非常多的模块，每个模块都有自己输出的信息，那么这个log会非常的繁杂，一大堆和自己无关的信息，通过在linux下可以通过grep命令进行过滤，这种方式需要打一行命令，而且在Windows下无法直接使用。在这里我自己写了一个简单的notepad++“小插件”，只要一个快捷键就可以弹出新的标签页并且显示grep的内容。
下面是python代码：

```cpp
# -*- coding: utf-8 -*-
import sys
import os
fin = open(sys.argv[1], 'r')
lines = fin.readlines()
fin.close()
key = sys.argv[2]
fout = open(sys.argv[1]+'.find_'+key,'w')
for line in lines:
  if line.find(key) != -1:
    fout.write(line)
fout.close()
# 调用 notepad++ 显示处理好的文件。
os.system('D:\\softs\\npp.7.4.2.bin.x64\\notepad++.exe ' + sys.argv[1]+'.find_'+key)
```
然后在notepad++的‘run’菜单中添加运行脚本，其中当前标签页文件对应的目录由$(FULL_CURRENT_PATH)提供，别忘记还有第二个参数，就是要查找的关键字，这里是qzk，设定好快捷键，例如F6之后，就可以一键grep啦！

```shell
python D:\tools\np_grep\np_grep.py $(FULL_CURRENT_PATH) qzk
```

### 一个很蠢的问题

```python
a=[1,2,4,8,16,32]
for i in range(len(a)):
  a[i] = a[i] - a[0]
print(a)
```
我的本意是把一个数组全都减掉第一个元素，得到所有元素相对于第一个元素的offset，但是发现上述代码出错。
搞了一下发现自己太蠢了，a[0]元素在第一操作后就变成了0，后面所有操作都是错误的！

### python 变量类型
python有个很不习惯的地方，就是所有函数在声明或者是reference的时候从来不会说明参数和返回值类型，这让人很难把控
例如下面的教程代码

```python
import urllib.request
rp = urllib.request.openurl('http://baidu.com')
res = rp.read().decode('utf-8')
print res
```

这段代码是一个简单的抓取网页教程，但是我就一直搞不懂，rp到底是什么类型，为什么会有read()方法，这个read()方法又是什么鬼，这个read()返回的又是什么类型，为什么会有decode方法，特别蛋疼。
查官方的reference，只能得到这非常随意的描述，然后那个context一查完全不知道在说什么。。。。

`
> This function always returns an object which can work as a context manager and has methods such as

网上查了很久，貌似只有很少的人有这样的疑问？
最终的方式是使用type函数现场查看，即

```python
print(type(rp))
print(type(res))
```

\section 限制浮点数精度
前沿：在print或者matplotlib输出是，过长的小数会让人特别烦恼。
限制浮点数精度方法如下：
~~~
np.round(np.arange(0, 0.51, 0.1),2)
~~~
