Python{#programming-python}
===========================

此页面记录一些有用的标准python语法。


## for循环中同时使用序号和值
一般情况下，for循环支持使用序号（像c++一样）或者使用值来遍历数组，但是如果需要同时用值和序号的时候有没有优雅的表达呢？答案是有的，使用enumerate语句。

~~~{.py}
x = [1, 4, 9 16]
for i,x in enumerate(x):
	print(i, x)
~~~