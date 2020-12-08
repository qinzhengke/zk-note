贝叶斯定理(Bayes' theorem){#bayes_theorem}
========================================

假设 \f$ \boldsymbol{x} \f$ 和 \f$ \boldsymbol{y} \f$ 是两个随机变量，那么贝叶斯规则（“Bayes' rule”）表达为：

\f[

p(\boldsymbol{x}|\boldsymbol{y}) = \frac{p(\boldsymbol{y}|\boldsymbol{x})p(\boldsymbol{x})} {p(\boldsymbol{y})}

\f]


其中， \f$ p(\boldsymbol{x}|\boldsymbol{y}) \f$ 表示在事件 \f$ \boldsymbol{y} \f$发生的情况下，\f$ \boldsymbol{x} \f$发生的概率密度。

\f$ p(\boldsymbol{x}| \boldsymbol{y}) \f$ 也被称为<b>后验概率（posterior）</b>， \f$ p(\boldsymbol{y}|\boldsymbol{x}) \f$被称为<b>似然（“likelihood”）</b>， \f$ p(\boldsymbol{x}) \f$被称为<b>先验概率（“prior”）</b>。

> 【注意】只有在贝叶斯定理的框架下，才有先验概率、后验概率这两种概念，在概率论的另一个框架“频率统计”框架下，是没有的。为了提升沟通效率，不要在频率统计框架下中提先验概率或者后验概率这些概念，例如在讨论最大似然估计的时候。

在后面的 \ref estimation_theory_entry 中，我们会提到， \f$ \boldsymbol{x} \f$往往是需要估计的状态量， 似然 \f$ p(\boldsymbol{y}|\boldsymbol{x}) \f$则是所谓的观测，而先验概率 \f$ p(\boldsymbol{x}) \f$则表示我们通过其他途径已经知道的知识，能够提供一部分的关于 \f$ \boldsymbol{x} \f$分布的信息。

我们现在来看分母，

\f[
p(\boldsymbol{y}) = \int p(\boldsymbol{x},\boldsymbol{y}) d \boldsymbol{x}
\f]

这个过程反过来的话被称为<b>边缘化（marginalization）</b>，即对多维随机变量的某些分量进行全积分，重新得到剩下的分量的概率分布。