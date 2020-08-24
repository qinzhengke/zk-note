libevent：经典的异步事件框架库{#cpp_lib_libevent}
=====================================

<hr>
\section 基于libevent的定时器


<hr>
\section 注意event_dispatch函数的阻塞性

如题libevent中的event_dispatch是阻塞的，所以要注意同一个线程，后面就不要放代码了。

\code{.cpp}
printf("before\n"); // 会被打印
event_dispatch();
printf("afetr\n");  // 不会被打印
\endcode