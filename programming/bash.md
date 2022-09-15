# Bash：Linux上的脚本语言

## 非阻塞运行程序

```bash
start c:\my.exe
```

## 运行路径带空格的程序

加引号，参数可以放在引号的后面

```bash
"c:\program files\my.exe" <arguments>
```

##循环

```bash
FOR /L %%a IN (1,1,15) DO (
my.exe
)
```

## if表达式
注意：1.中括号和括号里的内容一定要用空格隔开
     2.只有if和else if后面需要then，else后面不需要then。

```bash
if [$# -gt 0 ] 
then

  if[$1 == 'target1' ]
  then
    do_somthing.
  else if [$1 == 'target2' ]
  then
    do_other_things
  
  fi

else
fi
```


## 如何把运行结果赋值给变量？

```{.bash}
var = $(ls | grep aaa)
```


## bash_var $()和${}的区别

$()里面必需接命令，不能接受变量

${}里面必需接受变量，不能接受命令

两者不能混用