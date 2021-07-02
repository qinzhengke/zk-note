C++构建报错合集{#cpp_build_errors}
================================

\section 编译篇

\subsection no_rule_to_make No rule to make xxx.cpp 或者 xxx.so

- 如果提示无法make cpp源文件，那么一般情况是找不到在CMakeLists.txt中列出的文件，检查一下路径和文件名即可。
- 如果提示无法make xxx.so文件，特别是找不到第三方库，例如/usr/lib/xxx.so之类的，一般都是cmake缓存出现了问题，清空cmake缓存（删除CMakeCache.txt,CMakeFiles,MakeFile,cmake_install.cmake这几个文件）。

\subsection no_matching_function error: no matching function for call to xxx

字面意思是找不到匹配的函数（声明），可能的原因有

- 明显的函数调用方式出错，例如参数类型和数量不匹配
- 一种不太明显的调用出错，函数形参只接受const引用，但是调用出提供了右值，例如new出来的，后者函数返回值。

\subsection discards_qualifiers passing ‘const xxx’ as ‘this’ argument discards qualifiers

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

\code{cpp}
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

\endcode

编译报错：
\code
 In function 'Vec add(const Vec&, const Vec&)':
14:17: error: passing 'const Vec' as 'this' argument of 'int& Vec::x()' discards qualifiers [-fpermissive]
14:25: error: passing 'const Vec' as 'this' argument of 'int& Vec::x()' discards qualifiers [-fpermissive]
15:17: error: passing 'const Vec' as 'this' argument of 'int& Vec::y()' discards qualifiers [-fpermissive]
15:25: error: passing 'const Vec' as 'this' argument of 'int& Vec::y()' discards qualifiers [-fpermissive]
\endcode

实际上，线性代数代码库Eigen存在同样的“问题”，如果传入一个const修饰的Vector，那么调用x()或者其他类似接口时，同样会报这个错误。
产生这个“问题”的原因是我们试图通过一个接口既能读数据，又能写数据，并且还想传入的父对象是const修饰的，其中写数据和const父对象产生了矛盾。

解决的方法可以和Eigen一样，不要使用const修饰父对象，或者关于变量的读和写接口分开，例如getX()，setX()等等。
对于向量的设计这个案例，我选择和Eigen一样，毕竟x()和y()这种分量在数学表达中要大量重复使用，getX()和setX()这些接口，明显太啰嗦。

\subsection memcpy_overflow will always overflow destination buffer.

gcc编译memcpy的时候，如果目标地址是一个固定大小的静态的数组，那么编译器会检查copy的size是否会超过这块静态数组的大小，超出了就会报出错误，不得不说编译器做得非常不错。

\subsection ref_deleted_func Attempting to refer a deleted function 

这里的deleted表示构造函数被delete掉了。

这个问题常常出现在使用std::fstream时，fstream类的构造函数只允许传入引用而不允许拷贝。
如果将ifstream或者ofstream对象作为参数传入函数，那么传入时，一定要使用引用方式，否则就会报出这个错误。
这个报错没那么直接，编译器不会是说没有用传入，而是在函数调用的时候说调用的函数是被删除过的。

\subsection template_c_linkage Template with C linkage

extern C 封装起来的代码包含C++的template特性。

\subsection request_for_member request for member xxx in something not a structure or union

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

\subsection extra_qualification extra qualification

qualification是指双冒号符“::”，而“extra qualification”则一般出现在类中函数定义的时候重复用“类名::函数名”进行时声明。

\subsection error_jump_to_case_label Error: Jump to case label.

报错提示：

\code{bash}
Error: Jump to case label, crosses initialization of xxx
\endcode

原因，不同的case之间定义相同名称的变量。

\subsection cpp_comp_issue_01 error: new types may not be defined in a return type

结构体、联合体定义的时候结尾忘记加“;”就会出现该报错。

\subsection undefined_vtable undefined reference to `vtable for xxx'

字面上的意思是找不到虚函数表，可能的原因是：

1. 带有虚函数的基类的析构函数没有设置为virtual，原因参见 \ref virtual_desctructor 。


\subsection does_not_name_type 'xxx' does not name a type

表示没有该符号类型没有定义，一般来原因有可能是

1. 名称拼写错误
2. 头文件未包含
3. 命名空间不正确

\subsection expect_class_name error: expected class-name before ‘{’ token

字面意思是在“{”字符前面的必须是一个类名称，一般来说原因有可能是

1. 名称拼写错误
2. 头文件未包含
3. 命名空间不正确

\subsection invalid_new_abstract error: invalid new-expression of abstract class type ‘xxx’


\subsection invalid_operand error: invalid operands of types ‘<unresolved overloaded function type>’ and ‘int’ to binary ‘operator<’

可能的原因是：

1. 调用Eigen库中的带模板的Matrix<T,x,x>::block，解决方法参见： \ref block_with_template 。


\subsection non_class_type error: request for member ‘x’ in ‘yyy’, which is of non-class type ‘zzz’



\subsection lvalue_operand error: lvalue required as left operand of assignment

字面含义是等号的左边必须是左值，不能是右值，一般很明显的右值（例如常量，返回值）我们是会避免的，但是有时候就不是很明显。

1. Eigen中使用auto定义变量，发现得到的变量竟然是右值，而使用类名定义，得到的才是左值，具体参见 \ref eigen_auto

\subsection lvalue_bind cannot bind non-const lvalue reference of type ‘int&’ to an rvalue of type ‘int’

试图将非常量左值引用绑定右值，显然是错误的，如下代码所示。稍微白话解释一下，“bind A to B”中的“A”是引用，“B”才是变量。

\code{cpp}
#include <cstdio>
void func_add(int a, int b, int& c) {
    c = a+b;
}
int main() {
    int x=1,y=2;
    float z;
    func_add(x,y,(int)z); //编译报错
}
\endcode

c和c++中存在所谓的临时变量，想到的就三种：1.运算表达式，2.类型转换，3.函数返回值。
这三种临时变量传入以非常量引用为形参的函数时，是非法的，例如：

临时变量无法作为非常量引用的原因是很显然的，作为非常量引用，用户肯定是希望改动这个输入参数，然而实际上真正发生改动的是临时变量。
如果编译不报错，那么可能会给使用者造成很大的误解，使用者很可能要debug很久才能发现这个问题。于是编译器就负责的直接给出编译错误。

实际使用的一个案例就是，使用指针的引用作为形参，通常我们想通过一个函数修改指针的指向，可以使用二级指针，也可以使用指针的引用，既然c++提倡使用引用作为输出，我这里也使用指针的引用作为输出。
使用指针的引用有一个问题就是，通常我们传入指针的时候需要做类型转换，而类型转换就是上文说道的第二种临时变量，编译器会报错。正确的使用方法就是调用时提前把指针手动转换好，再传入函数。



\subsection no_declar error: ‘setX’ was not declared in this scope, and no declarations were found by argument-dependent lookup at the point of instantiation [-fpermissive]

字面含义是没有声明对应的符号。

1. 派生类构造函数调用基类的成员函数，参考 \ref  base_member_in_constructor

\subsection cap_non_var capture of non-variable <name>

1. 试图用错误的方法捕获类成员变量，例如“[成员变量]”或者“[&成员变量]”，应该使用“[=]”来（可修改地）捕获成员变量，使用“[=m]”来（不可修改地）捕获成员变量。

\subsection cpp_comp_issue_01 error: new types may not be defined in a return type

结构体、联合体定义的时候结尾忘记加“;”就会出现该报错，这里不得不吐槽GCC，这种报错提示，真的无法联想到正确原因，什么叫“新类型不能定义在返回类型中？”，这提示等于没给。

\subsection extra_qualification extra qualification

qualification是指双冒号符“::”，而“extra qualification”则一般出现在类中函数定义的时候重复用“类名::函数名”进行时声明，如下代码所示：

\code{cpp}
struct A{
    A::func(){

    }
};
\endcode

\subsection cannot_dynamic_cast cannot dynamic_cast xxx source type is not polymorphic

类型不是polymorphic，即类并非是多态的，一般来说问题是没有把基类定义成虚类。

\code{cpp}
struct A {};

struct B : A {};

int main()
{
    A* a = new B();

    B* b = dynamic_cast<B*>(a);
}
\endcode

解决办法很简单，给基类加一个虚的析构函数即可。

\subsection  this_not_captured error: ‘this’ was not captured for this lambda function

在类成员函数中使用lamda表达式，而lambda表达式中包含类内的其他成员函数，就需要先捕获this指针。

\code{cpp}

#include <cstdio>

struct A{
    void funcA(){
        auto lambda = [this]{   // 捕获this指针，获取调用成员函数能力
            funcB();
        };
        lambda();
    }
    void funcB(){
        printf("funcB()\n");
    }
};

int main()
{
    A a;
    a.funcA();
}
\endcode

\subsection use_deleted_function use of deleted function ‘ha::driveable::ViewPort::ViewPort(ha::driveable::ViewPort&&)’

使用右值作为实参，那么系统会优先调用移动构造函数。但是如果移动构造函数被删除了，那么就会出现这个错误。

第一种是显式删除，如下代码所示，一般人都能看出来。

\code{cpp}
#include <cstdio>
#include <vector>
#include <array>
using namespace std;

struct A{
    A(array<int,3> x){
        printf("%d\n",x[0]);
    }
    A(A &&) = delete;   // use of deleted function
};

int main()
{
    vector<A> as;
    as.push_back(A({0,1,2}));
}
\endcode

但是除了显式删除，造成困扰的更多是隐式的删除，参见 \ref move_constructor。

如下示例，虽然A本身没有把移动构造函数给delete掉，但是它的非静态成员B，把移动构造函数给delete了，所以A也是无法被移动构造的。
通常来说，B有可能是别人代码提供的类，不能修改。

解决方法是，显示定义拷贝构造函数，因为常量引用是万能引用，可以接受右值作为输入。
\code{cpp}
#include <cstdio>
#include <vector>
#include <array>
using namespace std;

struct B{
    B(){}
    B(B&&) = delete;
};

struct A{
    B b;
    A(array<int,3> x){
        printf("%d\n",x[0]);
    }

    A(const A& a){} // 加入拷贝构造函数就可以了。
};

\subsection cpp_incomplete_type has incomplete type and cannot be defined

另一种报错形式：
```
invalid use of incomplete type ‘xxx’
```

incomplete type是指只声明，但是没有具体定义的类，在C++中，可以先声明一个类但是不具体定义，其指针是完全可以使用的。
但是一旦设计到指针的解引用操作，就必须知道类型的具体定义，否则会提示incomplete type。

int main()
{
    vector<A> as;   // 不报错
    as.push_back(A({0,1,2}));   //报错
}

\endcode

#include <cstdio>

class A;

int main()
{
    A *a;   
    A b;
}

class A{
    void func(){
        printf("func()\n");
    }
};




\section 链接篇

\subsection cpp_undefined_reference undefined reference to `xxx'

报错的字面含义是链接时找不到对应的符号，符号有可能是变量或者函数，有以下可能的原因：

1. 被调用的符号声明了，但是没有定义，或者出现拼写错误导致声明和定义名称不相同。
2. 对于类的static成员，需要在class体外部显示定义，参考 \ref cpp_class_static_member 
3. 调用库文件版本不正确，一般来说，如果只安装了一个版本的库，那么其头文件和库文件是正确匹配的。但是当安装多个版本的库时，有可能出现包含A版本的头文件，却链接B版本的库文件，而B的库没有A版本对应的实现。编译过程是没问题的，编译只需要头文件，但是链接的时候就会发现没有相应的定义，所以链接的时候就会报错。

\subsection unresolved_symbol error LNK2019: unresolved external symbol "xxx"

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

\subsection cpp_multiple_def multiple definition of `xxx'

报错的字面含义是出现了多个相同的符号，明明只定义了1处变量，但是为何多个obj文件会重复定义？

1. 确实出现了同名的函数或者变量的定义。
2. 确认没有同名符号，但是变量或者函数定义在了h文件中，并且没有使用static修饰，编译多个源文件，这些源文件把头文件包含后，出现了多重定义的错误。

\subsection pthread_dso_missing libpthread.so.0: error adding symbols: DSO missing from command line

在target_link_library中添加pthread即可

\code{bash}
target_link_library(exe
    pthread
    )
\endcode

\subsection thread_not_member_boost error 'thread' is not a member of 'boost'

CMakeLists.txt中添加

\code{bash}

target_link_library(exe
        boost_thread
    )

\endcode

源代码中添加

\code{cpp}
#include <boost/thread.hpp>
\endcode

\subsection undefined_static undefined reference to `类名::count'

static类成员变量在类中只是进行了声明，没有定义，而普通成员变量在生成定义对像的时候进行了定义。
所以类成员变量需要而外定义。

\code{.cpp}
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
\endcode

\subsection thread_not_member_boost error 'thread' is not a member of 'boost'

如果仍然使用boost提供的thread库，那么有可能呢遇到这个问题，一般来说是cmake配置不正确。

CMakeLists.txt中添加

\code{bash}

target_link_library(exe
        boost_thread
    )

\endcode

源代码中添加

\code{cpp}
#include <boost/thread.hpp>
\endcode


\subsection boost_fs_dso_missing  libboost_system.so.1.xx.0: error adding symbols: DSO missing from command line

缺少filesystem对应的库文件，在CMakeLists.txt中添加

```
target_link_libraries(my_target
    boost_system
)
```


\subsection dso_missing DSO missing from command line

完整错误提示

\code
/usr/bin/ld: xxx/xxx.cc.o: undefined reference to symbol '_ZN5boost6system15system_categoryEv'
//usr/lib/x86_64-linux-gnu/libboost_system.so.1.65.1: error adding symbols: DSO missing from command line
collect2: error: ld returned 1 exit status
\endcode

解决方法参见：\ref boost_filesystem


\section 运行篇

\subsection bad_alloc terminate called after throwing an instance of 'std::bad_alloc'

