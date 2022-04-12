evpp：奇虎360大神的网络库{#cpp_lib_evpp}
=================================


## evpp_callback evpp库如何在设置回调函数时输入用户参数？

## 问题：
我们都知道可以对数据接收事件设置回调函数，当数据来临的时候自动触发。
在面向对象编程的代码中，通常是在类的成员函数里进行设置回调函数，同样，我们也希望能够在回调函数中能够处理对象中的数据。
然而，回调函数的参数形式是库定好的，并且是库自行调用，我们用户是无法传入参数的。
那么怎么处理对象中的数据呢？

首先，只有静态成员函数能够被当做回调，因为非静态成员函数一定需要一个对象来调用，这是库函数设计回调函数时不支持的。

一种很丑陋的方法就是全局变量传递，定义全局变量来穿透回调函数的作用域，当然这种方法实在太让人难以接受，毕竟极大地破坏了封装特性。

在C语言中，如果库设计得好，就会在设置回调函数的时候，加上一个user_data指针，这个user_data会在回调被库调用的时候按照约定传入，这样以来就可在回调函数中使用指定的变量。

然而，evpp设计上非常的C++11，没有使用C语言的这种方式，没有给用户留下user_data，那么我们要如何在回调函数中传入变量呢？

## 解决方法：

这里要引出C++11非常厉害的一个特性：std::bind，详细内容参考 \ref std_bind 。

std::bind是对函数的一种封装，它的厉害之处在于可以更改参数的个数，本来库要求回调函数形式必须是A(a,b)，如果直接传入一个B(a,b,c)，肯定是会报错的。

但是如果用bind封装就不一样了，传入std::bind(B, _1, _2, 99)，那么库拿到的回调函数仍然是(a,b)的形式！

库函数拿到回调函数的时候，c=99已经传入了！

下面用代码实例进行解释

\code{cpp}
class A{
    void init(){
        a = 10;
        //使用std::bind来封装回调函数，_1和_2表示封装后的函数有两个形参，是callback2的第2，3个参数。
        auto callback_wrap = bind(callback2, this, palceholders::_1, placeholders::_2);
        //将封装后的参数传入回调设置。
        s.SetMessageHandler(callback_wrap);
        s.Init(1080);
        s.Start();
    }

    // 注意回调函数要使用static进行修饰，否则编译会报错。
    // 本来SetMessagehandler函数只接收这种类型的回调，但是没有传入我们想要的数据
    static void callback1(evpp::EventLoop *l, evpp::udp::MessagePtr &msg)

    // 我们扩展了回调，加入了想要处理的变量，直接传入SetMessageHandle会报错，必须使用bind封装后传入。
    static void callback2(A *my_obj, evpp::EventLoop *l, evpp::udp::MessagePtr &msg){
        cout<<"a="<<a<<endl;
    }

    int a;
    evpp::udp::Server s;
}
\endcode

bind特别的神奇，它可以无视函数传参规则，强行将N个参数的函数传入1～N-1个参数的函数形式，理论上可以解决任何callback传参问题。
库的设计者也不需要预留user_data给用户，非常的简便。

补充：evpp的文档非常少，作者提倡看源码来学习使用方法。一开始遇到回调传参的问题，非常头大，网上资料也很少。
只能硬着头皮看源码，全局搜索SetMessageHandler参数，还真的发现了传入3个参数的调用，一看就是使用了std::bind。
研究一番后，终于成功的用上了，作者的C++11还是非常厉害的。

## 如何获取sockect_fd？

问题：根据evpp源码实例，我们知道udp::SendMesssage需要提供socket_fd作为参数，然而在evpp源码所有的实例中，socket_fd都是从接收callback函数中的Message参数里拿到。
那么如果我只想发送，如何获取socket_fd呢？

回答：使用createUDPSocket()函数，

\code{cpp}
evpp_socket_t fd = udp::sock::createUDPSocket(3030); // source port
struct sockaddr_in sin;
sin.sin_family = AF_INET;
sin.sin_addr.s_addr = htonl(0x7f000001);    // 127.0.0.1
sin.sin_port = htons(8080);    // Destination port
evpp::udp::SendMessage(fd, (struct sockaddr *)&sin, buffer, buffer_size);
\endcode