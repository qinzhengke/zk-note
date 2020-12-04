Exception vs Error code {#exception_vs_error_code}
=================================================

<hr>
\section exception优势的地方

【来源】StackOverflow的提问：“Conventions for exceptions or error codes”

明显，大部分回答者更偏向于使用exception，认为error_code方式要处理的东西太多。

一位被推崇的程序员的博客：“https://nedbatchelder.com/text/exceptions-vs-status.html”，这篇长文说明了exception更加优越的原因。

原因一：exception让代码更加的简洁（clean）

\code{cpp}

// Status code 版本
STATUS DoSomething(int a, int b)
{
    STATUS st;
    st = DoThing1(a);
    if (st != SGOOD) return st;
    st = DoThing2(b);
    if (st != SGOOD) return st;
    return SGOOD;
}

// Statsu code 版本2
#define TRY(s)  { STATUS st = (s); if (st != SGOOD) return st; }

STATUS DoSomething(int a, int b)
{
    TRY(DoThing1(a));
    TRY(DoThing2(b));
    return SGOOD;
}

// Exception版本
void DoSomething(int a, int b)
{
    DoThing1(a);
    DoThing2(b);
}
\endcode

原因二：更多的错误信息

比起返回一个int型错误码，exception能给出更多的信息。

虽然错误码能使用error message来辅助，但是error message有的时候很难抉择需不需要实际执行。

举个例子，callee内部有一个打开头文件操作，如果文件打开失败，它会返回一个错误码，但是它要不要在日志输出error message，比较难抉择。

首先，假设它输出了，这时如果caller认为这其实并不是个错误，打不开就打不开，可能确实不存在某些文件。这时候打出来的error message 就是误报。

如果它没log出来，碰上另一个caller认为这是一个错误，那这就会导致漏报。

最终形成的一个模式就是：作为库函数的callee永远不敢输出error message，直到最外层的caller才敢报，但是最外层能报错的信息和实际报错的起源地址获取得到的信息是不一样的。

本例子中，callee知道打不开的文件名是什么并且能告诉用户，但是caller可能不知道，（文件名在callee里面生成），甚至caller都不知道callee还有打开文件的操作，所以caller无法直接的指出到底是什么原因导致fail，而只能不负责的打一句“callee fail!”，留下一头雾水的用户。

原因三：返回值被占用

很多时候，我们希望函数能够通过返回值的形式提供结果，这样更符合输入输出的阅读结构。
返回值被错误码占用之后，我们只能使用形参作为载体。显然返回值相比形参更能体现“这是一个输出”的情况。
C++11增加的tuple实现了“多个返回值”这个feature，这显然说明大家更喜欢在返回值上下功夫。

\code{cpp}

// status code
int get(int&a);
int a;
int st = get(a);

// exception
int get();
int a = get();

\endcode

原因四：有些函数无法定义返回值

例如构造函数和析构函数。不用返回值，那么只能将绝对不会fail的部分放到构造函数，把其他部分使用一个显示的函数执行，例如initialization。

这本身也衍生了一个关于constructor和initialization的话题： \ref constructor_vs_initialization 。

这个话题的结果也很明显，大家更偏向于构造函数，比较契合OOP的精神。

<hr>
\section exception不好的地方

也有人说出了exception不好的地方：

【来源】https://www.joelonsoftware.com/2003/10/13/13/

1. exception相关逻辑在代码中是看不见的，无法知道exception是谁，来自哪里。
   
（这点似乎有点站不住脚）
   
2. exception会让代码“隐式”地终止，提前退出，可能导致数据状态不一致，例如内存没释放、或者文件没关闭、锁没有释放（这个问题很严重）就退出了。

（这一点貌似很有道理）

3. exception会让二进制的代码增加，可能增加10%到20%，LLVM项目的编码规范就明确指出了：

> In an effort to reduce code and executable size, LLVM does not use RTTI (e.g. dynamic_cast<>;) or exceptions.

4. 一些嵌入式项目的exception实现起来不是实时的，甚至工具链都不支持，说白了是高级feature，一旦哪一天代码库想要用到嵌入式设备，几乎是不可能的。

<hr>
\section 其他扩展阅读

- C++标准对于exception的FAQ： https://isocpp.org/wiki/faq/exceptions
- 知乎： 对使用 C++ 异常处理应具有怎样的态度？