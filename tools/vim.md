## 给变量名加上引号

场景：想要得到枚举的名字，c++没有反射，只能傻傻的把枚举的名称重写一遍，然后加上双引号。

~~~{cpp}
enum Fruit{APPLE, ORANGE, PEACH, NUM_FRUIT};

string nameFruit[NUM_FRUIT] = {"APPLE", "ORANGE", "PEACH"};

~~~

这时候给变量名字加上引号，就排上了用场。

方法：`c i w " \ESC p`
`c i w` 是cut当前的word，并且insert。
`"`是插入双引号，一般的编辑器插入一个双引号的时候，会自动补全另一个，并且把光标移动到双两个双引号之间。
`\ESC p `，复制之前cut的内容。

## 缩进
`V jjjj < <`
`V`就是选择当前行，`jjjj`就是往下多选4行，连续两次`<`就是想左缩进一个tab，`>`就是向右缩进一个tab。

## 使用空格代替tab
在vimrc配置里面设置
~~~{bash}
set tapstop=4
set expandtab
~~~

## 拼接命令`J`
删除下一行开头的所有空格，然后将下一行剩下的部分拼接到当前行，这在写代码对齐的时候经常用到。
就是一个命令：`J`。

## 替换

~~~{shell}
# 替换当前行所有cat为dog
:s/cat/dog/g
# 替换当前文件所有cat为dog
:%s/cat/dog/g
# 替换当前选择的区域所有cat为dog，和替换当前行一样。
:s/cat/dog/g 
~~~

## 将小写词替换成大写
`v a w U`，即选择一个词，然后使用U来替换成大写。

## 替换中的正则表达式
vim中的`:s`替换实际上接的是正则表达式，所以如果要替换`[\*`等符号，就要注意使用转义字符`\`。
