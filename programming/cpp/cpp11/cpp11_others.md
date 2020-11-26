C++11中的其他有用特性{#cpp11_others}
=================================


<hr>
\section using 代替 typedef

using代替typedef的好处是可以支持模板。

\code{.cpp}
// C++11写法
template <size_t N>
using Vector = Matrix<N, 1>;

// C++03写法
template <size_t N>
struct Vector
{
    typedef Matrix<N, 1> type;
};
\endcode