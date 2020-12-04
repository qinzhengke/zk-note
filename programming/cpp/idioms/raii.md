RAII：资源获取即初始化{#cpp_raii}
=============================

全称是：Resource Acqusition Is Initialization

下面的内容来自维基百科：

RAII，全称资源获取即初始化（英语：Resource Acquisition Is Initialization），它是在一些面向对象语言中的一种惯用法。RAII源于C++，在Java，C#，D，Ada，Vala和Rust中也有应用。1984-1989年期间，比雅尼·斯特劳斯特鲁普和安德鲁·柯尼希在设计C++异常时，为解决资源管理时的异常安全性而使用了该用法[1]，后来比雅尼·斯特劳斯特鲁普将其称为RAII[2]。

RAII要求，资源的有效期与持有资源的对象的生命期严格绑定，即由对象的构造函数完成资源的分配（获取），同时由析构函数完成资源的释放。在这种要求下，只要对象能正确地析构，就不会出现资源泄露问题。

<hr>
\section 作用

RAII的主要作用是在不失代码简洁性[3]的同时，可以很好地保证代码的异常安全性。

下面的C++实例说明了如何用RAII访问文件和互斥量：

\code{cpp}
#include <string>
#include <mutex>
#include <iostream>
#include <fstream>
#include <stdexcept>
 
void write_to_file(const std::string & message)
{
    // 创建关于文件的互斥锁
    static std::mutex mutex;
 
    // 在访问文件前进行加锁
    std::lock_guard<std::mutex> lock(mutex);
 
    // 尝试打开文件
    std::ofstream file("example.txt");
    if (!file.is_open())
        throw std::runtime_error("unable to open file");
 
    // 输出文件内容
    file << message << std::endl;
 
    // 当离开作用域时，文件句柄会被首先析构 (不管是否抛出了异常)
    // 互斥锁也会被析构 (同样地，不管是否抛出了异常)
}
\endcode

C++保证了所有栈对象在生命周期结束时会被销毁(即调用析构函数)[4]，所以该代码是异常安全的。无论在write_to_file函数正常返回时，还是在途中抛出异常时，都会引发write_to_file函数的堆栈回退，而此时会自动调用lock和file对象的析构函数。

当一个函数需要通过多个局部变量来管理资源时，RAII就显得非常好用。因为只有被构造成功(构造函数没有抛出异常)的对象才会在返回时调用析构函数[4]，同时析构函数的调用顺序恰好是它们构造顺序的反序[5]，这样既可以保证多个资源(对象)的正确释放，又能满足多个资源之间的依赖关系。

由于RAII可以极大地简化资源管理，并有效地保证程序的正确和代码的简洁，所以通常会强烈建议在C++中使用它。

【qzk】我之前一直不知道有了“lock”和“unlock”，为什么还需要“lock_guard”，现在知道了，当使用exception时，函数可能提前退出，“unlock”可能不会被执行！
而lock_guard使用析构函数来释放锁，所以不管函数中途从哪里退出，锁都会被释放。所以文件是不是也可以这样操作？

<hr>
\section fstream中的RAII

实际上，标准库的fstream早已经使用RAII的思想来实现，fstream的close()函数已经不需要开发者手动在结尾添加了。

fstream的对象在析构的时候会自动地调用close()函数，当然手动close()也不会产生问题。