随机变量的矩（Moment）{#probability_moments}
==========================================

\section 期望

随机变量的一阶矩，也就是<b>期望（mean）</b>定义为：

\f[
\boldsymbol{\mu} = E[\boldsymbol{x}] = \int \boldsymbol{x} p(\boldsymbol{x}) d \boldsymbol{x}
\f]

其中， \f$ E[] \f$表示期望操作符。

\subsection 样本均值

样本均值是期望的一种估计，而且是无偏估计。

\f[
\bar{x} = \frac{1}{n} \sum _i ^N x_i
\f]

什么叫无偏估计呢？

\section 方差

\section 协方差矩阵

随机变量的二阶矩，也就是<b>协方差矩阵（Covariance Matrix）</b>，对于一维随机变量，也称为方差，其定义为：

\f[
\boldsymbol{\sigma} = E[(\boldsymbol{x} - E(\boldsymbol{x}))(\boldsymbol{x}-E(\boldsymbol{x}))^T]
\f]

协方差矩阵表示的是两个变量同时离自身均值的程度，对于协方差矩阵的对角元素，两个变量都是自己，远离的程度理所应当是相同的。

对于非对角元素，如果两个变量都同时远离自身均值，那么它们与自身均值之差的乘积就很大，我们理解为它们的相关性大。
如果两个变量远离均值的时机没有任何关联，那么平均下来，它们与自身均值之差的乘积应该很小，我们可以理解它们相关性小。


\subsection 协方差矩阵的性质

协方差矩阵有以下性质：

协方差矩阵是对称矩阵，根据定义，随机变量的两个分量交换顺序后，它们的协方差不变。

协方差矩阵是半正定的，因为其对角元素均大于等于0。