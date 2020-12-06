卡尔曼滤波(Kalman Filter){#kalman_filter}
========================

\section kf_problem_setup 问题建立（problem_setup)

市面上存在很多对卡尔曼滤波不同的符号表达，有点混乱，我们在这里参考文献《State Estimation for Robotics》 \cite barfoot2017state 的写法。
对比过维基百科、《概率机器人》、《视觉SLAM十四讲》 ，个人认为这本书是说的最清晰的。

维基百科太片段，没讲来龙去脉，也没有推导，并且双下标表示方法不够简洁。

《概率机器人》时间比较早，表达方式和当代主流表达不太一样，而且有点搞不清重点。

《视觉SLAM十四讲》其实非常好，但毕竟关于卡尔曼滤波的篇幅有限。

运动方程：
\f[
    \boldsymbol{x}_k = \boldsymbol{A}_{k-1} \boldsymbol{x}_{k-1} + \boldsymbol{v}_k + \boldsymbol{w}_k
\f]

观测方程：

\f[
    \boldsymbol{y}_k = \boldsymbol{C}_k \boldsymbol{x}_k + \boldsymbol{n}_k
\f]

其中两个随机变量如下所示：

过程噪声： \f$ \boldsymbol{w}_k \in \mathbb{R}^N \sim \mathcal{N}(\boldsymbol{0}, \boldsymbol{Q}_k) \f$

观测噪声： \f$ \boldsymbol{n}_k \in \mathbb{R}^M \sim \mathcal{N}(\boldsymbol{0}, \boldsymbol{R}_k) \f$

\section kf_usage 用法

首先建立预测模型，它包含两部分，第一部分是运动方程，也就是待估计的状态转移方程，第二部分是协方差矩阵的转移方程。


对状态先验预测：

\f[
\label{kf_1}
\check{\boldsymbol{x}}_k = \boldsymbol{A}_{k-1} \hat{\boldsymbol{x}}_{k-1} + \boldsymbol{v}_{k}
\f]

对状态先验协方差的预测：

\f[
\check{\boldsymbol{P}}_k =  \boldsymbol{A}_{k-1} \hat{\boldsymbol{P}}_{k-1} \boldsymbol{A}_{k-1}^{T} + \boldsymbol{Q}_{k}
\f]

计算最优卡尔曼增益：

\f[
    \boldsymbol{K}_{k}  =  \check{\boldsymbol{P}}_k \boldsymbol{C}_k ^T (\boldsymbol{C}_k \check{\boldsymbol{P}}_k \boldsymbol{C} _k ^T + \boldsymbol{R}_k)^{-1}
\f]

更新后验估计：

\f[
    \hat{\boldsymbol{x}}_k = \check{\boldsymbol{x}}_k + \boldsymbol{K}_k (\boldsymbol{y}_k - \boldsymbol{C}_k \check{\boldsymbol{x}}_k)
\f]

更新后验估计的协方差：

\f[
   \hat{\boldsymbol{P}}_k = (1-\boldsymbol{K}_k \boldsymbol{C}_k) \check{\boldsymbol{P}}_k
\f]

<center>
![](./figures/kalman_filter_01.png)
图：卡尔曼滤波中的相关高斯分布，图片来自于文献 \cite barfoot2017state 。
</center>


\section kf_origin 推导：最大化后验概率

思路：根据贝叶斯规则展开后验概率，可以得到后验概率关于似然和先验的表达。

\f[
    P(\boldsymbol{x}_k | \boldsymbol{x}_{k-1}, \boldsymbol{y}_k, \boldsymbol{v}_k) \propto
    P(\boldsymbol{y}_k | \boldsymbol{x}_k) P(\boldsymbol{x}_k | \boldsymbol{x}_{k-1}, \boldsymbol{v}_k)
    \label{eq_kf_03_01}
\f]

求最大化，我们通常的做法是求导等于0，但是这里我们可以使用高斯分布的性质。

高斯分布概率最大的位置为期望，式 \\eqref{eq_kf_03_01} 的分子中似然项 \f$ P(\boldsymbol{y}_k | \boldsymbol{x}_k) \f$ 恰好对应观测方程，是高斯分布
\f$ P(\boldsymbol{x}_{k-1}, \boldsymbol{v}_k) \f$ 则恰好对应着运动方程，也是高斯分布。

两个高斯分布相乘，仍然是高斯分布，列出两边的公式，对比两边的指数部分，求左侧后验的期望和方差在右边的表达式，即可求得 \f$ \hat{\boldsymbol{P}}_k \f$ 关于 \f$ \check{\boldsymbol{P}}_k \f$ 的表达式，以及 \f$ \hat{\boldsymbol{x}_k} \f$ 关于 \f$ \check{\boldsymbol{x}}_k \f$ 的表达式。

> 这是一个引用。