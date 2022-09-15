# C++调试问题


## 企图通过memcpy对象进行拷贝操作。

类成员对象里面如果有容器类型，就会出现coredump，具体原因待研究。

## 模板定义中的typename和int不要搞混

模板定义中，有类型模板，也有大小模板，两者不要搞混了，在多重模板调用的时候，如果一个typename类型给了int类型，是无法编译通过的。

```cpp
template<typename T, int N>
class A{

}
```


## cannot bind non-const lvalue reference of type xxx to an rvalue of type xxx.

非常量引用一般是为了修改输入变量，但是如果输入进来的是临时变量（rvalue），那么函数结束后，临时变量会消失，这种改动就会毫无意义，编译器为了避免程序员犯错，会友好的报出编译错误。


##　初始化的重要性
一个未初始化的变量，可能导致算法的不正确，要debug很久。

debug方法论，一定要全体打印，不要先入为主地认定某些东西一定正确！


## 定义函数时命名空间使用

定义函数时，即使一开始使用了using namespace，在函数定义处仍然需要使用命名空间+"::"形式进行修饰。
实际上“using namespace”只是在使用类型时缺省命名空间，和定义是毫无关系的。

下面的代码会让系统无法区分到底要调用哪一个目标函数。

```cpp
#include <stdint.h>
#include <stdio.h>

namespace n1{
void func();
}

void func(){
    printf("This is func\n");
}

void n1::func(){
    printf("This is func in n1\n");
}

using namespace n1;

int main()
{
    func(); // 
    n1::func();
}

```

运行结果：

```bash
 In function 'int main()':
20:10: error: call of overloaded 'func()' is ambiguous
20:10: note: candidates are:
8:6: note: void func()
12:6: note: void n1::func()
```


## vector::push_back函数中会执行析构函数

记住push_back对象的时候会提前拷贝，拷贝的临时对象会被销毁，销毁时会调用析构函数，如果析构函数中有对static成员变量的操作，
那么就很容易导致结果不符合预期，例如下面的例子，a用来记录A对象的实例个数，我们预期是每push_back一个对象，那么count就会+1。
然而在push_back过程中，析构函数内a又被进行了-1操作，所以a仍然是0。

```cpp
struct A{
    static int a;
    A(){a++;}
    ~A(){a--;}
};
int A::a = 0;
int main(int argc, char* argv[]) {

    vector<A> al;
    A a;
    al.push_back(a);
    return 0;
}

```

结果输出

    deconst, a:0
    deconst, a:-1


## system()函数工作路径？
system（）函数即使调用了其他路径的可执行文件，可执行文件也相当于在当前路径下运行，举个例子，在~/目录下运行a.exe，a.exe里有一句话是

```cpp
system("f/b.exe");
```
b.exe在代码里写这一句生成"c.txt"文件。
那么这个c.txt到第一会在哪里生成呢？是在“~/”，还是“~/f/”？
答案是“~/”，也就是a.exe调用的地方，这里才是工作路径，所以程序里面写着在当前路径生成某个文件时，并不是指可执行文件所在的路径，而是工作路径，只不过没有嵌套调用可执行文件时，一般工作路径就是可执行文件的路径而已。


## 设置工作路径
紧接上一个问题，那么如何改变exe运行时候的工作路径呢？
Linux环境下
```cpp
#include <unistd.h>
...
chdir(working_dir)
...
```
其中，unistd表示unix环境下的标准接口，所以在windows下是用不了的。

Windows环境下
```cpp
#inlcude <direct.h>

...
_chdir(working_dir)
...
```


## typo_sizeof 初级错误：sizeof()的用法

如下代码所示，虽然问题很初级，但是有时头晕的时候还是会犯！

```cpp
bin_file.wirte((char*）imgCT.data, sizeof(imgIn.cols*imgIn.rows); //错误
bin_file.wirte((char*）imgCT.data, imgIn.cols*imgIn.rows*sizeof(char)；//正确
```


## 初级错误：移位操作符

C语言中的移位符号不代表赋值，只有用等号赋值之后，变量才会改变。

```cpp
a>>1; // 不赋值
a = a>>1; // 赋值
```


## 隐藏致命：函数内部malloc或者new

通常我们希望函数内部给传递的指针参数分配内存，但是这时候传递的指针一定要使用二级指针，否则传递的指针是无法得到新分配的内存地址的。


## 隐藏致命：直方图计算出现越界

用C语言写histogram函数时，一定要注意max-min后面要加1！例如以下代码

```cpp
*hist = (unsigned long) malloc(bins*sizeof(unsigned long));
memset(*hist, 0, bins*sizeof(unsigned long)
int r,c;
for(r=0;r<img_height;r+=)
{
  for(c=0; c<img_width; c++)
  {
    pixel = img_in[r*img_width+c];
    loc = (unsigned long)pixel * (unsigned long)bins / (unsigned long0（max_value-min_value+1）//这里一定要加1！
    （*hist）[loc] ++ ;
  }
}
```

这段代码运行时没有出exception，运行结果也是正确的，但是free时却出错了，当时一万个想不通，直到后来发现少了一个加1。
如果少了加1，举个例子，max=255，min=0，当pixel=255时，loc=bins，这时候就会访问越界！是的访问越界时候没有任何提示！
只有在free时才会显现出来！
其实max-min并不是整整的个数，真正的个数是max-min+1，例如1到256有256个数，时因为256-1+1=256。

遇到这种摸不着头脑的问题，逐步注释代码排除才是王道！往往认为不可能出现问题的地方才是问题隐藏的最深的地方。



## 非4整数倍尺寸图像存成bmp文件出错？
bmp标准强行要求图像宽度4字节对齐，所以一定要非4整数倍宽度一定要补齐。


## 多线程启动函数注意局部变量的使用
新线程里的某些静态数组数据，用着用着内容就被改的面目全非了，特别像踩内存。
一般新的线程都是由一个启动函数来启动的，启动函数将参数传入新线程，这时候要注意，一定不要将启动函数的局部变量传入新线程，因为启动函数是会结束的，局部变量是要被销毁的！
实际上新线程还没有启动的时候，启动函数就可能已经结束了。


## ifstream eof函数
用ifstream的eof来判断是否到达文件结尾是有坑的，即使到达了文件结尾，eof也不会马上为true，必须再读一次数据，eof才会返回true。
比较简洁的判断文件是否到结尾的方式是
```cpp
ifstream ifs;
while(ifs.peek())
{
}
```


## 引用不能重新改变对象。
无法根据=符号来重新改编引用对象，因为无法区分是重新引用还是对当前引用的赋值。

## 运行时出现链接错误？

```
c++ symbol lookup error undefined symbol
```

当动态链接库在动态加载时（插件系统常用），如果so文件没有定义相应的函数或者变量，就有可能报链接错误。

这和普通link时候的出错是一样的，调用者根据头文件拿到了声明，去找定义的时候找不到，只不过一个发生在运行时，另一个发生在link时。