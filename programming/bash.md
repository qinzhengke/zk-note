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
