c++{#cpp}
=====================

这个页面记录使用cpp（编程语言）遇到的问题。

## 链接的时候重复定义
明明只定义了1处变量，但是为何多个obj文件会重复定义？
可能1：变量定义在了h文件中，并且没有使用static修饰，


## system()函数工作路径？
system（）函数即使调用了其他路径的可执行文件，可执行文件也相当于在当前路径下运行，举个例子，在~/目录下运行a.exe，a.exe里有一句话是
```{.cpp}
system("f/b.exe");
```
b.exe在代码里写这一句生成"c.txt"文件。
那么这个c.txt到第一会在哪里生成呢？是在“~/”，还是“~/f/”？
答案是“~/”，也就是a.exe调用的地方，这里才是工作路径，所以程序里面写着在当前路径生成某个文件时，并不是指可执行文件所在的路径，而是工作路径，只不过没有嵌套调用可执行文件时，一般工作路径就是可执行文件的路径而已。

## 设置工作路径
紧接上一个问题，那么如何改变exe运行时候的工作路径呢？
Linux环境下
~~~{.cpp}
#include <unistd.h>
...
chdir(working_dir)
...
~~~
其中，unistd表示unix环境下的标准接口，所以在windows下是用不了的。

Windows环境下
~~~{.cpp}
#inlcude <direct.h>

...
_chdir(working_dir)
...
~~~

## TCHAR问题？
tinydir使用TCHAR作为基本的字符串输入，在Linux下，TCHAR默认是char，但是在Windows下，THCAR默认是wchar_t。
如果直接使用char\*作为tinydir的输入，就会出错，其实TCHAR由一个宏来控制，就是UNICODE，VS环境和qtcreator环境都能关闭这个宏，关闭之后TCHAR就变成了char，
VS的方法是项目属性页面设置字符集为多字节，而不是Unicode。
qtcreator的方法是在pro文件中设置 DEFINES -= UNICODE。


或者将wstring转换成string
```cpp
// c++11
#include <locale> 
#include <codecvt>
...
wstring string_to_convert;

//setup converter
using convert_type = std::codecvt_utf8<wchar_t>;
std::wstring_convert<convert_type, wchar_t> converter;

//use converter (.to_bytes: wstr->str, .from_bytes: str->wstr)
std::string converted_str = converter.to_bytes( string_to_convert );
```

对应的tinydir_open，要使用wchar_t作为输入，如果输入常量字符串，可以这样写L"abcde"。
codecvt头文件在gcc5才引入，如果使用gcc4，是无法通过编译的。

## 系统limit文件报错？
有的时候系统源文件报错会让人摸不着头脑，开发者根本不知道哪里出错。这里列举一个遇到的问题。
1. 有可能是定义了和已有库重名函数或者变量，例如有一个案例，不知道谁多此一举，定义了min和max函数，虽然自定义的max函数有ifndef防止重复定义，
但是已有库的函数不一定会这样定义啊，所以如果先编译了自定义的max，然后在编译已有库max之间如果有其他系统函数调用了这个max，就会直接报错了。
所以不要随便定义和已有库重名的东西。

## 添加标准头文件就会报错？
遇到一个很奇怪的问题，#include一个标准库头文件后，系统的其他模块就会报错，具体来说，我在main.cpp里include<local>，然后编译时，limit文件就会报max函数使用参数给错。
虽然问题很怪，但是还是找出来了，实际上，这是一个**触发式**问题，加入的代码本身没有问题，例如添加一句include标准库，但是添加进去之后确实出了问题。
实际上，include <local>触发了工程代码里面的一个隐藏问题，也就是上一篇所说的自身的工程定义了一个max宏，而且形参形式和标准库的max不一样，但是由于原来的工程真的再没有其他标准库里的函数调用max和min，所以就一直没有报错！但是添加<local>之后，local使用了max宏，但是本来是期望按照标准库的用法使用的，但是由于先编译了本地的max，所以local里的函数使用max时就报错。
 所以以后如果遇到了这种莫名其妙，include标准头文件都能报错的问题，就有可能是它触发了系统里原有的问题！
 
 ## namespace语法错误？
 namespace语法很简单，但是为什么会出错呢？
 有一种可能，就是在c文件或者c文件引用的头文件里定义或者使用的namespace，C语言没有明明空间，当然不支持。
 只不过编译器不会提示C文件不能定义namespace，只是会提示语法错误。
 
 ## memcpy报出 will always overflow destination buffer.
 gcc编译memcpy的时候，如果目标地址是一个固定大小的静态的数组，那么编译器会检查copy的size是否会超过这块静态数组的大小，超出了就会报出错误，不得不说编译器做得非常不错。
 
 ## 枚举变量++操作
 c++中，枚举变量进行++操作会出现编译错误！
 然而C语言中，枚举变量++是允许的！
 C++已经不兼容C了？
 
 ## passing 'const Type' as 'this' argument discard qulifiers
 如果const对象调用了非const成员函数，就会报这个错误，
 const对象只能调用const方法，const修饰的成员函数会保证不会修改成员变量，所以const对象才能因此保证不被修改。
