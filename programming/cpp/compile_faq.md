C++构建常见问题{#cpp_compile_faq}
================================

<hr> \section static相关问题

\subsection static类成员变量未定义

static类成员变量在类中只是进行了声明，没有定义，而普通成员变量在生成定义对像的时候进行了定义。
所以类成员变量需要而外定义。

\code{.cpp}
class A{
    public:
    A(){}
    static int count;
}

int A::count = 0;   //必须在外面进行定义，否则编译器会提示未定义
\endcode

<hr>
\section 链接的时候重复定义
明明只定义了1处变量，但是为何多个obj文件会重复定义？
可能1：变量定义在了h文件中，并且没有使用static修饰，

<hr>
\section 系统limit文件报错？
有的时候系统源文件报错会让人摸不着头脑，开发者根本不知道哪里出错。这里列举一个遇到的问题。

有可能是定义了和已有库重名函数或者变量，例如有一个案例，不知道谁多此一举，定义了min和max函数，虽然自定义的max函数有ifndef防止重复定义，
但是已有库的函数不一定会这样定义啊，所以如果先编译了自定义的max，然后在编译已有库max之间如果有其他系统函数调用了这个max，就会直接报错了。
所以不要随便定义和已有库重名的东西。

<hr>
\section 添加标准头文件就会报错？
遇到一个很奇怪的问题，#include一个标准库头文件后，系统的其他模块就会报错，具体来说，我在main.cpp里include<local>，然后编译时，limit文件就会报max函数使用参数给错。
虽然问题很怪，但是还是找出来了，实际上，这是一个**触发式**问题，加入的代码本身没有问题，例如添加一句include标准库，但是添加进去之后确实出了问题。

实际上，include <local>触发了工程代码里面的一个隐藏问题，也就是上一篇所说的自身的工程定义了一个max宏，而且形参形式和标准库的max不一样，但是由于原来的工程真的再没有其他标准库里的函数调用max和min，所以就一直没有报错！但是添加<local>之后，local使用了max宏，但是本来是期望按照标准库的用法使用的，但是由于先编译了本地的max，所以local里的函数使用max时就报错。
 所以以后如果遇到了这种莫名其妙，include标准头文件都能报错的问题，就有可能是它触发了系统里原有的问题！
 
<hr>
 \section namespace语法错误？
 namespace语法很简单，但是为什么会出错呢？
 有一种可能，就是在c文件或者c文件引用的头文件里定义或者使用的namespace，C语言没有明明空间，当然不支持。
 只不过编译器不会提示C文件不能定义namespace，只是会提示语法错误。
 
<hr>
 \section memcpy_overflow memcpy报出 will always overflow destination buffer.
 gcc编译memcpy的时候，如果目标地址是一个固定大小的静态的数组，那么编译器会检查copy的size是否会超过这块静态数组的大小，超出了就会报出错误，不得不说编译器做得非常不错。


<hr>
 \section enum_plusplus 枚举变量++操作不支持？
 c++中，枚举变量进行++操作会出现编译错误！
 然而C语言中，枚举变量++是允许的！
 C++已经不兼容C了？
 
<hr>
 \section const对象不能调用非const方法？

报错：passing 'const Type' as 'this' argument discard qulifiers
 如果const对象调用了非const成员函数，就会报这个错误，
 const对象只能调用const方法，const修饰的成员函数会保证不会修改成员变量，所以const对象才能因此保证不被修改。
 
<hr>
 \section _Bool类型不能用？
 \_Bool类型是C99引入的专属于C语言的布尔类型，如果要使用，必须包含stdbool.h头文件。

<hr>
\section c_with_cpp C语言和C++混用的常见问题？

1. c模块依赖包含c++特性的c++模块，这将导致编译不过，产生的问题例如namespace非法，因为C语言根本没有namespace。

2. extern C 封装起来的代码包含C++特性， 这将导致编译不过，产生的报错例如“template with C linkage”。

<hr>
\section ref_deleted_func 编译错误: Attempting to refer a deleted function 

这个问题常常出现在使用std::fstream时，如果将ifstream或者ofstream对象作为参数传入函数，那么传入时，一定要使用引用方式，否则就会报出这个错误。这个报错没那么直接，编译器不会是说没有用传入，而是在函数调用的时候说调用的函数是被删除过的。

<hr>
\section cpp_TCHAR TCHAR问题？
tinydir使用TCHAR作为基本的字符串输入，在Linux下，TCHAR默认是char，但是在Windows下，THCAR默认是wchar_t。
如果直接使用char\*作为tinydir的输入，就会出错，其实TCHAR由一个宏来控制，就是UNICODE，VS环境和qtcreator环境都能关闭这个宏，关闭之后TCHAR就变成了char，
VS的方法是项目属性页面设置字符集为多字节，而不是Unicode。
qtcreator的方法是在pro文件中设置 DEFINES -= UNICODE。


或者将wstring转换成string
\code{cpp}
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
\endcode

对应的tinydir_open，要使用wchar_t作为输入，如果输入常量字符串，可以这样写L"abcde"。
codecvt头文件在gcc5才引入，如果使用gcc4，是无法通过编译的。

<hr>
\section 函数明明存在却报unresolved symbol！

在VS下，函数明明存在为什么还是报出unresolved symbol错误?可能是字符集问题！特别是将不同操作系统下的文件进行混用的时候！通通，通通，通通改成多字节字符。

这个问题怎么google？一查unresolved symbol全都是确实没有添加函数定义的。

回答：搜索关键词：function do exist unresolved symbol

在vs下如果是main.cpp、a.h和a.c这种组合一定会出问题，可以试一试，注意，这里a.c是C语言写的，用于板子上的程序，不能有c++的特性，但是main.cpp可以，因为会用到第三方库进行显示，例如用Opencv读取、显示和保存图片。
解决方法，在project property的C++/Advanced中，Complie as一项从default改成as c++。
在QtCreator下也是一样的，需要设置编译方式为C++

<hr>
\section 头文件不要定义变量！
头文件不要定义变量，否则当多个c/cpp文件引用同一个头文件的时候，会出现重复定义问题！
头文件如果一定要定义变量，就在函数里定义，然后通过返回的形式使用。

<hr>
\section 在头文件中定义函数一定要使用inline！
header only写法一定要注意头文件中定义函数的实现的时候，一定要加入inline修饰符，或者使用类的静态成员来定义实现。

<hr>
\section 临时变量无法引用
c和c++中存在所谓的临时变量，想到的就三种：1.运算表达式，2.类型转换，3.函数返回值。
这三种临时变量传入以非常量引用为形参的函数时，是非法的，例如：
\code{cpp}
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
\endcode
临时变量无法作为非常量引用的原因是很显然的，作为非常量引用，用户肯定是希望改动这个输入参数，然而实际上真正发生改动的是临时变量。
如果编译不报错，那么可能会给使用者造成很大的误解，使用者很可能要debug很久才能发现这个问题。于是编译器就负责的直接给出编译错误。

实际使用的一个案例就是，使用指针的引用作为形参，通常我们想通过一个函数修改指针的指向，可以使用二级指针，也可以使用指针的引用，既然c++提倡使用引用作为输出，我这里也使用指针的引用作为输出。

使用指针的引用有一个问题就是，通常我们传入指针的时候需要做类型转换，而类型转换就是上文说道的第二种临时变量，编译器会报错。正确的使用方法就是调用时提前把指针手动转换好，再传入函数。

\section request_for_member request for member xxx in something not a structure or union

本身的含义是使用“.xxx”表达式时，“.”前面的的内容不是一个结构体或者联合。
具体可能出现的场景：

1.指针错误的使用了“.”来访问成员。
2.C99中使用designated initializer时，有成员没有逗号，在git解决冲突的时候常常遇到这个问题。

\endcode
struct A{int a, int b};
A x = {
 a = 10 // 解冲突时忘记了逗号，
 b = 20,
\endcode

<hr>
\section const_issue passing ... as 'this' argument ...  discards qualifiers

问题：问题的全称是
\code{bash}
error: passing ‘const Base’ as ‘this’ argument of ‘virtual void Base::test()’ discards qualifiers [-fpermissive]
\endcode

问题的原因是函数传入的const类型的对象a，但是函数内部调用了a的非const类型成员函数，有可能会导致类内部成员被修改，编译器不允许。

解决方法：

1. 如果成员函数没有修改任何对象变量，则可以用const修饰成员函数，例如

\code{cpp}
class A{
    void a() const{ 
        // Do something
    }
}
\endcode

2. 如果成员函数确实修改了对象变量，那么成员函数不能定义为const类型，则在传入a的时候，就不能用const修饰，因为你确实要修改这个对象。

<hr>
\section 类成员函数声明和定义处同时使用static修饰

这样的做法会编译报错，具体原因待了解。
正确的做法是只在声名处用static修饰。

<hr>
\section 类成员函数声明和定义同时使用默认参数

这样的做法会导致编译报错，具体原因待了解。
正确的做法是只在声明处使用默认参数。

<hr>
\section extra_qualification extra qualification

qualification是指双冒号符“::”，而“extra qualification”则一般出现在类中函数定义的时候重复用“类名::函数名”进行时声明。


<hr>
\section cpp_compile_issue10 passing const as ‘this’ argument discards qualifiers

尝试在const引用模式下调用非const函数，编译器为了阻止传入引用对象的改变。

\code{.cpp}
#include <stdint.h>
#include <stdio.h>

class A{
public:
    A(){}
    void func(){
        printf("A.func()\n");
        x++;
    }
    void func_const() const {
        printf("A.func_const()\n");
    }
    void func_const2() const;
private:
    int x = 0;
};

void A::func_const2() const {
    printf("A.func_const2()\n");
}

void top_func(const A& a){
     a.func();  //  编译报错
    a.func_const(); // 编译成功
    a.func_const2(); // 编译成功
}

int main()
{
    A a;
    top_func(a);
}

\endcode

编译结果如下：

\code{.sh}
 In function 'void top_func(const A&)':
19:12: error: passing 'const A' as 'this' argument of 'void A::func()' discards qualifiers [-fpermissive]
\endcode

这里顺便提一下，如果成员函数声明和定义分离，那么声明和定义处都需要在"()"后面加入const，这点和static修饰符不一样，static修饰符只能在声明的地方使用。

<hr>
\subsection unresolved_external_symbol unresolved external symbol？
如果这个symbol在源文件里：
- 看看这个源文件有没有被加到工程中。
- symbol在源文件中，并且已经添加到了工程，还会报这个错？
- 是否使用了看看是不是调用了.c文件，记得用EXTERN C封装，即在c文件函数声明的地方使用
- 都是cpp文件或者c文件，或者已经用EXTERN C封装了，还会出错？看看是不是不同源文件字符编码的问题。
- 库已经添加到工程，但是仍然报这个错？那就将库的版本和当前编译器版本保持一致，包括平台（x86或者x64）和版本（vc08，vc10，vc12，vc14）以及是否调试（debug和release）。
- 以上条件都满足，但在qtcreator中刚添加的函数还是报这个错？那就试着显式地qmake一下，然后再rebuild。