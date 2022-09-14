# C++的lambda表达式

## lambda表达式在什么地方使用？

需要传入函数（函数指针，函数对象）的地方，例如调用std::sort时，需要传入一个比较大小的函数，此时不需要在额外定义一个函数，直接摊开写lamdba表达式即可。
这使得程序更加的简洁、紧凑。


## 如何将lambda表达式传入template函数？

这里举一个稍微没那么简单的例子，一个由结构体组成的vector,现在要打印所有元素成员a的值。

首先，在模板定义的地方，函数对象类型也要定义成模板类型，而不是std::function<>类型。

其次，在实例化的时候，lambda表达式的传参直接用auto即可，不需要再写一遍类型，（前面的模板函数实例化时已经写了）。

\code{cpp}

template<typename T, typename F>
lib_func(std::vector<T> t, F usr_func){
    for(auto a : t){
        usr_func(t);
    }
}

struct tuple_t{ int a, int b};

vector<tuple_t> v;

lib_func<tuple_t>(v, [](auto x){printf("%d,",x.a);});

\endcode