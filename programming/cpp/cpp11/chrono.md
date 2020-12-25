标准库chrono{#cpp11_chrono}
=========================

chrono是C++11引入的一个关于时间的标准库。
它可以提供系统时间。

一个常规的具体应用为计算模块运行耗时，在chrono出现之前，开发者使用第三方库来完成该功能，例如opencv的计时模块。

> 注意，在C++20标准以前，chrono不提供日期相关的接口，所以想要用chrono来计算和年月日相关的需求，是做不到的。
> 我自己在这里就被卡了很久，如果想要找和日期相关的操作，去看C语言的time.h，或者C++的开源库：date，或者看C++20。


\section 认识clock

\subsection steady_clock

steady_clock表示时间单调递增的时钟，这个时钟是不会倒退的，不存在重置这种操作，每一个tick的时间长度也是相等的。
它和世界时钟不一定同步，例如它可以在系统启动的时候开始计时。

\subsection system_clock

system_clock指的是wall clock，也就是日常用的地球时钟。它不一定是单调递增的，因为它可以被系统随时调整，例如连接网络的时候与世界时间同步。
C++11的标准并未定义system_clock的起始时刻，但通常来说，大部分编译器都选择使用Unix时间作为纪元(epoch)。
Unix使用的纪元（也就是计时的起点）在公元纪元对应的时间为：1970年1月1日0时0分0秒。

\subsection steady_clock和system_clock的对比

看定义其实就明白了两者的不同，steady_clock提供了稳定的时钟，可以用来稳定衡量时间段长度，不用担心系统时钟被修改，而导致程序出现bug。
而system_clock主要是提供了世界时间的接口。

\subsection 其他

除了steady_clock和system_clock之外，c++11还提供了high_resolution_clock，表示tick周期最小的时钟。
C++11并未定义最小周期是多少，并且该时钟是steady还是system也并未定义，通常来说high_resolution_clock会直接复用steady_clock或者system_clock。
所以尽量少用？

\subsection now()函数
每一个时钟都得提供一个now()函数，返回一个time_point类型，表示根据自身的时钟，当前时刻是多少。

下面提供了一个案例，基于steady_clock和now()函数来估计代码运行耗时。

\code{cpp}
#include <iostream>
#include <iomanip>
#include <vector>
#include <numeric>
#include <chrono>
 
volatile int sink;
int main()
{
    std::cout << std::fixed << std::setprecision(9) << std::left;
    for (auto size = 1ull; size < 1000000000ull; size *= 100) {
        // record start time
        auto start = std::chrono::steady_clock::now();
        // do some work
        std::vector<int> v(size, 42);
        sink = std::accumulate(v.begin(), v.end(), 0u); // make sure it's a side effect
        // record end time
        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double> diff = end-start;
        std::cout << "Time to fill and iterate a vector of " << std::setw(9)
                  << size << " ints : " << diff.count() << " s\n";
    }
}
\endcode

运行结果：
\code{bash}
Time to fill and iterate a vector of 1         ints : 0.000067375 s
Time to fill and iterate a vector of 100       ints : 0.000000688 s
Time to fill and iterate a vector of 10000     ints : 0.000044553 s
Time to fill and iterate a vector of 1000000   ints : 0.004200451 s
Time to fill and iterate a vector of 100000000 ints : 0.260108972 s
\endcode


\section std_duration std::duration

\code{cpp}
template<
    class Rep,
    class Period = std::ratio<1>
> class duration;
\endcode

duration表示一个时间间隔（time interval），它由一个抽象的类型Rep来表达tick的数值，以及一个std::ratio类型Period来表达一个tick的周期。

例如，Rep可以选择uint32_t，Period选择std::ratio<1>，Rep（uin32_t）对应的变量假设为10，那么整个duration表达的时间间隔为10s。

注意，std::ratio是分数，可以表达小于1的数，例如Period选择std::ratio<1,1000>，刚才的例子就变成了10ms。


\section time_point

\code{cpp}
template<
    class Clock,
    class Duration = typename Clock::duration
> class time_point;
\endcode

有了时钟（clock）和时间间隔（duaration），我们就可以表示一个时间点了，也就是std::time_point。

time_point由Clock和Duartion组成，Clock可以选择上文诉述的system_clock或者steady_clock，亦或者是high_resolution_clock（虽然不是很常用）。
Duartion可以选择对应的数值类型和周期。

理解起来很简单，time_point的含义就是以什么时钟（起点）为基准，过了多长的时间。

\subsection 两个time_point相减得到duartion

两个time_point相减得到duration是常见的应用，上面的示例代码也提供了例子。

要说明的就是duaration要定义个Rep来表示数值的类型，示例中使用了double。

