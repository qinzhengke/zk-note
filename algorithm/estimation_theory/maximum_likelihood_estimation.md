最大似然估计(Maiximum Likelihood Estimation){#mle}
================================================

# 最大似然估计的定义

假设随机变量 $$ \boldsymbol{x} $$的概率密度函数为 $$ p(\boldsymbol{x};\boldsymbol{\theta}) $$，其中 $$ \boldsymbol{\theta} $$ 是<b>固定的、未知的</b>参数，例如高斯分布中的 $$ \boldsymbol{\mu} $$ 和 $$ \boldsymbol{\Sigma} $$ 。

则固定 $$ \boldsymbol{x} $$， $$ p(\boldsymbol{x}; \boldsymbol{\theta}) $$ 作为参数 $$ \boldsymbol{\theta} $$ 的函数，称为<b>似然函数</b>，记为

$$
L(\boldsymbol{x};\boldsymbol{\theta}) = p(\boldsymbol{x}; \boldsymbol{\theta}), \quad \theta \in \Theta
$$

其中， $$ \Theta $$ 为参数空间。

并称

$$
    \boldsymbol{\theta}^* = \arg \max _{\boldsymbol{\theta} \in \boldsymbol{\Theta}} {L(\boldsymbol{x};\boldsymbol{\theta})}
$$

为最大似然估计。

> 本文用来表示似然函数的符号 $$ p(\boldsymbol{x}; \boldsymbol{\theta}) $$来自于《计算机视觉中的数学方法》。
> 在贝叶斯框架下，往往会使用 $$ p( \boldsymbol{\theta} | \boldsymbol{x}) $$ 来表示似然函数，注意两者的区别。

# 应用于独立同分布实验

一些文献，例如维基百科和《计算机视觉中的数学方法》，都会在最大似然估计的定义后<b>紧接着</b>推导最大似然估计在独立同分布问题中的求解方法。
可能看起来会误以为独立同分布是最大似然估计所必须包含的前提条件，然而实际上并不是的，独立同分布抽样问题也只是最大似然估计的一种应用而已。

尽管如此，我们仍然从独立同分布抽样问题来展开最大似然估计，后面再讨论非独立同分布抽样的问题。

假设 $$ \boldsymbol{x} = (\boldsymbol{y}_1,\boldsymbol{y}_2,...,\boldsymbol{y}_k) $$ 是一次独立抽样，那么其联合概率密度( $$ \boldsymbol{Y}_1=\boldsymbol{y}_1, \boldsymbol{Y}_2 = \boldsymbol{y}_2,..., \boldsymbol{Y}_k=\boldsymbol{y}_k的概率密度多少 $$ )为：

$$
    p(\boldsymbol{x}; \boldsymbol{\theta}) = \prod _{j=1} ^{k} {p_j(\boldsymbol{y}_j); \boldsymbol{\theta}}
$$

既然我们做实验得到了这个 $$ \boldsymbol{x} $$这个结果，我们自然认为它发生的概率最大，那么如果把概率密度画出来，那么 $$ \boldsymbol{x} $$ 一定是极值点。

那么我们只需要找到合适的 $$ \boldsymbol{ \theta } $$，让概率密度函数在 $$ \boldsymbol{x} $$ 处的导数为0即可，即

$$
    \frac{\partial p(\boldsymbol{x}; \boldsymbol{\theta})}{\partial \boldsymbol{\theta}} = 0
$$

为了求解方便，我们对 $$ p(\boldsymbol{x}; \boldsymbol{\theta}) $$ 取对数，得到对数似然函数

$$
    l(\boldsymbol{x}; \boldsymbol{\theta}) = \log \prod _{j=1} ^{k} p_j(\boldsymbol{y}_j; \boldsymbol{\theta}) 
    = \sum _{j=1} ^{k} \log p_j(\boldsymbol{y}_j; \boldsymbol{\theta})
$$

# 关于最大似然估计和贝叶斯估计联系和区别的讨论

极大似然估计和贝叶斯估计是两种最常见的估计方法，两者在众多应用学科中都有广泛的应用。
初学者往往对于两者的区分解释得并不清楚。
如果单独地研究这两种方法，往往都能理解，但是两者放在一起，又无法精确的表达其中的不同点。

本文认为产生这种理解困难的原因是：<b>文献中，这两种方法往往在推导与举例的过程中，是基于不同的问题。</b>

比如在讨论最大似然估计的时候，往往是从<b>参数估计</b>的角度出发，进行推导，例如维基百科、《计算机视觉中的数学方法》。

而在讨论贝叶斯估计的时候，问题往往是<b>状态估计</b>，例如贝叶斯滤波器的相关讨论。

对于参数估计，往往参数在估计的过程中是不变的，而且分量数是固定的，我们常常基于多次独立同分布实验进行最大似然估计的推导。

而对于状态估计，有两种视角：(1)状态不会只估计一次，而是连续不断地估计，而且系统演进的过程中，状态会改变。
(2)如果把演进过程中的所有状态合成一个长的向量，那么这是一次估计。

无论从哪种视角来看，状态估计都不是多次独立同分布的实验。对于视角(1)，状态在改变，那么观测所得概率分布就是在变的，所以同分布并不成立。
对于视角(2)，虽然总状态不会改变了（所有时刻状态合成一个总状态），但是也只有一次估计，我们并没有按照原来的方式再把过程演进一次。

所以，两者用于举例的问题不一样，那么方法的差异自然难以直观对比。


在参数估计问题中，参数往往是固定不变的未知量，而大部分最大似然估计的推导出发点，也是多次独立实验。

