# C++11中的多线程

## std::thread::join

注意join会阻塞当前进程的运行，直到join的参数函数结束，如果不想阻塞，需要使用detach()函数。


## std::mutex

std::mutex是c++11自带的互斥锁，

```cpp
std::mutex mtx;
mtx.lock();
// do somthing
mtx.unlock();
```

## std::mutex::try_lock

```cpp
#include <chrono>
#include <mutex>
#include <thread>
#include <iostream> // std::cout
 
std::chrono::milliseconds interval(100);
 
std::mutex mutex;
int job_shared = 0; // both threads can modify 'job_shared',
    // mutex will protect this variable
 
int job_exclusive = 0; // only one thread can modify 'job_exclusive'
    // no protection needed
 
// this thread can modify both 'job_shared' and 'job_exclusive'
void job_1() 
{
    std::this_thread::sleep_for(interval); // let 'job_2' take a lock
 
    while (true) {
        // try to lock mutex to modify 'job_shared'
        if (mutex.try_lock()) {
            std::cout << "job shared (" << job_shared << ")\n";
            mutex.unlock();
            return;
        } else {
            // can't get lock to modify 'job_shared'
            // but there is some other work to do
            ++job_exclusive;
            std::cout << "job exclusive (" << job_exclusive << ")\n";
            std::this_thread::sleep_for(interval);
        }
    }
}
 
// this thread can modify only 'job_shared'
void job_2() 
{
    mutex.lock();
    std::this_thread::sleep_for(5 * interval);
    ++job_shared;
    mutex.unlock();
}
 
int main() 
{
    std::thread thread_1(job_1);
    std::thread thread_2(job_2);
 
    thread_1.join();
    thread_2.join();
}
```


## std::lock_guard

```cpp
#include <thread>
#include <mutex>
#include <iostream>
 
int g_i = 0;
std::mutex g_i_mutex;  // protects g_i
 
void safe_increment()
{
    const std::lock_guard<std::mutex> lock(g_i_mutex);
    ++g_i;
 
    std::cout << std::this_thread::get_id() << ": " << g_i << '\n';
 
    // g_i_mutex is automatically released when lock
    // goes out of scope
}
 
int main()
{
    std::cout << "main: " << g_i << '\n';
 
    std::thread t1(safe_increment);
    std::thread t2(safe_increment);
 
    t1.join();
    t2.join();
 
    std::cout << "main: " << g_i << '\n';
}
```


## init阶段和start阶段

通常来说多线程模块会有init、start、run几种阶段，尽量分清楚这几个阶段的任务不要混淆，对于多线程编程，能很大概率避免一些线程启动混乱带来的程序崩溃问题。

init阶段：只做一些参数的初始化，预计算等等，不启动任何线程，不阻塞

start阶段：start阶段一般是指创建另一个线程并且在线程中进行run()，不会阻塞当前线程。

run阶段：run阶段一般是指在本线程中持续运行，是会阻塞当前线程。

如果想要分发多个线程，不要在init中操作，因为init函数有可能在启动线程后再做一些其他操作，可能会耦合到刚刚启动的线程，尽量在主线程的run()函数中start另外的线程。