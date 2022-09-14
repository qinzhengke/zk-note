# C++11中的std::function和std::bind


# std_function std::function是什么？

大部分网上的文章只讲了作用，但是没讲std::function存在的意义，作用其实很容易理解，无非是把函数调用封装了，最关键是能直接调用为什么要“多此一举”，再封装一次呢？
网上有一个文章说清楚了：https://www.jianshu.com/p/e396c1aab4b0

类模版std::function是一种通用、多态的函数封装。
std::function的实例可以对任何可以调用的目标实体进行存储、复制、和调用操作，这些目标实体包括普通函数、Lambda表达式、函数指针、以及其它函数对象等。

简要来说：std::function是将所有可调用的实体封装起来，形成了一个新的std::function对象，用户在使用的时候不需要再去一一调用实体，只需要使用新的std::function来调用各实体

如下，std::function作为回调函数使用，它可以调用任何有两个int形参的返回值为int的对象

\code{cpp}
#include <iostream>
#include <functional>
#include <list>
using namespace std;

// 传统C函数
int c_function(int a, int b)
{
    return a + b;
}

// 仿函数
class Functor
{
public:
    int operator()(int a, int b)
    {
        return a + b;
    }
};

int main(int argc, char** argv)
{
    Functor functor;
    std::list<std::function<int(int, int)>> callables;

    callables.push_back(c_function);
    callables.push_back(functor);
    callables.push_back([](int x, int y)->int{
        return x + y;
    });

    // 最关键的地方，可以把不同形式的函数抽象成数组进行调用，和函数指针功能一样。
    for (const auto& e : callables)
    {
        cout << e(3, 4) << endl;
    }
}
\endcode


# std_bind std::bind是什么，有何意义？

网上有一篇文章讲解得很好，链接：https://www.jianshu.com/p/e396c1aab4b0

将函数、成员函数和闭包转成function函数对象
将多元(n>1)函数转成一元函数或者(n-1)元函数。
bind是一种机制，可以预先把指定的可调用的实体的某些参数绑定到已有的变量，产生一个新的可调用实体。
它作为一个通用函数适配器，接收一个可调用对象，生成一个新的可调用对象来适应原对象的参数列表。

## bind的使用方法

比如，存在一个这样的函数check_size，因为这是一个二元函数，当我们要将它作为find_if的参数，会出错。因为find_if只接受一元函数，那么如何解决呢？
一个方法是Lambda表达式，还有一个方法就是使用std::bind

下面这个bind的函数只有一个占位符，即只需要传入一个参数。它将check_size的第二个参数绑定在sz上，sz的值就是check_size的第二个参数的值，而check_size第一个参数需要传入

\code{cpp}
bool check_size(const string &s,string::size_type sz)
{
      return s.size()>=sz;
}

auto wc = find_if(words.begin(),words.end(),bind(check_size,_1,sz));
\endcode

## bind的传参问题

\code{cpp}
auto g = bind(f,a,b,_2,c,_1);
\endcode

如果现在我们调用g(3,5)，那么就相当于bind(f,a,b,5,c,3);
所以_1相当于传递的第一个参数，_2相当于传递的第二个参数...以此类推。

需要注意：bind对于直接绑定的值，是以值传递的方式，对于用_1这类，是使用引用传递。bind的返回值是可以调用的实体，所以通常我们都会将它和function联合在一起使用。

## bind的一个亲身应用实例

参见 \ref evpp_callback

## bind引用参数

有时候对于有些绑定的参数我们希望以引用方式传递，或者说要绑定的参数无法拷贝。
比如ostream 流对象是无法拷贝的，那么我们希望将它传递给bind而不拷贝它，就需要使用ref。
ref返回一个对象，包含给定的引用，是可以拷贝的。

\code{cpp}
for_each(words.begin(),words.end,bind(print,ref(os),_1,' '));
\endcode

## std::bind绑定成员函数和静态成员函数

对于成员函数的绑定，我们一定需要一个调用者，也就是类的实例！
需要注意的是，bind无法绑定重载函数，因为当重载函数的参数个数不相同时，bind也失去了它的意义。

\code{cpp}
class Utils {  
public:  
    Utils(const char* name) {  
        strcpy(_name, name);  
    }        
    void sayHello(const char* name) const {  
        std::cout << _name << " say: hello " << name << std::endl;  
    }        
    static int getId() {  
        return 10001;  
    }         
    int operator()(int i, int j, int k) const {  
        return i + j + k;  
    }       
private:  
    char _name[32];  
};  

int main(void) {  

    // 绑定成员函数  
    Utils utils("Vicky");  
    auto sayHello = std::bind(&Utils::sayHello, utils/*调用者*/, std::placeholders::_1);  
    sayHello("Jack");  

    // 绑定静态成员函数  
    auto getId = std::bind(&Utils::getId);  
    std::cout << getId() << std::endl; 
}
\endcode

## bind与function搭配

在cocos2dx的源码中，我们经常可以看到function作为函数形参，而bind作为实参传入

\code{cpp}
bool Label::multilineTextWrap(std::function<int(const std::u16string&, int, int)> nextTokenLen)
{}

bool Label::multilineTextWrapByChar()
{
      return multilineTextWrap(std::bind(getFirstCharLen, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
}
\endcode


# std_placeholder std::placeholder

我们知道std::bind会把一个函数转换成另外一个函数，并且形参的个数可能会改变，所以需要所谓的“占位符”去表示新函数中，编号为_i的参数在旧函数那个地方传入。

当调用新的函数对象时，新函数对象会调用被调用函数，并且其参数会传递到被调用函数参数列表中持有与新函数对象中位置对应的占位符。


\code{cpp}
void function(arg1,arg2,arg3,arg4,arg5)
{
    //do something
}
auto g = bind(function,a,b,_2,c,_1);
\endcode

新的函数对象：g
被调用函数：function
当调用函数对象g时候，函数对象g会调用function函数，并把其参数传给function函数，g的第一个参数会传给function的持有占位符_1的位置，即arg5。第二个参数会传给function的持有占位符_2的位置，即arg3。

\code{cpp}
void g(X,Y);
// 相对于调用下面函数
function(function,a,b,Y,c,X);
\endcode

其中的arg1,arg2,arg4已经被绑定到a,b,c上。

placeholder本身是命名空间，占位符实际上是“_1”，“_2”，“_3”等等。


# std_bind_ref 【坑点】std::bind传入引用“&”失效

普通的&传入引用是无效的，结果仍然是复制传递，必须使用std::ref(a)进行另一种引用，才能真正的引用传递，具体原理还没仔细了解，参见： \ref cpp11_std_ref