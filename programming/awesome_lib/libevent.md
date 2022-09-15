# libevent：经典的异步事件框架库


## 基于libevent的定时器



## 注意event_dispatch函数的阻塞性

如题libevent中的event_dispatch是阻塞的，所以要注意同一个线程，后面就不要放代码了。

```cpp
printf("before\n"); // 会被打印
event_dispatch();
printf("afetr\n");  // 不会被打印
```


## event_add()函数在调用函数结束之后才会开始计时

不能一进函数就设定下一个计时器。

```cpp
func(){
    tv = {0,100000};
    event_add(&tv);

    do_something();//

    // 从这里开始计时，到下一次运行。
}
```