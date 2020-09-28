脚本语言：bash{#bash}
===========

### 非阻塞运行程序

```bash
start c:\my.exe
```

### 运行路径带空格的程序

加引号，参数可以放在引号的后面

```bash
"c:\program files\my.exe" <arguments>
```

###循环

```bash
FOR /L %%a IN (1,1,15) DO (
my.exe
)
```

### if表达式
注意：1.中括号和括号里的内容一定要用空格隔开
     2.只有if和else if后面需要then，else后面不需要then。

~~~{bash}
if [ $# -gt 0 ] 
then

  if[ $1 == 'target1' ]
  then
    do_somthing.
  else if [ $1 == 'target2' ]
  then
    do_other_things
  
  fi

else
fi
~~~

<hr>
\section 如何把运行结果赋值给变量？

\code{.bash}
var = $(ls | grep aaa)
\endcode