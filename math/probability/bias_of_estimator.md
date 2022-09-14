# 估计的偏差

# 采样均值的偏差

我们知道，采样均值 $$ \bar{x} $$ 是对随机变量的期望的一种估计，而且我们已经知道了，它是无偏的，也就是说。

$$
E[\bar{x}] = E[\frac{1}{N} \sum _i ^N x_i] = E[x]
$$

虽然这个结论司空见惯，看起来也理所当然，但是其本质仍然是需要考究。

首先，$$ \bar{x} $$ 本身也是一个随机变量！它有自己的概率分布，也有自己的期望和方差，当它的期望和 $$ x $$ 的期望相等时，我们称之为无偏估计。

我们知道，随机变量可以通过多次重复实验，不断地得到具体的值，同样，$$ \bar{x} $$也能够通过通过多次重复实验，不过每一次重复实验，本身又包含了N次对 $$ x $$的实验。

当看到 $$ E[\bar{x}] $$ 时，心里要有概念，它是多次对 $$ \bar{x} $$　重复实验的结果，而不是对 $$ x $$　。

$$ \bar{x} $$ 是 $$ x $$　的无偏估计，仍然是需要证明的

<table><tr><td>
【证明】$$ \bar{x} $$是 $$ x $$的无偏估计

$$
\begin{aligned}
E[\bar{x}] = & E[\frac{1}{N}\sum _i ^N x_i] \\
           = & \frac{1}{N} \sum _i ^N E[x_i]
\end{aligned}
$$

在不知道 $$ x_i $$的情况下，它是一个随机变量，所以有 $$ E[x_i] = E[x] $$

从而

$$
E[\bar{x}] = E[x]
$$

</td></tr></table>

【疑问】其实有一个疑问，$$ x_i $$ 是不是一个随机变量？它的定义难道不是一次实验出来的具体数值吗？

# 采样方差的偏差

对于随机变量 $$ x $$，未矫正的采样方差（Uncorrected Sample Variance）的定义是：

$$
    \bar{x} = \frac{1}{N} \sum _i ^N {x_i}
$$

$$
    S^2 = \frac{1}{N} \sum _i ^N {(x_i-\bar{x})^2}
$$

为什么叫做“未矫正”的呢？因为这个估计实际上是有偏的。

<table><tr><td>

【证明】未矫正的采样方差是有偏的。

$$
\begin{align}
    \operatorname{E}[S^2]
        &= \operatorname{E}\left[\frac 1 n \sum_{i=1}^n \big(x_i-\overline{x}\big)^2 \right]
         = \operatorname{E}\bigg[\frac 1 n \sum_{i=1}^n \bigg((x_i-\mu)-(\overline{x}-\mu)\bigg)^2 \bigg] \\[8pt]
        &= \operatorname{E}\bigg[\frac 1 n \sum_{i=1}^n (x_i-\mu)^2 -
                                  \frac 2 n (\overline{x}-\mu)\sum_{i=1}^n (x_i-\mu) +
                                  \frac 1 n (\overline{x}-\mu)^2 \cdot n\bigg] \\[8pt]
        &= \operatorname{E}\bigg[\frac 1 n \sum_{i=1}^n (x_i-\mu)^2 -
                                  \frac 2 n (\overline{x}-\mu)\sum_{i=1}^n (x_i-\mu) +
                                  (\overline{x}-\mu)^2 \bigg] \\[8pt]
        &= \operatorname{E}\bigg[\frac 1 n \sum_{i=1}^n (x_i-\mu)^2 -
                                  \frac 2 n (\overline{x}-\mu) \cdot n \cdot (\overline{x}-\mu)+
                                  (\overline{x}-\mu)^2 \bigg] \\[8pt]
        &= \operatorname{E}\bigg[\frac 1 n \sum_{i=1}^n (x_i-\mu)^2 - (\overline{x}-\mu)^2 \bigg] \\[8pt]
        &= \operatorname{E}\bigg[\frac 1 n \sum_{i=1}^n (x_i-\mu)^2\bigg] - \operatorname{E}\bigg[(\overline{x}-\mu)^2 \bigg] \\[8pt]
         &= \sigma^2 - (\overline{x}-\mu)^2
          = \left( 1 -\frac{1}{n}\right) \sigma^2 < \sigma^2.
    \end{align}
$$

</td></tr></table>