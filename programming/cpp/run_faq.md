# C++运行常见问题


## vector::push_back函数中会执行析构函数

记住push_back对象的时候会提前拷贝，拷贝的临时对象会被销毁，销毁时会调用析构函数，如果析构函数中有对static成员变量的操作，
那么就很容易导致结果不符合预期，例如下面的例子，a用来记录A对象的实例个数，我们预期是每push_back一个对象，那么count就会+1。
然而在push_back过程中，析构函数内a又被进行了-1操作，所以a仍然是0。

\code{.cpp}
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

\endcode

结果输出

    deconst, a:0
    deconst, a:-1


# system_work_dir system()函数工作路径？
system（）函数即使调用了其他路径的可执行文件，可执行文件也相当于在当前路径下运行，举个例子，在~/目录下运行a.exe，a.exe里有一句话是

\code{cpp}
system("f/b.exe");
\endcode
b.exe在代码里写这一句生成"c.txt"文件。
那么这个c.txt到第一会在哪里生成呢？是在“~/”，还是“~/f/”？
答案是“~/”，也就是a.exe调用的地方，这里才是工作路径，所以程序里面写着在当前路径生成某个文件时，并不是指可执行文件所在的路径，而是工作路径，只不过没有嵌套调用可执行文件时，一般工作路径就是可执行文件的路径而已。


# 设置工作路径
紧接上一个问题，那么如何改变exe运行时候的工作路径呢？
Linux环境下
\code{cpp}
#include <unistd.h>
...
chdir(working_dir)
...
\endcode
其中，unistd表示unix环境下的标准接口，所以在windows下是用不了的。

Windows环境下
\code{cpp}
#inlcude <direct.h>

...
_chdir(working_dir)
...
\endcode


# typo_sizeof 初级错误：sizeof()的用法

如下代码所示，虽然问题很初级，但是有时头晕的时候还是会犯！

\code{c}
bin_file.wirte((char*）imgCT.data, sizeof(imgIn.cols*imgIn.rows); //错误
bin_file.wirte((char*）imgCT.data, imgIn.cols*imgIn.rows*sizeof(char)；//正确
\endcode


# 初级错误：移位操作符

C语言中的移位符号不代表赋值，只有用等号赋值之后，变量才会改变。

\code{c}
a>>1; // 不赋值
a = a>>1; // 赋值
\endcode


# 隐藏致命：函数内部malloc或者new

通常我们希望函数内部给传递的指针参数分配内存，但是这时候传递的指针一定要使用二级指针，否则传递的指针是无法得到新分配的内存地址的。


# 隐藏致命：直方图计算出现越界

用C语言写histogram函数时，一定要注意max-min后面要加1！例如以下代码

\code{c}
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
\endcode

这段代码运行时没有出exception，运行结果也是正确的，但是free时却出错了，当时一万个想不通，直到后来发现少了一个加1。
如果少了加1，举个例子，max=255，min=0，当pixel=255时，loc=bins，这时候就会访问越界！是的访问越界时候没有任何提示！
只有在free时才会显现出来！
其实max-min并不是整整的个数，真正的个数是max-min+1，例如1到256有256个数，时因为256-1+1=256。

遇到这种摸不着头脑的问题，逐步注释代码排除才是王道！往往认为不可能出现问题的地方才是问题隐藏的最深的地方。


# cpp_debug_release debug和release的不要混用！
debug时间，2017-09-25 20:00-22：00

一个de了快**两个小时**的bug！
一段将二进制单通道图像转换成png图像的代码，文件前8个字节是W和H，接着是W\*H长度的图像数据。
在imwrite的时候一直出错，W和H读出来的是正确的，搞了半天一直没有头绪，非常的苦恼。
其中还怀疑过是不是QtCreator的问题，然后用VisualStudio试了一遍，发现VisualStudio居然可以！（VS一开始也有问题，但随即发现是自己文件路径不对）
然后仔细对比了一下，发现在VS下链接的OpenCV库文件是opencv_world310d.lib，并且运行的方式是DEBUG。
然而在Creator中我链接的库是opencv_world310.lib，并且运行的方式是DEBUG。
加上后缀"d"之后，终于可以运行了！或者将运行方式改成release，同样也可以运行！
这个bug在之前的imread函数是没有问题的，这进一步搞混淆了！

\code{cpp}
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
\endcode

**教训：** lib库的引用一定要**遵循规范**！debug版本的代码要引用debug版本的lib，release版本的代码要引用release版本的库。
举一反三，x64的编译方式用x64的库，x86的编译方式用x86的库，vc08，vc10，vc12的库最好也一致！


 
# 处理三通道图像出错？
2017-09-26, debug时间，10分钟。

下面一段代码将float图像转换成color_map形式，在处理每个通道的数据时，地址没有写对，W和c忘记乘上通道数了。
\code{cpp}
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
\endcode

**教训：**处理多通道图像一定要注意寻址和通道的关系。


# 定点化的程序反而更慢？
2017-10-11 debug时间：1天

使用定点小数进行优化，在PC上跑是有1/5的时间减少的，但是移植到ARM上，发现反而运行的时间还更长，差不多是1.5倍左右，很纳闷。

原因：最后查到了程序中有一个限制运算数量的宏常数写错了。。。。本来要限制在XXX_YYY=1500的，结果却成了XXX_APP_YYY=5000。
两个宏搞混了，原因就是在pc版只有XXX_YYY这个宏，而在ARM版本上却将XXX_YYY偷偷改成了XXX_APP_YYY。

**教训：**

1. 不能太相信develop中的程序，任何地方都有可能变更，要有强大的内心的面对这一切，也要有仔细观察的耐心。

2. 产生问题的根本原因是在线工程和离线工程代码不一致的问题，这时候如果使用基于多态实现的在线离线应用程序框架，就不会出现问题，因为在线工程和离线工程将会使用同一个宏。



# 数据读取出错？
2017-10-26 debug时长：2小时
debug发现数据存储结构不符合预期，但是没找到为什么不符合预期，第二天重新测了一组数据，又变好了！

教训：嵌入式程序数据记录可能会有偶然问题，发现问题时一定要多测几组（至少两组）来先确认问题！


# fabs出错？
2017-11-03 debug时长：10分钟
描述：fabs函数结果不对？这个问题很明显，printf出来就有问题。
原因：没有加入math.h头文件，这个原因很坑爹，因为没有头文件，编译居然不报错。
一个只依赖于标准库的程序没遇到过这种问题，这个问题出现在引用别人的库的前提下，有的时候别人的库定义了这个函数，但是并不是正确的，或者是编译选项选择忽略这种问题，例如我在工作中，就发现一些找不到定义的函数只有警告而不是错误。

教训：这种明显很奇怪的问题，一般是和环境相关的，头文件什么的不要省，而且要怀疑提出编译的人。


# 加了新功能，然后其他“不相关”模块崩溃？
2017-12-06 debug时长， 30分钟

原因，通过指针形式获取别的线程结构体内容，然后处理过程中别的线程改动了那块内存上的数据，有可能那块内存已经被释放。

报错那是比较轻的结果，最可怕的是，刚好没有报错，那块内存是别人在用的，你偷偷踩了别人的内存，而别人的模块除了错还完全不知道什么回事。
俗称“踩内存”，如果不报错，这种问题很难发现，目前唯一比较有效的debug方式就是git回滚+review代码。

如果不确定通过指针获取的结构体会不会变，那么一定要自己定义一个结构体变量，然后传入地址，再memcpy一下，不要贪图简单，直接定义个指针传入。


# ifstream读取数据总是在特定长度出错？
2018-01-02 debug时长，1个小时

描述：读取一个binary文件，文件的长度为143360字节，然而只能读取106个字节，之后总是fail，摸不着头脑

原因：读取binary文件的时候没有加入ios::bianry参数。

没有加入ios::binary参数，文件就会通过文本形式读取，读取时0x1A被认为是end of file，所以读取总在特定长度失效。



# 文件总是读写有问题？

2018-01-08 debug时长，2个小时

读出的文件不对？看看二进制参数是不是正确的，百分之八十的问题都来自这里。
C语言看"wb"和"rb"，c++看ios::binary


# 根本无错的地方crash？

2018-12-07 debug时长，1个小时

一开始在A地方crash，屏蔽了代码A之后又在B处崩溃，同样屏蔽代码B后又在C处崩溃，这个ABC处怎么看都不会崩溃，根本没有指针寻址操作。
后来一段一段代码屏蔽，才发现问题出现在ABC之前的X处，X处代码出现指针越界，但是本身运行过程中居然没问题，等到后面运行其他函数时，就爆发了，这种bug非常可怕，很容易误导人。


#  根本无错的地方crash（2）？

2019-01-28 debug时长，1小时

迷迷糊糊加了一些代码之后，然后本来无错的地方突然crash了？crash的地方和改动地方差别相差很远，而且完全没有动过，怎么会crash呢？
有一种可能一定要警醒，就是前置代码内存越界了！


# 非4整数倍尺寸图像存成bmp文件出错？
bmp标准强行要求图像宽度4字节对齐，所以一定要非4整数倍宽度一定要补齐。


# 多线程启动函数注意局部变量的使用
新线程里的某些静态数组数据，用着用着内容就被改的面目全非了，特别像踩内存。
一般新的线程都是由一个启动函数来启动的，启动函数将参数传入新线程，这时候要注意，一定不要将启动函数的局部变量传入新线程，因为启动函数是会结束的，局部变量是要被销毁的！
实际上新线程还没有启动的时候，启动函数就可能已经结束了。


# ifstream eof函数
用ifstream的eof来判断是否到达文件结尾是有坑的，即使到达了文件结尾，eof也不会马上为true，必须再读一次数据，eof才会返回true。
比较简洁的判断文件是否到结尾的方式是
\code{cpp}
ifstream ifs;
while(ifs.peek())
{
}
\endcode


# 引用不能重新改变对象。
无法根据=符号来重新改编引用对象，因为无法区分是重新引用还是对当前引用的赋值。

# 运行时出现链接错误？

```
c++ symbol lookup error undefined symbol
```

当动态链接库在动态加载时（插件系统常用），如果so文件没有定义相应的函数或者变量，就有可能报链接错误。

这和普通link时候的出错是一样的，调用者根据头文件拿到了声明，去找定义的时候找不到，只不过一个发生在运行时，另一个发生在link时。