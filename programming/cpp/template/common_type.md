std::common_type{#cpp11_common_type}
====================================

\code{cpp}
template< class... T >
struct common_type;
\endcode

std::common_type输入多种类型，得到这些类型中的公共类型。什么是公共类型？那就是所有类型都能隐式转换的类型。

具体的转换逻辑比较复杂，参考cppreference.com，这里只举个例子理解一下

\code{cpp}
#include <iostream>
#include <type_traits>
 
template <class T>
struct Number { T n; };
 
template <class T, class U>
Number<typename std::common_type<T, U>::type> operator+(const Number<T>& lhs,
                                                        const Number<U>& rhs) 
{
    return {lhs.n + rhs.n};
}
 
int main()
{
    Number<int> i1 = {1}, i2 = {2};
    Number<double> d1 = {2.3}, d2 = {3.5};
    std::cout << "i1i2: " << (i1 + i2).n << "\ni1d2: " << (i1 + d2).n << '\n'
              << "d1i2: " << (d1 + i2).n << "\nd1d2: " << (d1 + d2).n << '\n';
}
\endcode

输出：
\code{bash}
i1i2: 3
i1d2: 4.5
d1i2: 4.3
d1d2: 5.8
\endcode

也就是说，int转换成了double，这和我们所了解的int类型和double类型做四则运算的隐式转换规则是一样的。

\section common_type的实际应用

common_type在std::chrono中有比较大量的使用。在std::chrono中，两个time_point相减，能得到一个duaration，但是两个time_point可能由不同的数值类型构建而来，所以需要一个最终的输出类型。

详情参见 https://en.cppreference.com/w/cpp/chrono/time_point/operator_arith2。
