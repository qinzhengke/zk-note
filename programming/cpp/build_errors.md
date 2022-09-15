# C++构建报错合集

## 编译篇

### No rule to make xxx.cpp 或者 xxx.so


- 如果提示无法make cpp源文件，那么一般情况是找不到在CMakeLists.txt中列出的文件，检查一下路径和文件名即可。
- 如果提示无法make xxx.so文件，特别是找不到第三方库，例如/usr/lib/xxx.so之类的，一般都是cmake缓存出现了问题，清空cmake缓存（删除CMakeCache.txt,CMakeFiles,MakeFile,cmake_install.cmake这几个文件）。

### passing ‘const xxx’ as ‘this’ argument discards qualifiers

首先确认一点，什么是“qualifiers”？
注意，通常来说，使用英文来表达的对象，含义都很明确
“qualifiers”表示类型的修饰符，它目前只包含这三种：

- const，也就是我们常说的常量修饰符
- volatile，用的比较少，表示修饰的变量在后面随时会改变，强制编译器使用变量地址数据，否则编译器会自行根据代码情况进行优化，例如基于寄存器缓存。具体使用场景自行探索。
- mutable，用的也比较少，根据cppreference的定义，用mutable定义的成员变量，那么它是可以改变的，即使其container是const的！似乎破坏了const的法则，具体使用场景自行探索。

所以说，如果错误提示出现了“qualifiers”相关的内容，那么一定就上述几个修饰符的问题。第二个和第三个修饰符很少用，那么一般情况下，都是const的问题。
“discards qualifiers”表示丢弃了修饰符，咋一看不太懂，实际上它表达的是在函数传递的过程中修饰符被丢弃了，或者函数内部出现了矛盾现象。

举个最常见的例子，下面代码展示了一个案例，试图定义一个二维向量类，通过x()和y()访问具体分量，注意这些接口是可以修改成员变量的，因为返回的是引用。
但是在企图通过add()函数将两者相加时，传入的是const的父对象，这时一个const对象调用一个可以修改对象成员变量的成员函数，即x()和y()，就是矛盾，最终导致报错。

```cpp
#include <cstdio>
struct Vec{
    int data[2];
    int & x(){
        return data[0];
    }
    int & y(){
        return data[1];
    }
};

Vec add(const Vec& a, const Vec&b){
    Vec s;
    s.x() = a.x() + b.x();  // a和b是const的，但是x()和y()接口是可以修改成员变量的。
    s.y() = a.y() + b.y();
    return s;
}

int main()
{
    Vec a,b,c;
    a.x() = 1;
    a.y() = 2;
    b.x() = 3;
    b.y() = 4;
    c = add(a,b);
    
    printf("a+b=(%d,%d)\n", c.x(),c.y());
}
```

编译报错：

```
 In function 'Vec add(const Vec&, const Vec&)':
14:17: error: passing 'const Vec' as 'this' argument of 'int& Vec::x()' discards qualifiers [-fpermissive]
14:25: error: passing 'const Vec' as 'this' argument of 'int& Vec::x()' discards qualifiers [-fpermissive]
15:17: error: passing 'const Vec' as 'this' argument of 'int& Vec::y()' discards qualifiers [-fpermissive]
15:25: error: passing 'const Vec' as 'this' argument of 'int& Vec::y()' discards qualifiers [-fpermissive]
```

实际上，线性代数代码库Eigen存在同样的“问题”，如果传入一个const修饰的Vector，那么调用x()或者其他类似接口时，同样会报这个错误。
产生这个“问题”的原因是我们试图通过一个接口既能读数据，又能写数据，并且还想传入的父对象是const修饰的，其中写数据和const父对象产生了矛盾。

解决的方法可以和Eigen一样，不要使用const修饰父对象，或者关于变量的读和写接口分开，例如getX()，setX()等等。
对于向量的设计这个案例，我选择和Eigen一样，毕竟x()和y()这种分量在数学表达中要大量重复使用，getX()和setX()这些接口，明显太啰嗦。

### overflow destination buffer.

gcc编译memcpy的时候，如果目标地址是一个固定大小的静态的数组，那么编译器会检查copy的size是否会超过这块静态数组的大小，超出了就会报出错误，不得不说编译器做得非常不错。

### Attempting to refer a deleted function 

这里的deleted表示构造函数被delete掉了。

这个问题常常出现在使用std::fstream时，fstream类的构造函数只允许传入引用而不允许拷贝。
如果将ifstream或者ofstream对象作为参数传入函数，那么传入时，一定要使用引用方式，否则就会报出这个错误。
这个报错没那么直接，编译器不会是说没有用传入，而是在函数调用的时候说调用的函数是被删除过的。

### Template with C linkage

extern C 封装起来的代码包含C++的template特性。

### request for member xxx in something not a structure or union

本身的含义是使用“.xxx”表达式时，“.”前面的的内容不是一个结构体或者联合。
具体可能出现的场景：

1.指针错误的使用了“.”来访问成员。
2.C99中使用designated initializer时，有成员没有逗号，在git解决冲突的时候常常遇到这个问题。

```cpp
struct A{int a, int b};
A x = {
 a = 10 // 解冲突时忘记了逗号，
 b = 20,
```

### extra qualification

qualification是指双冒号符“::”，而“extra qualification”则一般出现在类中函数定义的时候重复用“类名::函数名”进行时声明。

## Error: Jump to case label.

报错提示：

```bash
Error: Jump to case label, crosses initialization of xxx
```

原因，不同的case之间定义相同名称的变量。

## error: new types may not be defined in a return type

结构体、联合体定义的时候结尾忘记加“;”就会出现该报错。

## undefined reference to `vtable for xxx'

字面上的意思是找不到虚函数表，可能的原因是：

1. 带有虚函数的基类的析构函数没有设置为virtual，原因参见 \ref virtual_desctructor 。


## 'xxx' does not name a type

表示没有该符号类型没有定义，一般来原因有可能是

1. 名称拼写错误
2. 头文件未包含
3. 命名空间不正确

## error: expected class-name before ‘{’ token

字面意思是在“{”字符前面的必须是一个类名称，一般来说原因有可能是

1. 名称拼写错误
2. 头文件未包含
3. 命名空间不正确

## error: invalid new-expression of abstract class type ‘xxx’


## error: invalid operands of types ‘<unresolved overloaded function type>’ and ‘int’ to binary ‘operator<’

可能的原因是：

1. 调用Eigen库中的带模板的Matrix<T,x,x>::block，解决方法参见： \ref block_with_template 。


## error: request for member ‘x’ in ‘yyy’, which is of non-class type ‘zzz’



## error: lvalue required as left operand of assignment

字面含义是等号的左边必须是左值，不能是右值，一般很明显的右值（例如常量，返回值）我们是会避免的，但是有时候就不是很明显。

1. Eigen中使用auto定义变量，发现得到的变量竟然是右值，而使用类名定义，得到的才是左值，具体参见 \ref eigen_auto

## cannot bind non-const lvalue reference of type ‘int&’ to an rvalue of type ‘int’

试图将非常量左值引用绑定右值，显然是错误的，如下代码所示。稍微白话解释一下，“bind A to B”中的“A”是引用，“B”才是变量。

```cpp
#include <cstdio>
void func_add(int a, int b, int& c) {
    c = a+b;
}
int main() {
    int x=1,y=2;
    float z;
    func_add(x,y,(int)z); //编译报错
}
```

c和c++中存在所谓的临时变量，想到的就三种：1.运算表达式，2.类型转换，3.函数返回值。
这三种临时变量传入以非常量引用为形参的函数时，是非法的，例如：

临时变量无法作为非常量引用的原因是很显然的，作为非常量引用，用户肯定是希望改动这个输入参数，然而实际上真正发生改动的是临时变量。
如果编译不报错，那么可能会给使用者造成很大的误解，使用者很可能要debug很久才能发现这个问题。于是编译器就负责的直接给出编译错误。

实际使用的一个案例就是，使用指针的引用作为形参，通常我们想通过一个函数修改指针的指向，可以使用二级指针，也可以使用指针的引用，既然c++提倡使用引用作为输出，我这里也使用指针的引用作为输出。
使用指针的引用有一个问题就是，通常我们传入指针的时候需要做类型转换，而类型转换就是上文说道的第二种临时变量，编译器会报错。正确的使用方法就是调用时提前把指针手动转换好，再传入函数。



## error: ‘setX’ was not declared in this scope, and no declarations were found by argument-dependent lookup at the point of instantiation [-fpermissive]

字面含义是没有声明对应的符号。

1. 派生类构造函数调用基类的成员函数，参考 \ref  base_member_in_constructor

## capture of non-variable <name>

1. 试图用错误的方法捕获类成员变量，例如“[成员变量]”或者“[&成员变量]”，应该使用“[=]”来（可修改地）捕获成员变量，使用“[=m]”来（不可修改地）捕获成员变量。

## error: new types may not be defined in a return type

结构体、联合体定义的时候结尾忘记加“;”就会出现该报错，这里不得不吐槽GCC，这种报错提示，真的无法联想到正确原因，什么叫“新类型不能定义在返回类型中？”，这提示等于没给。

## extra qualification

qualification是指双冒号符“::”，而“extra qualification”则一般出现在类中函数定义的时候重复用“类名::函数名”进行时声明，如下代码所示：

```cpp
struct A{
    A::func(){

    }
};
```

### 系统limit文件报错？
有的时候系统源文件报错会让人摸不着头脑，开发者根本不知道哪里出错。这里列举一个遇到的问题。

有可能是定义了和已有库重名函数或者变量，例如有一个案例，不知道谁多此一举，定义了min和max函数，虽然自定义的max函数有ifndef防止重复定义，
但是已有库的函数不一定会这样定义啊，所以如果先编译了自定义的max，然后在编译已有库max之间如果有其他系统函数调用了这个max，就会直接报错了。
所以不要随便定义和已有库重名的东西。


### 添加标准头文件就会报错？
遇到一个很奇怪的问题，#include一个标准库头文件后，系统的其他模块就会报错，具体来说，我在main.cpp里include<local>，然后编译时，limit文件就会报max函数使用参数给错。
虽然问题很怪，但是还是找出来了，实际上，这是一个**触发式**问题，加入的代码本身没有问题，例如添加一句include标准库，但是添加进去之后确实出了问题。

实际上，include <local>触发了工程代码里面的一个隐藏问题，也就是上一篇所说的自身的工程定义了一个max宏，而且形参形式和标准库的max不一样，但是由于原来的工程真的再没有其他标准库里的函数调用max和min，所以就一直没有报错！但是添加<local>之后，local使用了max宏，但是本来是期望按照标准库的用法使用的，但是由于先编译了本地的max，所以local里的函数使用max时就报错。
 所以以后如果遇到了这种莫名其妙，include标准头文件都能报错的问题，就有可能是它触发了系统里原有的问题！
 

 ### namespace语法错误？
 namespace语法很简单，但是为什么会出错呢？
 有一种可能，就是在c文件或者c文件引用的头文件里定义或者使用的namespace，C语言没有明明空间，当然不支持。
 只不过编译器不会提示C文件不能定义namespace，只是会提示语法错误。
 

 ### memcpy_overflow memcpy报出 will always overflow destination buffer.
 gcc编译memcpy的时候，如果目标地址是一个固定大小的静态的数组，那么编译器会检查copy的size是否会超过这块静态数组的大小，超出了就会报出错误，不得不说编译器做得非常不错。



 ### enum_plusplus 枚举变量++操作不支持？
 c++中，枚举变量进行++操作会出现编译错误！
 然而C语言中，枚举变量++是允许的！
 C++已经不兼容C了？
 

 ### _Bool类型不能用？
 \_Bool类型是C99引入的专属于C语言的布尔类型，如果要使用，必须包含stdbool.h头文件。


#### C语言和C++混用的常见问题？

1. c模块依赖包含c++特性的c++模块，这将导致编译不过，产生的问题例如namespace非法，因为C语言根本没有namespace。

2. extern C 封装起来的代码包含C++特性， 这将导致编译不过，产生的报错例如“template with C linkage”。


### ref_deleted_func 编译错误: Attempting to refer a deleted function 

这个问题常常出现在使用std::fstream时，如果将ifstream或者ofstream对象作为参数传入函数，那么传入时，一定要使用引用方式，否则就会报出这个错误。这个报错没那么直接，编译器不会是说没有用传入，而是在函数调用的时候说调用的函数是被删除过的。


### TCHAR问题？
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


### 函数明明存在却报unresolved symbol！

在VS下，函数明明存在为什么还是报出unresolved symbol错误?可能是字符集问题！特别是将不同操作系统下的文件进行混用的时候！通通，通通，通通改成多字节字符。

这个问题怎么google？一查unresolved symbol全都是确实没有添加函数定义的。

回答：搜索关键词：function do exist unresolved symbol

在vs下如果是main.cpp、a.h和a.c这种组合一定会出问题，可以试一试，注意，这里a.c是C语言写的，用于板子上的程序，不能有c++的特性，但是main.cpp可以，因为会用到第三方库进行显示，例如用Opencv读取、显示和保存图片。
解决方法，在project property的C++/Advanced中，Complie as一项从default改成as c++。
在QtCreator下也是一样的，需要设置编译方式为C++


### 头文件不要定义变量！
头文件不要定义变量，否则当多个c/cpp文件引用同一个头文件的时候，会出现重复定义问题！
头文件如果一定要定义变量，就在函数里定义，然后通过返回的形式使用。


### 在头文件中定义函数一定要使用inline！
header only写法一定要注意头文件中定义函数的实现的时候，一定要加入inline修饰符，或者使用类的静态成员来定义实现。


### 临时变量无法引用
c和c++中存在所谓的临时变量，想到的就三种：1.运算表达式，2.类型转换，3.函数返回值。
这三种临时变量传入以非常量引用为形参的函数时，是非法的，例如：
```cpp
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
```
临时变量无法作为非常量引用的原因是很显然的，作为非常量引用，用户肯定是希望改动这个输入参数，然而实际上真正发生改动的是临时变量。
如果编译不报错，那么可能会给使用者造成很大的误解，使用者很可能要debug很久才能发现这个问题。于是编译器就负责的直接给出编译错误。

实际使用的一个案例就是，使用指针的引用作为形参，通常我们想通过一个函数修改指针的指向，可以使用二级指针，也可以使用指针的引用，既然c++提倡使用引用作为输出，我这里也使用指针的引用作为输出。

使用指针的引用有一个问题就是，通常我们传入指针的时候需要做类型转换，而类型转换就是上文说道的第二种临时变量，编译器会报错。正确的使用方法就是调用时提前把指针手动转换好，再传入函数。

### request for member xxx in something not a structure or union

本身的含义是使用“.xxx”表达式时，“.”前面的的内容不是一个结构体或者联合。
具体可能出现的场景：

1.指针错误的使用了“.”来访问成员。
2.C99中使用designated initializer时，有成员没有逗号，在git解决冲突的时候常常遇到这个问题。

```
struct A{int a, int b};
A x = {
 a = 10 // 解冲突时忘记了逗号，
 b = 20,
```


### passing ... as 'this' argument ...  discards qualifiers

问题：问题的全称是
```bash
error: passing ‘const Base’ as ‘this’ argument of ‘virtual void Base::test()’ discards qualifiers [-fpermissive]
```

问题的原因是函数传入的const类型的对象a，但是函数内部调用了a的非const类型成员函数，有可能会导致类内部成员被修改，编译器不允许。

解决方法：

1. 如果成员函数没有修改任何对象变量，则可以用const修饰成员函数，例如

```cpp
class A{
    void a() const{ 
        // Do something
    }
}
```

2. 如果成员函数确实修改了对象变量，那么成员函数不能定义为const类型，则在传入a的时候，就不能用const修饰，因为你确实要修改这个对象。


### 类成员函数声明和定义处同时使用static修饰

这样的做法会编译报错，具体原因待了解。
正确的做法是只在声名处用static修饰。


### 类成员函数声明和定义同时使用默认参数

这样的做法会导致编译报错，具体原因待了解。
正确的做法是只在声明处使用默认参数。

## dynamic_pointer_cast模板类要正确

```cpp
class Base {
public:
    Base (){};
protected:
    inline virtual int getInt () { return 0; }
};

class Derived : public Base {
protected:
    inline int getInt ( ) override { return 1; }

};

typedef std::shared_ptr<Base> spBase;
typedef std::shared_ptr<Derived> spDerived;

```

这时候如果这样操作，则会报错

```cpp
spDerived derived = static_pointer_cast < spDerived > ( base );
```

这里要注意的是，模板参数是派生类，而不是派生类的shared_ptr，千万别搞错了！

这样才是正确的

```cpp
spDerived derived = static_pointer_cast <Derived> ( base );
```

## 链接篇

### undefined reference to `xxx'

报错的字面含义是链接时找不到对应的符号，符号有可能是变量或者函数，有以下可能的原因：

1. 被调用的符号声明了，但是没有定义，或者出现拼写错误导致声明和定义名称不相同。
2. 对于类的static成员，需要在class体外部显示定义，参考 \ref cpp_class_static_member 
3. 调用库文件版本不正确，一般来说，如果只安装了一个版本的库，那么其头文件和库文件是正确匹配的。但是当安装多个版本的库时，有可能出现包含A版本的头文件，却链接B版本的库文件，而B的库没有A版本对应的实现。编译过程是没问题的，编译只需要头文件，但是链接的时候就会发现没有相应的定义，所以链接的时候就会报错。

### error LNK2019: unresolved external symbol "xxx"

这个问题和上面的“undefined reference”是一样的，只不过是换成了VC编译器的提示，有以下可能的原因：

1. 相关源文件有没有被加到工程中。
2. C模块调用C++模块时，没有使用“EXTERN C”封装。
3. 一些开发环境（例如QtCreator）的字符编码不正确。
4. Windows下，调用预编译好的库时，平台、版本、模式不对应，平台包括x86和x64，版本包括vc08、vc10、vc12、vc14，模式包括debug、release。
5. 开发中途改变了库的版本，编译缓存文件没更新，试着显式地删除编译文件，重新构建。
6. VisualStudio开发环境，字符集没有设置成多字节字符。
7. VisualStudio开发环境，C和C++混合编译，需要在“project property的C++/Advanced”，将“Complie as”选项从“default”改成“as c++”。

另外，这个问题怎么google？一查unresolved symbol全都是因为原因1，无法直接查到正确信息。

回答：搜索关键词：function do exist unresolved symbol

### multiple definition of `xxx'

报错的字面含义是出现了多个相同的符号，明明只定义了1处变量，但是为何多个obj文件会重复定义？

1. 确实出现了同名的函数或者变量的定义。
2. 确认没有同名符号，但是变量或者函数定义在了h文件中，并且没有使用static修饰，编译多个源文件，这些源文件把头文件包含后，出现了多重定义的错误。

### libpthread.so.0: error adding symbols: DSO missing from command line

在target_link_library中添加pthread即可

```bash
target_link_library(exe
    pthread
    )
```

### error 'thread' is not a member of 'boost'

CMakeLists.txt中添加

```bash

target_link_library(exe
        boost_thread
    )

```

源代码中添加

```cpp
#include <boost/thread.hpp>
```

### undefined reference to `类名::count'

static类成员变量在类中只是进行了声明，没有定义，而普通成员变量在生成定义对像的时候进行了定义。
所以类成员变量需要而外定义。

```cpp
#include <cstdio>
class A{
    public:
    A(){}
    static int count;
};

int A::count = 0;   //必须在外面进行定义，否则编译器会提示未定义

int main(void){
    A a;
    printf("%d\n",a.count);
}
```

### error 'thread' is not a member of 'boost'

如果仍然使用boost提供的thread库，那么有可能呢遇到这个问题，一般来说是cmake配置不正确。

CMakeLists.txt中添加

```bash

target_link_library(exe
        boost_thread
    )

```

源代码中添加

```cpp
#include <boost/thread.hpp>
```


###  libboost_system.so.1.xx.0: error adding symbols: DSO missing from command line

缺少filesystem对应的库文件，在CMakeLists.txt中添加

```
target_link_libraries(my_target
    boost_system
)
```

### 动态库链接静态库报错

完整的报错信息为：

```bash
relocation R_X86_64_PC32 against symbol `_ZGVZN4pcpp8LoggerPP11getInstanceEvE8instance' can not be used when making a shared object; recompile with -fPIC
```

这个问题原因一般是动态库链接了静态库导致的，一般来说动态库是不会链接静态库的，因为动态库遵循引用的规范，不会把静态库塞到自身之中。
如果是引用别人的库，那要改成引用shared库，如果是引用自己的库，那么在add_library中增加“SHARED”修饰。

### reference to symbol 'pthread_createhttps://github.com/qinzhengke/zk-note/blob/$1GLIBC_2.2.5'

使用pthread库，需要编译配置中显示设置。

```cmake
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

```


### DSO missing from command line

链接过程中，试图通过中间静态库链接进行传递，对于版本<2.2的ld，是可以的，但是对于>=2.2版本的ld，就是不行的。

什么是链接传递，举下面的例子：

1. 一个shared libA，定义了foo()函数
2. 一个静态库libB，显示链接了libA，
3. 另一个可执行文件binC，显示地链接libB

那么问：可执行文件binC能否调用foo()函数。

### 多个c文件共享一个头文件，这与头文件只展开一次是否矛盾？

写代码的时候发现一个“问题”，我们一般用\#ifdef包裹头文件，保证头文件只展开一次，否则会出现重复定义。
那么多个c文件包含同一个头文件时，后面的c文件是否就无法获取h文件里的内容了呢？
当然，通过实际编程经验，我们都知道答案是否定的，但是怎么解释呢？

这个问题提出来说明提问人对于编译-链接过程不够了解。

【回答】不矛盾，头文件仅展开一次是对于一个c文件的编译过程来说的，并不是整个工程的构建，如果一个c文件根据间接多次包含同一个头文件，那么头文件确实只展开一次。
但是对于另外一个c文件，编译是一个崭新的过程，和上一个编译的c文件就无关了，所以两者并不影响。



## 运行篇

### bad_alloc terminate called after throwing an instance of 'std::bad_alloc'

