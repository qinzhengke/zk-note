编程语言：C++{#cpp}
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
 
 ## \_Bool类型
 \_Bool类型是C99引入的专属于C语言的布尔类型，如果要使用，必须包含stdbool.h头文件。

## C语言和C++混用的可能问题。
1.c模块依赖包含c++特性的c++模块
这将导致编译不过，产生的问题类似namespace非法，因为C语言根本没有namespace。
2.extern C 封装起来的代码包含C++特性
这将导致编译不过，产生的报错例如“template with C linkage”。

# 1.标准C++

这一部分主要记录使用标准C++时遇到的问题。



## 编译错误: Attempting to refer a deleted function 

这个问题常常出现在使用std::fstream时，如果将ifstream或者ofstream对象作为参数传入函数，那么传入时，一定要使用引用方式，否则就会报出这个错误。这个报错没那么直接，编译器不会是说没有用传入，而是在函数调用的时候说调用的函数是被删除过的。

## 初级错误：sizeof()的用法

如下代码所示，虽然问题很初级，但是有时头晕的时候还是会犯！

```c
bin_file.wirte((char*）imgCT.data, sizeof(imgIn.cols*imgIn.rows); //错误
bin_file.wirte((char*）imgCT.data, imgIn.cols*imgIn.rows*sizeof(char)；//正确
```



## 初级错误：移位操作符

C语言中的移位符号不代表赋值，只有用等号赋值之后，变量才会改变。

```c
a>>1; // 不赋值
a = a>>1; // 赋值
```



## 隐藏致命：函数内部malloc或者new

通常我们希望函数内部给传递的指针参数分配内存，但是这时候传递的指针一定要使用二级指针，否则传递的指针是无法得到新分配的内存地址的。



## 隐藏致命：直方图计算出现越界

用C语言写histogram函数时，一定要注意max-min后面要加1！例如以下代码

```c
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



## 专题：C++中的二进制文件读取

这一小节研究一下c++对二进制文件读写的用法。

首先定义两个结构体A和B，如下代码所示，其中“#pragma”语句是保证数据存储时1字节对齐，编译器一般默认4字节对齐，例如一个结构体按照变量计算得61个字节，那么编译器会在存储这个结构体时补上3个字节，保证4字节对齐。字节对齐虽然能够优化存储空间，但是在读写文件的时候会造成麻烦，特别是读取端代码不知道结构体形式，而只是知道变量列表的时候，读文件顺序会出错。

```c++
#include <iostream>
#include <fstream>
using namespace std;

#pragma pack(1)
typedef struct _A
{
    int index,time_stamp;
} A;

typedef struct _B
{
   float data1,data2,data3;
}B;
#pragma pack()

```

然后是写文件部分，其实写文件很简单，一般不考虑写的位置和文件尾什么的，下面代码展示了存储两个结构体，分别是A和B，构成一个数据块。通过调整N可以增加数据块的个数，但是目前我们只用一个数据块

```c++
int write_file_01()
{
    const int N = 1;
    A a[N];
    B b[N];

    for(int i=0; i<N; i++)
    {
        a[i].index = i;
        a[i].time_stamp = i*2;
        b[i].data1 = i;
        b[i].data1 = i*i;
        b[i].data3 = i*i*i;
    }

    ofstream out_file;
    out_file.open("file_01",ios::binary);
    for(int i=0; i<N; i++)
    {
        out_file.write((char*)(&(a[i])), sizeof(A));
        out_file.write((char*)(&(b[i])), sizeof(B));
    }
    out_file.close();

    return 1;
}
```

读取文件就比较麻烦了，特别是使用不完全读取时，特别是在使用seekg函数时。我们看一下代码，我们一般使用eof()函数来检测是否读取到文件末尾，但是，我们有时候不想读取所有内容，而只是其中一部分。例如数据由Header部分和Data部分组成串联在一起，Header描述了Data部分的内部结构，即”HDHDHDHD......“的形式。我每次只想读取H部分，因为D部分太长了，我只能使用seekg()函数略过。

一开始很自然得想到如下的代码，用read()函数读取H部分，然后用seekg()函数略过D部分，知道走到文件的末尾。然而，这段代码运行的时候其实是无限循环，如图所示！

```c++
int read_file_01() // 错误的使用方式！
{
    A aa;
    B bb;
    ifstream in_file;
    in_file.open("file_01", ios::binary);
    while(!in_file.eof())
    {
        in_file.read((char*)(&aa), sizeof(A));
        cout<<"aa.index="<<aa.index<<" aa.time_stamp="<<aa.time_stamp<<endl;
        cout<<"eofbit = "<<in_file.eof() << ", failbit = "<<in_file.fail()<<", badbit = "<<in_file.bad()<< endl;
        in_file.seekg(sizeof(B),ios::cur);
        cout<<"eofbit = "<<in_file.eof() << ", failbit = "<<in_file.fail()<<", badbit = "<<in_file.bad()<< endl;
    }
    in_file.close();
    return 1;
}
```

![](files/read_binary_file_error_01.png)

我们可以看到，在seekg()函数将文件指针走到文件末尾时，再用read()函数，这时候eof就会置位，但是此时再seekg()之后，eof又会变成0了！这感觉不太合常理，只用eof不太可靠，事实上，cplusplus网站给出了比较合理的用法：

> Reaching the *End-of-File* sets the eofbit. But note that operations that reach the *End-of-File* may also set the failbit if this makes them fail (thus setting both eofbit and failbit).

我们可以这样修改读取函数：

```c++
int read_file_01_correct()//不太完美的读取方法
{
    A aa;
    B bb;
    ifstream in_file;
    in_file.open("file_01", ios::binary);
    while(!in_file.eof() && !in_file.fail())
    {
        in_file.read((char*)(&aa), sizeof(A));
        cout<<"aa.index="<<aa.index<<" aa.time_stamp="<<aa.time_stamp<<endl;
        cout<<"eofbit = "<<in_file.eof() << ", failbit = "<<in_file.fail()<<", badbit = "<<in_file.bad()<< endl;
        in_file.seekg(sizeof(B),ios::cur);
        cout<<"eofbit = "<<in_file.eof() << ", failbit = "<<in_file.fail()<<", badbit = "<<in_file.bad()<< endl;
    }
    in_file.close();
    return 1;
}
```



其运行结果如下，程序不再是无限循环了，但是这也并不完美，因为事实上我们只期待一次循环，而这里多出了一次无用的循环，问题在于seekg()函数与read()函数不同，当执行seekg()函数走过文件的末尾时，它并不会将eofbit置1！所以只有等下次循环read()失败时，才能得到结束循环的条件。多了一次无用循环即不好看也有隐患，谁知道在那个无用循环中程序会做什么出格的事情呢？seekg()函数第一次超出文件末尾不将eof置位，第二次超出文件末尾时反而将eof从1置0，感觉seekg函数该做的不做，不该做的却反倒做了。

![](files/read_binary_file_error_02.png)

我目前在使用的最稳定可靠的方案是构建一个check函数，这个函数里面执行read命令，查看文件指针是否越界，然后再将因为执行read函数而位移的指针拨会即可，如下代码所示。这种方式需要新建一个函数，而且不是最漂亮的，但是这是我目前认为最可靠的一个方案，毕竟软件首要的指标是可靠！

```c++
inline bool check_eof(ifstream & file)
{
    char bit;
    bool s = false;
    file.read(&bit, sizeof(char));
    if(file.eof() || file.fail())
        s = true;
    file.seekg(-1, ios::cur);
    return s;
}

int read_file_01_perfect() // 完美的读取方案
{
    A aa;
    ifstream in_file;
    in_file.open("file_01", ios::binary);
    while(!check_eof(in_file))
    {
        in_file.read((char*)(&aa), sizeof(A));
        cout<<"aa.index="<<aa.index<<" aa.time_stamp="<<aa.time_stamp<<endl;
        cout<<"e, f, b = "<<in_file.eof()<<", "<<in_file.fail()<<", "<<in_file.bad()<<endl;
        in_file.seekg(sizeof(B), ios::cur);
        cout<<"e, f, b = "<<in_file.eof()<<", "<<in_file.fail()<<", "<<in_file.bad()<<endl;
    }
    in_file.close();
    return 1;
}
```

运行结果如下图所示，只有一个循环，很完美。

![](files/read_binary_file_error_03.png)

main函数里面的代码由于实验需要进行部分屏蔽。

```c++
int main(int argc, char *argv[])
{
    write_file_01();
//    read_file_01_correct();
    read_file_01_perfect();
    //write_file_02();
    //read_file_02();
    return 0;
}
```

### 代码不规范之一：全局变量，增加代码流程理解的难度，DEBUG的难度。

一般某个变量在某处出错的时候，调试者都会想追溯变量从目前到出生的时刻，单线、顺序地查到看到底错误。
但是如果是使用全局变量，会发现这个变量的经历就像一团乱麻，你根本不知道它到底**在哪里被修改**的，你得漫山遍野地去搜索每一处它被修改的地方，如果说通过IDE可以全局查找还能勉强解决这个问题的话，另外一个很致命的点就是是，你不知道每一处的**先后顺序**，甚至有些地方虽然出现了该全局变量，但是实际上根本没有执行。这个问题工具也没有办法搞定，调试者只能硬着头皮调查每一处在实际运行的顺序。如果一个程序本身规模不大还好，但是一个模块上了万行，就会发现调试这东西是相当痛苦的，尤其是在调试别人代码的时候！

举个例子，调试者在DEBUG，如下面代码所述，my_main函数最后一行，例x被赋值给了y，在这一行看到了x异常，然后往前追溯，乍一看发现卧槽这个变量居然没有初始化，再一看，发现哦，这货是全局变量，但是这个变量在main函数里没有显式的出现（在子函数里出现称为隐式），然后只能搜索这货到底在哪里出现过，一搜，53处，卧槽！这个变量在函数a，b，c都出现过，而且在my_main函数之前也出现过，调试者得review每一个地方的代码，理一遍逻辑才能知道有没有出错。

```cpp
int a;
a(){if(0) x =3; }
b(){x=10;}
c(){if(z>Z) x=4; }
void my_main()
{
  a(); // 一些函数
  b();
  c();
  int y = x;
}
```

有的人说我看着代码也不是很复杂啊，随便一看下就知道了。是的，代码简单是没问题，但是如果
- 前面的abc三个函数增加到**几千行**，
- 而且**互相调用**，**调用的深度**有5、6层，
- 函数处于**不同的源文件**，
- 每一处变量的修改充斥着非常多的**数学运算**，
- 伴随着许多**类似的变量名**，x0，x1，xx，my_x，这些变量和x相互作用，相互转化。。。
- 这个变量不是简单的什么float,int类型或者结构体，而是一张图像内存，这个图像内存buffer你得加一些代码show出来或者save才能看到。
- 和它一样生猛的全局变量在这个模块还有十几甚至几十个。。。

这时候就会理解此时的痛苦了。

### 代码不规范之二：变量名频繁换马甲
变量名每次进入一个子函数就会换一个名字，有时候调试的时候真的不好把握，总得调到definition和callee那里查看两个变量是不是一样的。
特别是当版本变更时，例如本来函数f处于第二层，由于版本变更，第二层函数被干掉了，只好带着f直接来到一层，然后发现曾经的二层函数给f的实参在一层里名字完全变了，这时候又要回退版本，找曾经的二层函数给f的参数在一层函数到底是哪些。

名字的变更例如，从index到image_index再到frame_index，从direction到image_direction再到direction。。。
变量名在进入多级函数时，最好**保持不变**，这样在修改代码的时候不用**频繁去check**形参和实参是否正确。
变量可见区域的划分目的就是让程序员能在不同的函数用同一个变量名。



## 隐藏致命：函数明明存在却报unresolved symbol！

在VS下，函数明明存在为什么还是报出unresolved symbol错误?可能是字符集问题！特别是将不同操作系统下的文件进行混用的时候！通通，通通，通通改成多字节字符。

这个问题怎么google？一查unresolved symbol全都是确实没有添加函数定义的。
回答：搜索关键词：function do exist unresolved symbol

在vs下如果是main.cpp、a.h和a.c这种组合一定会出问题，可以试一试，注意，这里a.c是C语言写的，用于板子上的程序，不能有c++的特性，但是main.cpp可以，因为会用到第三方库进行显示，例如用Opencv读取、显示和保存图片。
解决方法，在project property的C++/Advanced中，Complie as一项从default改成as c++。
在QtCreator下也是一样的，需要设置编译方式为C++

### debug和release的不要混用！
debug时间，2017-09-25 20:00-22：00

一个de了快**两个小时**的bug！
一段将二进制单通道图像转换成png图像的代码，文件前8个字节是W和H，接着是W\*H长度的图像数据。
在imwrite的时候一直出错，W和H读出来的是正确的，搞了半天一直没有头绪，非常的苦恼。
其中还怀疑过是不是QtCreator的问题，然后用VisualStudio试了一遍，发现VisualStudio居然可以！（VS一开始也有问题，但随即发现是自己文件路径不对）
然后仔细对比了一下，发现在VS下链接的OpenCV库文件是opencv_world310d.lib，并且运行的方式是DEBUG。
然而在Creator中我链接的库是opencv_world310.lib，并且运行的方式是DEBUG。
加上后缀"d"之后，终于可以运行了！或者将运行方式改成release，同样也可以运行！
这个bug在之前的imread函数是没有问题的，这进一步搞混淆了！

```cpp
void convert_bin_to_png(string in_path, string out_path)
{
  ifstream f;
  f.open(in_path, ios::binary);
  uint32_t W,H;
  f.read((char*)&W, sizeof(uint32_t));
  f.read((char*)&H, sizeof(uint32_t));
  char *img = new char[W*H];
  f.read(img, W*H);
  Mat save = Mat(H, W, CV8UC1);
  save.data = (uint8_t)img;
  imwrite(out_path, save); // 问题就出现在这一行
  delete img;
}
```

**教训：** lib库的引用一定要**遵循规范**！debug版本的代码要引用debug版本的lib，release版本的代码要引用release版本的库。
举一反三，x64的编译方式用x64的库，x86的编译方式用x86的库，vc08，vc10，vc12的库最好也一致！

### 使用Creator创建的控制台程序无任何输出？
debug时间，2017-09-25 22:20-22:30

如题，程序本身是能够正确运行的，例如我输出的文件是正确的。
但是在终端运行程序时，程序本身没有任何输出，不论是正确的信息还是错误的信息。
最后发现是使用Creator构建程序时，没有在pro文件中加入CONFIG+=console导致的，加入之后程序就能正常输出了。
加入之后记得qmake+rebuild一下，仅仅重新编译有时候不会更新的。

**教训：**使用qtcreator构建工程时一定要清楚常用的几个参数的含义以及应用，pro文件并不是啥都不用写就能正确配置的。

### 处理三通道图像出错？
2017-09-26, debug时间，10分钟。

下面一段代码将float图像转换成color_map形式，在处理每个通道的数据时，地址没有写对，W和c忘记乘上通道数了。
```cpp
int cvt2color(float *img, uint32_t W, uint32_t H, uint8_t **img_out)
{
    const uint32_t N = 511;
    uint8_t color_map[N][3];
    for(uint32_t i=0; i<N; i++)
    {
        color_map[i][0] = i<=255 ? 255-i : 0;
        color_map[i][1] = i<=255 ? i : 510-i;
        color_map[i][2] = i<=255 ? 0 : i-255;
    }

    *img_out = new uint8_t[3*W*H];
    for(uint32_t r=0; r<H; r++)
        for(uint32_t c=0; c<W; c++)
        {
            int32_t loc = (int32_t)img[r*W+c] + 255;
            for(int k=0; k<3; k++)
            {
                if(loc < 0)
                    loc = 0;
                else if(loc > 510)
                    loc = 510;
                // (*img_out)[r*W+c+k] = color_map[(uint32_t)loc][k]; // 错误
                (*img_out)[r*3*W+3*c+k] = color_map[(uint32_t)loc][k]; // 正确

            }
        }

    return 0;
}
```

**教训：**处理多通道图像一定要注意寻址和通道的关系。

### 定点化的程序反而更慢？
2017-10-11 debug时间：1天

使用定点小数进行优化，在PC上跑是有1/5的时间减少的，但是移植到ARM上，发现反而运行的时间还更长，差不多是1.5倍左右，很纳闷。

原因：最后查到了程序中有一个限制运算数量的宏常数写错了。。。。本来要限制在XXX_YYY=1500的，结果却成了XXX_APP_YYY=5000。
两个宏搞混了，原因就是在pc版只有XXX_YYY这个宏，而在ARM版本上却将XXX_YYY偷偷改成了XXX_APP_YYY。

**教训：**不能太相信develop中的程序，任何地方都有可能变更，要有强大的内心的面对这一切，也要有仔细观察的耐心。

### 数据读取出错？
2017-10-26 debug时长：2小时
debug发现数据存储结构不符合预期，但是没找到为什么不符合预期，第二天重新测了一组数据，又变好了！

教训：嵌入式程序数据记录可能会有偶然问题，发现问题时一定要多测几组（至少两组）来先确认问题！

### fabs出错？
2017-11-03 debug时长：10分钟
描述：fabs函数结果不对？这个问题很明显，printf出来就有问题。
原因：没有加入math.h头文件，这个愿意很坑爹，因为没有头文件，编译居然不报错。
一个只依赖于标准库的程序没遇到过这种问题，这个问题出现在引用别人的库的前提下，有的时候别人的库定义了这个函数，但是并不是正确的，或者是编译选项选择忽略这种问题，例如我在工作中，就发现一些找不到定义的函数只有警告而不是错误。

教训：这种明显很奇怪的问题，一般是和环境相关的，头文件什么的不要省，而且要怀疑提出编译的人。

### 加了新功能，然后其他“不相关”模块崩溃？
2017-12-06 debug时长， 30分钟
原因，通过指针形式获取别的线程结构体内容，然后处理过程中别的线程改动了那块内存上的数据，有可能那块内存已经被释放。
报错那是比较轻的结果，最可怕的是，刚好没有报错，那块内存是别人在用的，你偷偷踩了别人的内存，而别人的模块除了错还完全不知道什么回事。
俗称“踩内存”，如果不报错，这种问题很难发现，目前唯一比较有效的debug方式就是git回滚+review代码。

如果不确定通过指针获取的结构体会不会变，那么一定要自己定义一个结构体变量，然后传入地址，再memcpy一下，不要贪图简单，直接定义个指针传入。

### ifstream读取数据总是在特定长度出错？
2018-01-02 debug时长，1个小时
描述：读取一个binary文件，文件的长度为143360字节，然而只能读取106个字节，之后总是fail，摸不着头脑
原因：读取binary文件的时候没有加入ios::bianry参数。
没有加入ios::binary参数，文件就会通过文本形式读取，读取时0x1A被认为是end of file，所以读取总在特定长度失效。


### 文件总是读写有问题？
2018-01-08 debug时长，2个小时
读出的文件不对？看看二进制参数是不是正确的，百分之八十的问题都来自这里。
C语言看"wb"和"rb"，c++看ios::binary


### 根本无错的地方crash？
2018-12-07 debug时长，1个小时
一开始在A地方crash，屏蔽了代码A之后又在B处崩溃，同样屏蔽代码B后又在C处崩溃，这个ABC处怎么看都不会崩溃，根本没有指针寻址操作。
后来一段一段代码屏蔽，才发现问题出现在ABC之前的X处，X处代码出现指针越界，但是本身运行过程中居然没问题，等到后面运行其他函数时，就爆发了，这种bug非常可怕，很容易误导人。

### 根本无错的地方crash（2）？
2019-01-28 debug时长，1小时
迷迷糊糊加了一些代码之后，然后本来无错的地方突然crash了？crash的地方和改动地方差别相差很远，而且完全没有动过，怎么会crash呢？
有一种可能一定要警醒，就是前置代码内存越界了！

### 非4整数倍尺寸图像存成bmp文件出错？
bmp标准强行要求图像宽度4字节对齐，所以一定要非4整数倍宽度一定要补齐。


### 头文件不要定义变量！
头文件不要定义变量，否则当多个c/cpp文件引用同一个头文件的时候，会出现重复定义问题！
头文件如果一定要定义变量，就在函数里定义，然后通过返回的形式使用。

### 在头文件中定义函数一定要使用inline！
header only写法一定要注意头文件中定义函数的实现的时候，一定要加入inline修饰符，或者使用类的静态成员来定义实现。

### 多线程启动函数注意局部变量的使用
新线程里的某些静态数组数据，用着用着内容就被改的面目全非了，特别像踩内存。
一般新的线程都是由一个启动函数来启动的，启动函数将参数传入新线程，这时候要注意，一定不要将启动函数的局部变量传入新线程，因为启动函数是会结束的，局部变量是要被销毁的！
实际上新线程还没有启动的时候，启动函数就可能已经结束了。

### 如何用驼峰命名法命名大写缩写？
大写缩写会使得我们使用驼峰命名法是感到困扰，如果全部大写，就会失去区分单词的能力，如果后面小写，这失去了缩写的表达能力。
微软的规范就是：如果缩写词是2个字母，那么全部大写，如果缩写词大于等于3个，那么当做普通单词，即头字母大写，后面小写。
例如IOExport，ImuHeader，这样的话如果看到了3个大写字母，说明第三个字母是下一个单词了首字母。

### 临时变量无法引用
c和c++中存在所谓的临时变量，想到的就三种：1.运算表达式，2.类型转换，3.函数返回值。
这三种临时变量传入以非常量引用为形参的函数时，是非法的，例如：
~~~{cpp}
void func_add(int a, int b, int& c)
{
    c = a+b;
}
int main()
{
    int x=1,y=2;
    float z;
    fun_add(x,y,(int)z); //编译报错
}
~~~
临时变量无法作为非常量引用的原因是很显然的，作为非常量引用，用户肯定是希望改动这个输入参数，然而实际上真正发生改动的是临时变量。
如果编译不报错，那么可能会给使用者造成很大的误解，使用者很可能要debug很久才能发现这个问题。于是编译器就负责的直接给出编译错误。

实际使用的一个案例就是，使用指针的引用作为形参，通常我们想通过一个函数修改指针的指向，可以使用二级指针，也可以使用指针的引用，既然c++提倡使用引用作为输出，我这里也使用指针的引用作为输出。

使用指针的引用有一个问题就是，通常我们传入指针的时候需要做类型转换，而类型转换就是上文说道的第二种临时变量，编译器会报错。正确的使用方法就是调用时提前把指针手动转换好，再传入函数。

### ifstream eof函数
用ifstream的eof来判断是否到达文件结尾是有坑的，即使到达了文件结尾，eof也不会马上为true，必须再读一次数据，eof才会返回true。
比较简洁的判断文件是否到结尾的方式是
~~~{cpp}
ifstream ifs;
while(ifs.peek())
{
}
~~~
