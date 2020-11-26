C语言编译常见问题{#c_compile_faq}
==============================

<hr>
\section 编译篇

<hr>
\subsection gcc不识别_Bool以及bool类型
_Bool以及其封装bool，是C99才引入的，如果编译器不识别，有可能是C99选项没有打开，打开C99的方法根据构建系统而不同，需要搜索。

【参考】\ref c99_bool

<hr>
\subsection 将指针赋值给uint32_t
很多32位平台的程序喜欢使将指针赋值给整数，这样就不需要二级指针，例如

\code{cpp}
int func(uint32_t *addr)
{
  void * data = malloc (1000);
  *addr = (uint32_t) data;
}
\endcode
如果需要在函数里面开内存，输出内存地址，那么要么使用二级指针，要么用这种转地址的方式。
但是，这种方式其实并不好，在64位系统中，指针的大小可能是64位的，这样赋值就会出错。
仍然像想用这种方式的话，使用intptr_t类型，兼容32和64位，在C99中支持，在<stdint.h>中定义。

<hr>
\subsection printf_red 如何打印红色字符串？
\code{cpp}
#include<stdarg.h>
...
void printf_red(const char *cmd, ...)  
{  
    printf("\x1B[31m");  //设置输出为红色
    va_list args;       //定义一个va_list类型的变量，用来储存单个参数  
    va_start(args,cmd); //使args指向可变参数的第一个参数  
    vprintf(cmd,args);  //必须用vprintf等带V的  
    va_end(args);       //结束可变参数的获取  
    printf("\x1B[0m")   // 回到默认设置
}  
\endcode

<hr>
\subsection printf 中的uint64整数
使用printf过程中，一定要写对%字符串，之前遇到过一个问题，使用%d来打印一个int64的数是错误的，例如下列代码

\code{cpp}
uint64_t a = 1000;
printf("%d\n",a); // 错误
\endcode
实际上，%u，%ld，%lu都不对，在曾经一个ARM平台编译器上，正确的结果是%llu，因为在该平台上，sizeof(long)=sizeof(int)=4，而uint64_t真正的类型是long long，在不同平台时，要注意类型字节数可能不一样。

<hr>
\subsection linux-arm-gcc 中的char默认是unsigned char!!!
好可怕，给char类型变量赋值一个负数，会得到错误的结果，而且编译器不会报错或者警告
只有使用==判别时才提出警告。

\code{cpp}
char a = -1;
printf("%d\n",a );  //打印出 255
if(a == -10)
  printf("error") // 编译报错
\endcode

<hr>
\subsection extern 声明
默认的全局变量都开放成全局可用变量，其他模块想用该变量只需要extern声明即可。
例如a.c文件中定义int a = 0; b.c文件中声明extern int a;
那么b文件中使用的变量其实就是a文件中的变量
**a.c文件和b.c文件不需要什么相互依赖的关系，也和头文件无关**。
但是一般来说，在a.h中声明extern int a;然后b.c文件再include a.h文件，这样的用法会比较规范。


<hr>
\subsection multiple definition?
1. 函数重复定义了，如果非要使用同名函数，加入static修饰符，只供本地函数使用。
2. 头文件没有加入#ifdef #define #endif保证内容只定义一次。
3. 前两个条件都没问题，仍然报这问题？查看一下有没有在头文件中定义变量？如果定义了变量，那么该头文件被多次include的时候，变量会重复的定义，解决的办法有两种，第一种，直接使用static修饰变量，那么h文件中的变量，就会在include的时候变成c文件的static变量，自然不会有冲突，第二种就是定义函数替代变量，函数里再定义这个全局变量，按理说全局变量本就应该使用函数访问。

<hr>
\subsection unresolved external symbol？
如果这个symbol在源文件里：
- 看看这个源文件有没有被加到工程中。
- symbol在源文件中，并且已经添加到了工程，还会报这个错？看看是不是调用了.c文件，记得用EXTERN C封装，即在c文件函数声明的地方使用
\code{cpp}
#ifdef __cplusplus__
extern "C"{
#endif
int my_c_function();
#ifdef __cplusplus__
}
#endif
\endcode
- 都是cpp文件或者c文件，或者已经用EXTERN C封装了，还会出错？看看是不是不同源文件字符编码的问题。
- 以上条件都满足，但在qtcreator中刚添加的函数还是报这个错？那就试着显式地qmake一下，然后再rebuild。

如果这个symbol在库文件中：
- 看看这个库有没有添加到工程。
- 库已经添加到工程，但是仍然报这个错？那就将库的版本和当前编译器版本保持一致，包括平台（x86或者x64）和版本（vc08，vc10，vc12，vc14）以及是否调试（debug和release）。

<hr>
\subsection 工程内外的头文件区别？
C语言头文件不管是放在工程内部还是工程外部都可以include，那么这两种方式有什么区别呢？
目前发现的区别有一点，就是工程内部的头文件内部再include别的头文件的时候可以享用工程文件已经添加的路径。
举个例子，a.h是工程部内的，b.h是工程外部的，main.cpp里面把这两个头文件都include了，工程pro（qtcreator）文件里包含了opencv的库目录。
那么打开a.h时输入“include <opencv.....”，此时creator会自动补全，但是在b.h中输入则没有任何反应，这就是目前发现的一个区别。
但是我估计实际编译的时候应该没有问题，这只是IDE的识别问题而已。

<hr>
\subsection core.hpp header must be compiled as c++
在c文件中include Opencv的hpp头文件就会出现这个问题，因为编译c文件时是使用C编译器编译的，而.c文件是不认识.hpp文件的，实现的方式就是c编译器并未定义__cplusplus宏。按照道理说c文件不应该包含hpp文件，最好把c文件改成cpp文件。如果硬是不改，在VisualStudio中可以设置compile as c++。

<hr>
\subsection 头文件重复包含
我们都知道头文件一开始两行都是#ifndef和#define，是为了避免头文件重复包含，从逻辑上和减少编译时间看，都需要消除这种重复包含，
那么消除重复包含的工作为什么不丢给程序员做呢？
1. 这样的工作非常枯燥而繁琐，包含的链路太长的话，光是找到公共头文件节点就很麻烦；
2. 更重要的是，部分模块常常需要独立运行，这种冗余保证了模块也能够独立地运行。

<hr>
\subsection utf-8文件的bom字节
很蛋疼，读utf-8文件前面3个字节都不是文件中的内容，而是0xef,0xbb,0xbf，后来一查，发现这是utf-8文件的bom字节，就是byte order mark,
读utf-8文件时，一定要去掉前面三个字节。

<hr>
\subsection utf-8文件中的行结尾'\r'
很蛋疼+1，读utf-8文件的时候，行结尾都是以'\r'结束，
当文件内容是另一个文件的路径的时候，例如
\endcode
/mnt/cc/随便
\endcode
其实这一行读出来是这样的
\endcode
'/','m',','t','/','c','c','/','随,'便',\r
\endcode
我一般喜欢在文件路径后面加上'/'，方便后面读取。
但是如果在\r后面加上'/'就会搞错。。

<hr>
\subsection qtcreator中开启gcc的c99支持
\endcode
QMAKE_CFLAGS += -std=c99
\endcode

<hr>
\section 库函数篇

\subsection FLT_EPSILON宏

问题：做除法的时候，我们知道分母不能为0，那么如何检查呢？ “if(a!=0.0f)”这种小学生的错误就不要犯了。

解决方法：一般来说我们需要拿到一个很小的数，如果分母的绝对值小于这个数，就认为分母为零。
最适合的方法是使用<float.h>提供的FLT_EPSILON。

\code{c}
#include <float.h>
float a = 0;
if(fabs(a)<FLT_EPSILON){
  // return error.
}
\endcode


<hr>
\section 性能篇

<hr>
\subsection 一个float不够用的例子
x = {3137995, 3137895, 3137978}
A = {{x0\*x0, x0, 1},{x1\*x1, x1, 1},{x2\*x2, x2, 1}}

<hr>
\subsection 下采样拷贝时间
下采样拷贝虽然点数少，但是需要多次拷贝操作，那么这种方式和全拷贝速度差异如何呢？

\code{cpp}
uint32_t W=640, H=480, WH=W*H;
FILE *f = fopen("img","rb");
uint8_t* buf = malloc(WH);
fread(buf, WH, 1, f);

int64_t t = cv::getTickCount();
uint8_t * img = (uint8_t*) malloc(WH);
memcpy(img, buf, WH);
t = cv::getTickCount() - t;
cout<<t/cv::getTickFrenquency()<<endl;

t = cv::getTickCount();
uint32_t sd=5, Wd=W/sd, Hd=H/sd;
uint8_t *img_d = (uint8_t*)malloc(Wd*Hd);
for(uint32_t r=0; r<Hd; r++)
  for(uint32_t c=0; c<Wd; c++)
    img_d[r*Wd+c] = buf[r*sd*W+c*sd];
t = cv::getTickCount()-t;
cout<<t/cv::getTickFrequency()<<endl;
\endcode
PC上运行的结果，t1是全拷贝的时间，t2是下采样拷贝的时间，可以看到在debug版本中，2倍采样和3倍采样时间都要长于全采样，直到4倍采样开始时间才小于全采样，这是因为下采样需要设计循环，依次赋值，全拷贝肯定在汇编层面做了优化的。
有意思的是在Release版本中，2倍采样的时间就小于全拷贝，这就说明debug版本对于循环有着更深的影响，下采样最大运行时间相差了接近6倍。

Debug版本

| sd   | 1    | 2    | 3    | 4    | 5    | 6    |
| ---- | ---- | ---- | ---- | ---- | ---- | ---- |
| t1   | 91.5 | 90.5 | 91.7 | 91.2 | 90.4 | 90.6 |
| t2   | 840  | 219  | 102  | 60.2 | 39.1 | 27.6 |

Release版本

| sd   | 1    | 2    | 3    | 4    | 5    | 6    |
| ---- | ---- | ---- | ---- | ---- | ---- | ---- |
| t1   | 74.4 | 72.7 | 74.3 | 74.6 | 77.3 | 75.1 |
| t2   | 158  | 62.6 | 30.5 | 18.5 | 12.3 | 8.96 |

ARM
| sd   | 1    | 2    | 3    | 4    | 5    | 6    |
| ---- | ---- | ---- | ---- | ---- | ---- | ---- |
| t1   | 644  | 654  | 652  | 645  | 670  | 685  |
| t2   | 667  | 208  | 97.6 | 28.2 | 69.0 | 48.4 |

<hr>
\subsection 浮点转定点中的四舍五入
浮点数转定点数不能直接截取，必须四舍五入，否则精度会丢失，有时候会丢失很多！
例如下面的代码，将浮点数转换成16Q8的定点数，当输入0.035时，0.035\*256.0=8.96，直接截取就变成了8，然而8对应着0.03125，9对应着0.03515625，很明显应该是9更合适。
我们自然而然地想到+0.5，例如第二行代码所示，但是这样仍然是有问题的，就是在使用负数的时候。
例如输入-0.6，那么照理说-0.6更接近-1，然而-0.6+0.5=-0.1会截取成0，没有错(int)(-0.1)=0，在VS下是这样的。

\code{cpp}
#define Q168_FROM_FLOAT(x) ( (int16_t)((x)*256.0) ) // 丢失精度
#define Q168_FROM_FLOAT(x) ( (int16_t)((x)*256.0+0.5) ) // 负数不准
#define Q168_FROM_FLOAT(x) ( (int16_t)((x)>=0?(x)*256.0+0.5:(x)*256.0-0.5) ) //完美

// 测试
printf("%d,%d\n", Q168_FROM_FLOAT(0.35), Q168_FROM_FLOAT(-0.35))
//结果为 9和-9，正确！
\endcode

<hr>
\section 代码规范篇

<hr>
\subsection 全局变量用处
1.静态表格常量
2.调试系统

调试系统是反封装的，调试系统往往需要获取某些很深的函数内部的某些中间变量，如果通过传参获取，
（1）那么函数参数就会很多，
（2）调试系统需要查看的变量集可能会经常变化，层层递归修改函数形参。

<hr>
\subsection C语言算法中的数学参数放在哪里？
程序运行过程中始终不变的数学常量称为参数，做算法的代码会有特别多的参数，要怎么放置这些参数呢？
1. 直接在调用的地方用数字常量表示，例如
\code{cpp}
x = 0.01 *y;
\endcode
这种方式被称为magic number, 不太好。
第一，不好理解，如果只有一个number，还勉强可以注释，如果一行代码有多个number，就真的不好注释了。
第二，如果这个参数在很多地方用到，参数变动的时候，需要多处修改，非常容易有漏网之鱼。

2.直接用宏来define数字，在调用的地方使用宏。
\code{cpp}
#define SLAM_PARA_XXXX 0.01
\endcode
这种方式比直接用数字要好，define单个数字是没问题的，但是在定义数组型参数的时候就不太方便了，例如

\code{cpp}
float para_xxx[2][3] = {{1.0000, 2.0000, 3.0000},
                        {4.0000, 5.0000, 6.0000}};
\endcode
一个很勉强的方式就是把宏定义成花括号里的内容，然后就在调用的地方新建一个变量=宏，但是这样很怪。。。
而且如果这个参数表很大的话，例如10x10的静态查找表，每次调用的地方新建变量并赋值一次会有很多额外消耗。

3. 用全局变量直接定义
    这种方式算是一种比较整洁的解决方案，但是全局变量用多了之后
    
    
<hr>
\section 实用代码片段

<hr>
\subsection 彩色规范打印

\code{c}
#indef PRINT_ERROR
#define PRINT_ERROR(...)    \
  do{                       \
    printf("\x1B[41;37m");  \
    printf("[ERROR]");      \
    printf("\x1B[0m");      \
    printf(__VA_ARGS__);    \
    }                       \
  while(0)
#endif
  
#ifndef PRINT_INFO
#define PRINT_INFO(...)     \
  do{                       \
    printf("\1xB[42;37m");  \
    printf("[INFO] ");      \
    printf(__VA_ARGS__);    \
    }                       \
  while(0)
#endif

\endcode

<hr>
\subsection 头文件多次包含于只展开一次矛盾？
写代码的时候发现一个“问题”，我们一般用#ifdef包裹头文件，保证头文件只展开一次，否则会出现重复定义。
那么多个c文件包含同一个头文件时，后面的c文件是否就无法获取h文件里的内容了呢？
当然，通过实际编程经验，我们都知道答案是否定的，但是怎么解释呢？
我自己的解释是，头文件展开过程和c文件包含h文件过程是两码事，头文件展开在第一次被c文件包含的时候进行，之后再次被c文件包含时，就不在进行，而是仅仅拷贝h文件已经展开的内容。

<hr>
\subsection 宏在c文件中的作用域
宏定义有时候不一定会在c文件中起作用，如下面的例子所示。
main.c依赖a.h和a.c，而a.h又依赖b.h，所以我们自然而然的认为在main.c里包含a.h之前加入某一个宏，它能够在a.h和b.h都生效。
在头文件中，这是没有问题的，但是对于b.c文件，就不管用了（实际工作碰到的例子），因为b.h和b.c在编译的时候他们并不依赖其他模块，所以有可能b模块比main编译的时刻还要早（编译成obj文件），另外，如果仅仅是修改了宏，a.h和a.c的内容不会改变，那么a.o根本就不会重新发生编译。
\code{c}
// main.c
#define TURN_OFF_DEVIL
#include <a.h>

// a.h
#include <b.h>
#ifndef TUNN_OFF_DEVIL
static int i_am_devel_1;
#endif

// b.h
// b.c
#include <b.h>
#ifndef TURN_OFF_DEVIL
int i_am_devel2;
#endif
\endcode
解决方法，使用编译工具的宏定义工具，例如cmake的
\code{c}make
target_compile_definitions(my_exe PRIVATE USE_SOMTHING=1)
\endcode

<hr>
\subsection 万不得已修改第三方代码怎么注释？
这种情况通常出现在两个第三方库之间有命名冲突，例如都存在某一些数学函数。
注释方法：
\code{c}
// Hacked by somebody
// End of Hacking
\endcode
用“hack”这个词非常好，既能够表达出这是对原有模块的破坏，一眼就能看出hack了那些地方，原来是什么代码，另外也很容易搜索。

<hr>
\subsection 255.1f转换成uint8_t是多少？
答案是255。

<hr>
\subsection 找不到头文件？
明明头文件的路径已经添加，并且在**QtCreator**里能够跟踪到该头文件，但是在**terminal** make的时候仍然找不到头文件？
有一个原因：CMakeList.txt用了环境变量$ENV{}，并且QtCreator设置了这个环境变量，但是bash里面并没有设置！
反过来，能编译，但是QtCreator老是找不到头文件，或者在QtCreator内部构建失败。

<hr>
\subsection 获取变量的名字
\code{c}
#include <stdio.h>

#define PRINTER(name) printer(#name, (name))

void printer(char *name, int value) {
    printf("name: %s\tvalue: %d\n", name, value);
}

int main (int argc, char* argv[]) {
    int foo = 0;
    int bar = 1;

    PRINTER(foo);
    PRINTER(bar);

    return 0;
}

name: foo   value: 0
name: bar   value: 1
\endcode

<hr>
\subsection 函数指针常量怎么定义？
\code{c}
// normal pointer to function
int (*func)(int);

// pointer to const function -- not allowed
int (const *func)(int);

// const pointer to function. Allowed, must be initialized.          
int (*const func)(int) = some_func;

// Bonus: pointer to function returning pointer to const
void const *(*func)(int);

// triple bonus: const pointer to function returning pointer to const.
void const *(*const func)(int) = func.
\endcode

<hr>
\subsection const也不是完美的常量？
C99中，全局变量和静态变量初始化必须使用常量，而且这个常量只能是字面上的常量，例如“1，0xff”以及枚举。
const修饰的常量是二等公民，不能用于初始化，坑了我很久。
例如
\code{c}
// C99
const int a =10;
int b = a; // 错误，const变量也不能用于全局变量初始化。
\endcode

<hr>
\subsection C语言调用C++函数的正确姿势
大家都知道，C++调用C语言模块很容易，就是在函数生命的地方使用#ifdef \_\_cplusplus extern "C" #endif 包起来就行。
但是有的时候需要在C语言环境中调用C++的模块，虽然这种调用感觉很不合理，但是仍然不可避免，例如组里的顶层框架代码是C，我们自身的模块是C++，这时候必须要把C++模块塞到C模块里。
首先，c++模块头文件需要把函数声明用extern "C"包裹，
其次，c模块在调用之前，使用extern重新声明该函数，相当于声明了两遍。另外c模块绝对不要include该c++模块的头文件，否则会报错。
最后，c模块正常调用c++模块函数。

其实到这里，我并不太理解为什么第一步要做，如果第二步直接重新声明了c++函数，并且压根没有include c++模块的头文件，那调用过程根本和c++头文件没有关系。
然而实际测试时，如果c++头文件去掉了extern "C"，还是会出现C linkage的问题。
这说明，extern "C"的包裹，虽然只是针对声明进行的，但是它会实际影响到定义部分，会让定义部分使用C的方式生成函数符号名。
那最后留一个疑问，如果extern "C"包裹的C++函数有重载，那么会发生什么呢？

\endcode
//C++头文件 cppExample.h
#ifndef CPP_EXAMPLE_H
#define CPP_EXAMPLE_H
extern "C" int add( int x, int y );
#endif

//C++实现文件 cppExample.cpp
#include "cppExample.h"
int add( int x, int y )
{
　return x + y;
}

/* C实现文件 cFile.c
/* 这样会编译出错：#include "cExample.h" */
extern int add( int x, int y );
int main( int argc, char* argv[] )
{
　add( 2, 3 );
　return 0;
}
\endcode

<hr>
\subsection 函数指针作为函数传参

\code{c}
typedef void (*callback_function)(void); // type for conciseness

callback_function disconnectFunc; // variable to store function pointer type

void D::setDisconnectFunc(callback_function pFunc)
{
    disconnectFunc = pFunc; // store
}

void D::disconnected()
{
    disconnectFunc(); // call
    connected = false;
}
\endcode

<hr>
\subsection sizeof用于静态数组
sizeof变量如果用在静态数组，那么是可以得到静态数组的总长度的，
\code{c}
double array[10];
sizeof(array); // 8*10 = 80
\endcode

<hr>
\subsection 下采样拷贝时间
下采样拷贝虽然点数少，但是需要多次拷贝操作，那么这种方式和全拷贝速度差异如何呢？

\code{cpp}
uint32_t W=640, H=480, WH=W*H;
FILE *f = fopen("img","rb");
uint8_t* buf = malloc(WH);
fread(buf, WH, 1, f);

int64_t t = cv::getTickCount();
uint8_t * img = (uint8_t*) malloc(WH);
memcpy(img, buf, WH);
t = cv::getTickCount() - t;
cout<<t/cv::getTickFrenquency()<<endl;

t = cv::getTickCount();
uint32_t sd=5, Wd=W/sd, Hd=H/sd;
uint8_t *img_d = (uint8_t*)malloc(Wd*Hd);
for(uint32_t r=0; r<Hd; r++)
  for(uint32_t c=0; c<Wd; c++)
    img_d[r*Wd+c] = buf[r*sd*W+c*sd];
t = cv::getTickCount()-t;
cout<<t/cv::getTickFrequency()<<endl;
\endcode

PC上运行的结果，t1是全拷贝的时间，t2是下采样拷贝的时间，可以看到在debug版本中，2倍采样和3倍采样时间都要长于全采样，直到4倍采样开始时间才小于全采样，这是因为下采样需要设计循环，依次赋值，全拷贝肯定在汇编层面做了优化的。
有意思的是在Release版本中，2倍采样的时间就小于全拷贝，这就说明debug版本对于循环有着更深的影响，下采样最大运行时间相差了接近6倍。

Debug版本

 sd | 1    | 2    | 3    | 4    | 5    | 6    
 -- | ---- | ---- | ---- | ---- | ---- |----
 t1 | 91.5 | 90.5 | 91.7 | 91.2 | 90.4 | 90.6 
 t2 | 840  | 219  | 102  | 60.2 | 39.1 | 27.6 

Release版本

| sd | 1    | 2    | 3    | 4    | 5    | 6    |
| -- | ---- | ---- | ---- | ---- | ---- | ---- |
| t1 | 74.4 | 72.7 | 74.3 | 74.6 | 77.3 | 75.1 |
| t2 | 158  | 62.6 | 30.5 | 18.5 | 12.3 | 8.96 |
 
ARM
| sd | 1    | 2    | 3    | 4    | 5    | 6    |
| -- | ---- | ---- | ---- | ---- | ---- | ---- |
| t1 | 644  | 654 | 652  | 645 | 670 | 685 |
| t2 | 667  | 208 | 97.6 | 28.2 | 69.0 | 48.4 |


<hr>
\subsection 图像浏览工具
做图像算法的时候经常要处理图像buffer，就是最原始指针那种，因为程序运行在嵌入式平台，所以有的时候调试起来看图不是特别方便，很多时候我们从板子上存下来的图是原始的raw格式，或者我们想传入我们自己的图像进行测试，这时候就需要某种图像转换和浏览工具。

下面是代码，其实思路很简单，使用opencv将raw格式的文件打开，然后转换成bmp，再使用bat调用图片浏览工具，实现双击就能浏览的效果。
这里的bin格式是我自己定义的，bin表示单通道，bin3表示3通道，通常是BGR。
代码中转换单通道的函数没有填写，这里只填写了3通道的。

\code{cpp}
#include <iostream>
#include <fstream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;
typedef unsigned char uint8_t;
typedef unsigned int uint32_t;

#define PRINT_AND_RETURN(x) do{cout<<x<<endl; return 1;}while(0)
#define PRINT_USAGE_AND_RETURN() PRINT_AND_RETURN("Usage: bin_viewer.exe <input file> <output file>")

int convert_bin_to_bmp(string in_path, string out_path)
{
    return 0;
}

int convert_bin3_to_bmp(string in_path, string out_path)
{
    ifstream f;
    f.open(in_path, ios::binary);
    uint32_t W,H;
    f.read((char*)&W, sizeof(uint32_t));
    f.read((char*)&H, sizeof(uint32_t));
    Mat save(H,W, CV_8UC3);
    f.read((char*)save.data, 3*W*H);
    imwrite(out_path, save);
    return 0;
}

int main(char argc, char **argv)
{
    if(argc<3)
        PRINT_USAGE_AND_RETURN();

    string in_path = string(argv[1]);
    if(in_path.substr(in_path.size()-3, 3) == "bin")
        convert_bin_to_bmp(string(argv[1]), string(argv[2]));
    else if(in_path.substr(in_path.size()-4, 4) == "bin3")
        convert_bin3_to_bmp(string(argv[1]), string(argv[2]));
    else
        PRINT_AND_RETURN("only open bin or bin3 file!");

    cout<<"done!"<<endl;
    return 0;
}

\endcode
接下来就是编写bat文件，调用exe、传入参数并且调用图片浏览工具。
其中%1就是传入的参数，对，没错，就是双击的文件的路径，不要怀疑，微软爸爸已经给你做好了！
给bin文件和bin3文件选择默认的打开方式，设置成bat文件，对，就是右键选择默认程序！
ImageGlass是一个网上下载的轻量级的浏览工具。

\code{bash}
bin_viewer.exe %1 out.bmp
"C:\Program Files\ImageGlass\ImageGlass.exe" out.bmp
\endcode

<hr>
\subsection 遇到unresolved external symbol怎么办？
如果这个symbol在源文件里：
- 看看这个源文件有没有被加到工程中。
- symbol在源文件中，并且已经添加到了工程，还会报这个错？看看是不是.c文件和.cpp文件互相调用了，如果互相调用，记得用EXTERN C封装。
- 都是cpp文件或者c文件，或者已经用EXTERN C封装了，还会出错？看看是不是不同源文件字符编码的问题。
- 以上条件都满足，但在qtcreator中刚添加的函数还是报这个错？那就试着显式地qmake一下，然后再rebuild。
- 如果还有问题，那我也不知道怎么办了，就把电脑砸了吧。

如果这个symbol在库文件中：
- 看看这个库有没有添加到工程。
- 库已经添加到工程，但是仍然报这个错？那就将库的版本和当前编译器版本保持一致，包括平台（x86或者x64）和版本（vc08，vc10，vc12，vc14）以及是否调试（debug和release）。

<hr>
\subsection 工程内外的头文件区别？
C语言头文件不管是放在工程内部还是工程外部都可以include，那么这两种方式有什么区别呢？
目前发现的区别有一点，就是工程内部的头文件内部再include别的头文件的时候可以享用工程文件已经添加的路径。
举个例子，a.h是工程部内的，b.h是工程外部的，main.cpp里面把这两个头文件都include了，工程pro（qtcreator）文件里包含了opencv的库目录。
那么打开a.h时输入“include <opencv.....”，此时creator会自动补全，但是在b.h中输入则没有任何反应，这就是目前发现的一个区别。
但是我估计实际编译的时候应该没有问题，这只是IDE的识别问题而已。

<hr>
\subsection core.hpp header must be compiled as c++
在c文件中include Opencv的hpp头文件就会出现这个问题，因为编译c文件时是使用C编译器编译的，而.c文件是不认识.hpp文件的，实现的方式就是c编译器并未定义__cplusplus宏。按照道理说c文件不应该包含h文件，最好把c文件改成cpp文件。如果硬是不改，在VisualStudio中可以设置compile as c++。在别的环境就不知道怎么搞了。

<hr>
\subsection 浮点转定点中的四舍五入
浮点数转定点数不能直接截取，必须四舍五入，否则精度会丢失，有时候会丢失很多！
例如下面的代码，将浮点数转换成16Q8的定点数，当输入0.035时，0.035\*256.0=8.96，直接截取就变成了8，然而8对应着0.03125，9对应着0.03515625，很明显应该是9更合适。
我们自然而然地想到+0.5，例如第二行代码所示，但是这样仍然是有问题的，就是在使用负数的时候。
例如输入-0.6，那么照理说-0.6更接近-1，然而-0.6+0.5=-0.1会截取成0，没有错(int)(-0.1)=0，在VS下是这样的。

\code{cpp}
#define Q168_FROM_FLOAT(x) ( (int16_t)((x)*256.0) ) // 丢失精度
#define Q168_FROM_FLOAT(x) ( (int16_t)((x)*256.0+0.5) ) // 负数不准
#define Q168_FROM_FLOAT(x) ( (int16_t)((x)>=0?(x)*256.0+0.5:(x)*256.0-0.5) ) //完美

// 测试
printf("%d,%d\n", Q168_FROM_FLOAT(0.35), Q168_FROM_FLOAT(-0.35))
//结果为 9和-9，正确！
\endcode

<hr>
\subsection printf 中的uint64整数
使用printf过程中，一定要写对%字符串，之前遇到过一个问题，使用%d来打印一个int64的数是错误的，例如下列代码

\code{cpp}
uint64_t a = 1000;
printf("%d\n",a); // 错误
\endcode

实际上，%u，%ld，%lu都不对，在曾经一个ARM平台编译器上，正确的结果是%llu，因为在该平台上，sizeof(long)=sizeof(int)=4，而uint64_t真正的类型是long long，在不同平台时，要注意类型字节数可能不一样。


<hr>
\subsection 多周期算法
两种结构：
1. 启动计算独立
2. 启动计算不独立

<hr>
\subsection linux-arm-gcc 中的char默认是unsigned char!!!
好可怕，给char类型变量赋值一个负数，会得到错误的结果，而且编译器不会报错或者警告
只有使用==判别时才提出警告。

\code{c}
char a = -1;
printf("%d\n",a );  //打印出 255
if(a == -10)
  printf("error") // 编译报错
\endcode

<hr>
\subsection C语言算法中的数学参数放在哪里？
程序运行过程中始终不变的数学常量称为参数，做算法的代码会有特别多的参数，要怎么放置这些参数呢？
1. 直接在调用的地方用数字常量表示，例如
\code{cpp}
x = 0.01 *y;
\endcode
这种方式被称为magic number, 不太好。
第一，不好理解，如果只有一个number，还勉强可以注释，如果一行代码有多个number，就真的不好注释了。
第二，如果这个参数在很多地方用到，参数变动的时候，需要多处修改，非常容易有漏网之鱼。

2.直接用宏来define数字，在调用的地方使用宏。
\code{cpp}
#define SLAM_PARA_XXXX 0.01
\endcode
这种方式比直接用数字要好，define单个数字是没问题的，但是在定义数组型参数的时候就不太方便了，例如
\code{cpp}
float para_xxx[2][3] = {{1.0000, 2.0000, 3.0000},
                        {4.0000, 5.0000, 6.0000}};
\endcode
一个很勉强的方式就是把宏定义成花括号里的内容，然后就在调用的地方新建一个变量=宏，但是这样很怪。。。
而且如果这个参数表很大的话，例如10x10的静态查找表，每次调用的地方新建变量并赋值一次会有很多额外消耗。

3. 用全局变量直接定义
这种方式算是一种比较整洁的解决方案，但是全局变量用多了之后

<hr>
\subsection 一个float不够用的例子
x = {3137995, 3137895, 3137978}
A = {{x0\*x0, x0, 1},{x1\*x1, x1, 1},{x2\*x2, x2, 1}}

<hr>
\subsection 另一个float不够用的例子
以纳秒为单位UTC时间数值上特别大，举个例子（946,685,275,874,349,312，这一连串是一个数字），大概是30年左右。

<hr>
\subsection 头文件重复包含
我们都知道头文件一开始两行都是#ifndef和#define，是为了避免头文件重复包含，从逻辑上和减少编译时间看，都需要消除这种重复包含，
那么消除重复包含的工作为什么不丢给程序员做呢？
1. 这样的工作非常枯燥而繁琐，包含的链路太长的话，光是找到公共头文件节点就很麻烦；
2.更重要的是，部分模块常常需要独立运行，这种冗余保证了模块也能够独立地运行。

<hr>
\subsection extern 声明
默认的全局变量都开放成全局可用变量，其他模块想用该变量只需要extern声明即可。
例如a.c文件中定义int a = 0; b.c文件中声明extern int a;
那么b文件中使用的变量其实就是a文件中的变量
**a.c文件和b.c文件不需要什么相互依赖的关系，也和头文件无关**。
但是一般来说，在a.h中声明extern int a;然后b.c文件再include a.h文件，这样的用法会比较规范。



<hr>
\subsection 全局变量用处
1.静态表格常量
2.调试系统

调试系统是反封装的，调试系统往往需要获取某些很深的函数内部的某些中间变量，如果通过传参获取，
（1）那么函数参数就会很多，
（2）调试系统需要查看的变量集可能会经常变化，层层递归修改函数形参

<hr>
\subsection \_Bool变量
将在线代码分离成离线代码的时候，发现有些文件里面定义了_Bool类型，而gcc编译器居然不识别。
Google后发现，_Bool类型是C语言的布尔类型，从C99才开始引入的。
C99以前的C语言都是用int或者枚举的野路子来表示布尔型。。。
我们平时用的bool都是C++的东西。。。
也就是说如果在 extern "C"中间使用bool，是不被支持的，除非引用了头文件stdbool.h


<hr>
\subsection 将指针赋值给uint32_t
很多32位平台的程序喜欢使将指针赋值给整数，这样就不需要二级指针，例如

\code{cpp}
int func(uint32_t *addr)
{
  void * data = malloc (1000);
  *addr = (uint32_t) data;
}
\endcode
如果需要在函数里面开内存，输出内存地址，那么要么使用二级指针，要么用这种转地址的方式。
但是，这种方式其实并不好，在64位系统中，指针的大小可能是64位的，这样赋值就会出错。
仍然像想用这种方式的话，使用intptr_t类型，兼容32和64位，在C99中支持，在<stdint.h>中定义。

<hr>
\subsection designated initializer
C语言中结构体在C99中的一种新的初始化语法，叫做designated initializer，即指定初始化，之前我一直找不到正式的名称，我一直称之为点变量初始化。。。

\code{cpp}
MY_TYPE a = { .flag = true, .value = 123, .stuff = 0.456 };
\endcode
这个东西与编译器无关，是C99的标准，gcc想用的话必须开启选项 -std=c99

如果使用qtcreator构建工程，那么默认使用g++编译，然而如果designated initializer语句是在cpp文件里，g++是不支持的，不管CXX_FLAG 是不是加了c++11，或者C_FLAG 加了99，或者加入extern "C"，都不行， extern "C"只是指示c++编译器把函数编成C语言可识别的符号表，并不支持这种赋值语法。

但是我发现了一种方法，就是强行改cpp后缀为c，这样g++就会调用gcc来编译c文件。

<hr>
\subsection代码区分不同操作系统
\code{cpp}
#ifdef __linux__ 
    //linux code goes here
#elif _WIN32
    // windows code goes here
#else
    cout<<"OS not supported!"<<endl;
#endif
\endcode

<hr>
\subsection utf-8文件的bom字节
很蛋疼，读utf-8文件前面3个字节都不是文件中的内容，而是0xef,0xbb,0xbf，后来一查，发现这是utf-8文件的bom字节，就是byte order mark,
读utf-8文件时，一定要去掉前面三个字节。

<hr>
\subsection utf-8文件中的行结尾'\r'
很蛋疼+1，读utf-8文件的时候，行结尾都是以'\r'结束，
当文件内容是另一个文件的路径的时候，例如
\endcode
/mnt/cc/随便
\endcode
其实这一行读出来是这样的
\endcode
'/','m',','t','/','c','c','/','随,'便',\r
\endcode
我一般喜欢在文件路径后面加上'/'，方便后面读取。
但是如果在\r后面加上'/'就会搞错。。

<hr>
\subsection qtcreator中开启gcc的c99支持
\endcode
QMAKE_CFLAGS += -std=c99
\endcode

<hr>
\subsection printf 如何输出红色字符串？
\code{cpp}
#include<stdarg.h>
...
void printf_red(const char *cmd, ...)  
{  
    printf("\x1B[31m");  //设置输出为红色
    va_list args;       //定义一个va_list类型的变量，用来储存单个参数  
    va_start(args,cmd); //使args指向可变参数的第一个参数  
    vprintf(cmd,args);  //必须用vprintf等带V的  
    va_end(args);       //结束可变参数的获取  
    printf("\x1B[0m")   // 回到默认设置
}  
\endcode


<hr>
\subsection 规范打印

\code{c}
#indef PRINT_ERROR
#define PRINT_ERROR(...)    \
  do{                       \
    printf("\x1B[41;37m");  \
    printf("[ERROR]");      \
    printf("\x1B[0m");      \
    printf(__VA_ARGS__);    \
    }                       \
  while(0)
#endif
  
#ifndef PRINT_INFO
#define PRINT_INFO(...)     \
  do{                       \
    printf("\1xB[42;37m");  \
    printf("[INFO] ");      \
    printf(__VA_ARGS__);    \
    }                       \
  while(0)
#endif
    

\endcode

<hr>
\subsection 重复定义
如果发现某个变量明明只有一处，但是编译器老是提出重复定义，就要看看头文件是否没有写#ifndef #define #endif三连。

<hr>
\subsection .o文件里发现重复定义
很有可能的原因是：
在头文件中定义了变量和函数，这会导致头文件被多个c文件引用，h文件里的变量和函数就会出现在多个.o文件中，最后链接的时候出现错误。

<hr>
\subsection 二维数组的传参和“引用”
\code{cpp}
void func_a(int a[][10])
{
...
}
// 或者
void func_b(int (*a)[10])
{
...
}

void main()
{
  int a[5][10];
  int (*b)[10] = a;
}
\endcode
注意：第二种方式中的`*b`必须使用括号括起来，否则就变成了一维的数组指针了。

<hr>
\subsection No rules to make target
这个提示表示编译模块时时找不到源代码，检查一下C和C++文件是否在构建脚本中添加进去了。

<hr>
\subsection 使用enum代替整数的好处
防止数组访问溢出
假设在函数func里操作一个size为N的数组，函数传入需要操作的index，那么在没有前置判断溢出的情况下，需要加入判断是否溢出的语句。
\code{c}
int a[10]
int func(int x)
{
  if(x>=10)
  {
    return 1;
  }
  printf("%d",a[x]);
}
\endcode
如果数组的size不大，而且具有明确的含义，例如某种过程处理通道的个数，可以考虑使用枚举来代替。
使用枚举的好处是，枚举变量一定不会超出定义域的范围，在编译阶段就决定了不会产生数组的越界。

<hr>
\subsection `static const char *` 定义但未使用
定义全局的`static const char*`字符串会提示变量定义但未使用。
原因是未解之谜，可以考虑换种方法。
\code{c}
static const char * a = "abcdef"; //有问题
static cosnt char a[] = "abcdef"; //OK
\endcode

<hr>
\subsection 运算符优先级cheatsheet

<table class="wikitable">

<tbody><tr>
<th style="text-align: left"> 优先级
</th>
<th style="text-align: left"> 运算符
</th>
<th style="text-align: left"> 描述
</th>
<th style="text-align: left"> 结合性
</th></tr>
<tr>
<th rowspan="6"> 1
</th>
<td style="border-bottom-style: none"> <code>++</code> <code>--</code>
</td>
<td style="border-bottom-style: none"> 后缀自增与自减
</td>
<td style="vertical-align: top" rowspan="6"> 从左到右
</td></tr>
<tr>
<td style="border-bottom-style: none; border-top-style: none"> <code>()</code>
</td>
<td style="border-bottom-style: none; border-top-style: none"> 函数调用
</td></tr>
<tr>
<td style="border-bottom-style: none; border-top-style: none"> <code>[]</code>
</td>
<td style="border-bottom-style: none; border-top-style: none"> 数组下标
</td></tr>
<tr>
<td style="border-bottom-style: none; border-top-style: none"> <code>.</code>
</td>
<td style="border-bottom-style: none; border-top-style: none"> 结构体与联合体成员访问
</td></tr>
<tr>
<td style="border-bottom-style: none; border-top-style: none"> <code>-&gt;</code>
</td>
<td style="border-bottom-style: none; border-top-style: none"> 结构体与联合体成员通过指针访问
</td></tr>
<tr>
<td style="border-bottom-style: none; border-top-style: none"> <code>(<i>type</i>){<i>list</i>}</code>
</td>
<td style="border-bottom-style: none; border-top-style: none"> 复合字面量<span class="t-mark">(C99)</span>
</td></tr>
<tr>
<th rowspan="8"> 2
</th>
<td style="border-bottom-style: none"> <code>++</code> <code>--</code>
</td>
<td style="border-bottom-style: none"> 前缀自增与自减<sup id="cite_ref-1" class="reference"><a href="#cite_note-1">[注 1]</a></sup>
</td>
<td style="vertical-align: top" rowspan="8"> 从右到左
</td></tr>
<tr>
<td style="border-bottom-style: none; border-top-style: none"> <code>+</code> <code>-</code>
</td>
<td style="border-bottom-style: none; border-top-style: none"> 一元加与减
</td></tr>
<tr>
<td style="border-bottom-style: none; border-top-style: none"> <code>!</code> <code>~</code>
</td>
<td style="border-bottom-style: none; border-top-style: none"> 逻辑非与逐位非
</td></tr>
<tr>
<td style="border-bottom-style: none; border-top-style: none"> <code>(<i>type</i>)</code>
</td>
<td style="border-bottom-style: none; border-top-style: none"> 类型转型
</td></tr>
<tr>
<td style="border-bottom-style: none; border-top-style: none"> <code>*</code>
</td>
<td style="border-bottom-style: none; border-top-style: none"> 间接（解引用）
</td></tr>
<tr>
<td style="border-bottom-style: none; border-top-style: none"> <code>&amp;</code>
</td>
<td style="border-bottom-style: none; border-top-style: none"> 取址
</td></tr>
<tr>
<td style="border-bottom-style: none; border-top-style: none"> <code>sizeof</code>
</td>
<td style="border-bottom-style: none; border-top-style: none"> 取大小<sup id="cite_ref-2" class="reference"><a href="#cite_note-2">[注 2]</a></sup>
</td></tr>
<tr>
<td style="border-bottom-style: none; border-top-style: none"> <code>_Alignof</code>
</td>
<td style="border-bottom-style: none; border-top-style: none"> 对齐要求<span class="t-mark">(C11)</span>
</td></tr>
<tr>
<th> 3
</th>
<td> <code>*</code> <code>/</code> <code>%</code>
</td>
<td> 乘法、除法及余数
</td>
<td style="vertical-align: top" rowspan="11"> 从左到右
</td></tr>
<tr>
<th> 4
</th>
<td> <code>+</code> <code>-</code>
</td>
<td> 加法及减法
</td></tr>
<tr>
<th> 5
</th>
<td> <code>&lt;&lt;</code> <code>&gt;&gt;</code>
</td>
<td> 逐位左移及右移
</td></tr>
<tr>
<th rowspan="2"> 6
</th>
<td style="border-bottom-style: none"> <code>&lt;</code> <code>&lt;=</code>
</td>
<td style="border-bottom-style: none"> 分别为 &lt; 与 ≤ 的关系运算符
</td></tr>
<tr>
<td style="border-top-style: none"> <code>&gt;</code> <code>&gt;=</code>
</td>
<td style="border-top-style: none"> 分别为 &gt; 与 ≥ 的关系运算符
</td></tr>
<tr>
<th> 7
</th>
<td> <code>==</code> <code>!=</code>
</td>
<td> 分别为 = 与 ≠ 关系
</td></tr>
<tr>
<th> 8
</th>
<td> <code>&amp;</code>
</td>
<td> 逐位与
</td></tr>
<tr>
<th> 9
</th>
<td> <code>^</code>
</td>
<td> 逐位异或（排除或）
</td></tr>
<tr>
<th> 10
</th>
<td> <code>|</code>
</td>
<td> 逐位或（包含或）
</td></tr>
<tr>
<th> 11
</th>
<td> <code>&amp;&amp;</code>
</td>
<td> 逻辑与
</td></tr>
<tr>
<th> 12
</th>
<td> <code>||</code>
</td>
<td> 逻辑或
</td></tr>
<tr>
<th> 13
</th>
<td> <code>?:</code>
</td>
<td> 三元条件<sup id="cite_ref-3" class="reference"><a href="#cite_note-3">[注 3]</a></sup>
</td>
<td style="vertical-align: top" rowspan="6"> 从右到左
</td></tr>
<tr>
<th rowspan="5"> 14<sup id="cite_ref-4" class="reference"><a href="#cite_note-4">[注 4]</a></sup>
</th>
<td style="border-bottom-style: none"> <code>=</code>
</td>
<td style="border-bottom-style: none"> 简单赋值
</td></tr>
<tr>
<td style="border-bottom-style: none; border-top-style: none"> <code>+=</code> <code>-=</code>
</td>
<td style="border-bottom-style: none; border-top-style: none"> 以和及差赋值
</td></tr>
<tr>
<td style="border-bottom-style: none; border-top-style: none"> <code>*=</code> <code>/=</code> <code>%=</code>
</td>
<td style="border-bottom-style: none; border-top-style: none"> 以积、商及余数赋值
</td></tr>
<tr>
<td style="border-bottom-style: none; border-top-style: none"> <code>&lt;&lt;=</code> <code>&gt;&gt;=</code>
</td>
<td style="border-bottom-style: none; border-top-style: none"> 以逐位左移及右移赋值
</td></tr>
<tr>
<td style="border-top-style: none"> <code>&amp;=</code> <code>^=</code> <code>|=</code>
</td>
<td style="border-top-style: none"> 以逐位与、异或及或赋值
</td></tr>
<tr>
<th> 15
</th>
<td> <code>,</code>
</td>
<td> 逗号
</td>
<td> 从左到右
</td></tr></tbody></table>

<hr>
\section GCC 编译(链接）报错指南

<hr>
\subsection undefined reference
1.（初级）变量或者函数没有定义
2.（初级）变量或者函数所在的源文件没有加入到编译配置里

<hr>
\subsection multiple definition
1.（初级）在多个源文件中定义了相同的非static函数或者非static变量。
2.（初级）Coding层面上确实只定义了一个变量，但是这个变量定义在头文件中，而且是非static的，一旦有多个源文件包含这个头文件，这一个变量就会变成多个。
3.（中级）不同的lib或者exe编译了同一个源文件，而且.o文件放在了不同的目录，Linker会发现多个同名.o文件，自然里面的变量也会重复定义。


<hr>
\subsection 怎样printf size_t类型才不会报warning
\code{c}
// C89
size_t foo;
...
printf("foo = %lu\n", (unsigned long) foo);
// For C99 and later, use %zu:

size_t foo;
...
printf("foo = %zu\n", foo);
\endcode

<hr>
\section 判断数字是否是nan

isnan()函数。

<hr>
<section> 角度归一化

\code{.cpp}
#include <iostream>
#include <cmath>

using namespace std;

double normAngle2pi(double x){
    x = fmod(x,360);
    return (x<0) ? x+360 : x;
}

double normAnglepp(double x){
    x = fmod(x + 180,360);
    return (x<0) ? x+180 : x-180; 
}

int main()
{
    cout<<normAngle2pi(-30)<<","<<normAngle2pi(370)<<endl;
    cout<<normAnglepp(-30)<<","<<normAnglepp(370)<<endl;
}

int main()
{
    cout<<normAngle2pi(-30)<<","<<normAngle2pi(370)<<endl;
    cout<<normAnglepp(-30)<<","<<normAnglepp(370)<<endl;
}

\endcode

<hr>
\section 指针中的“+”号

指针的加号运算符得到的结果和指针的类型是有关系的，每一次+1，相当于指针偏移sizeof(类型)个字节。
在memcpy这些操作的时候千万要注意。

\code{.cpp}
#include <stdint.h>
#include <stdio.h>

int main()
{
    char *p = 0;
    int16_t *p2 = 0;
    int32_t *p4 = 0;
    int64_t *p8 = 0;
    
    printf("%p,%p,%p,%p", p+1,p2+1,p4+1,p8+1);  // 运行结果: 0x1,0x2,0x4,0x8 
}

\endcode


<hr>
\section uin64_overflow 一个uint64_t无法满足需求的实际例子。

使用纳秒来表示从公元0年到公元2000年的时间。

需要表达的时间大致为：2000*365*24*3600*1e9 = 63,072,000,000,000,000,000

而uint64能表达的最大数值为：numeric_limits<uint64_t>::max() = 18,446,744,073,709,551,615

显然，uint64_t无法表达这个数。

所以在使用ns来表示时间的时候，一定要注意溢出，连uint64都不够用。

<hr>
\section 字面常量整型表达式溢出

即使变量长度没有问题，常量表达式也会溢出，如下代码所示。
想在想要使用纳秒来表达8个小时的时长，第一种计算方法是错误的，“8”，“3600“以及”1000000000“都是int32_t，
它们每个都在int32_t的表达范围内，但是乘起来就会超出。因为三个常量都是int32_t类型，所以编译器最终选择输出int32_t类型，最终常量溢出。
哪怕前面用了uin64_t的变量来接受赋值，但是右边的计算已经溢出了。

第二种方法是正确的，随便在其中一个数字后面加上LL，那么编译器最终会选择LL类型输出，也就是uint64_t，这样计算就不会溢出。

\code{cpp}
#include <iostream>
#include <stdint.h>

using namespace std;

int main(){
    uint64_t x = 8 * 3600 * 1000000000;
    cout<<"x="<<x<<endl;
    uint64_t y = 8 * 3600 * 1000000000LL;
    cout<<"y="<<y<<endl;
}
\endcode

\code{bash}
x=18446744071658864640
y=28800000000000
\endcode

\section 编译错误：＂can not used when making shared object＂
完整的报错信息为：

\code{bash}
relocation R_X86_64_PC32 against symbol `_ZGVZN4pcpp8LoggerPP11getInstanceEvE8instance' can not be used when making a shared object; recompile with -fPIC
\endcode

这个问题原因一般是动态库链接了静态库导致的，一般来说动态库是不会链接静态库的，因为动态库遵循引用的规范，不会把静态库塞到自身之中。
如果是引用别人的库，那要改成引用shared库，如果是引用自己的库，那么在add_library中增加“SHARED”修饰。