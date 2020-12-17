带有偏差矫正的卡尔曼滤波{#biased_kmf}
=================================

\section 为何需要带偏差矫正的滤波？

\section 原始模型
假设原始的状态量为： \f$ \boldsymbol{x} \f$，原始的状态转移方程为为：

\f[
\boldsymbol{x}_k = \boldsymbol{A}_k \boldsymbol{x}_{k-1} + \boldsymbol{w}_k
\f]

其中， \f$ \boldsymbol{A}_k \f$ 为状态转移矩阵，\f$ \boldsymbol{w}_k \in \mathbb{R}^N \sim \mathcal{N}(\boldsymbol{0}, \boldsymbol{Q}_k) \f$ 为过程噪声。

原始观测方程：

\f[
    \boldsymbol{y}_k = \boldsymbol{C}_k \boldsymbol{x}_k + \boldsymbol{n}_k
\f]


其中， \f$  \f$ 观测噪声： \f$ \boldsymbol{n}_k \in \mathbb{R}^M \sim \mathcal{N}(\boldsymbol{0}, \boldsymbol{R}_k) \f$

\section 加入bias的问题模型

假设传感器的数量为 \f$ L \f$，那么状态向量表达为：

\f[
\boldsymbol{x}'_k = \left[
\begin{aligned}
     & \boldsymbol{x}_k \\
     & \boldsymbol{b}_k
\end{aligned}
\right]
\f]

其中，

\f[
\boldsymbol{b}_k = [b_{11}, ... , b_{1N'}, ..., b_{L1}, ... , b_{LN'}] ^T
\f]

为偏差状态量，第一个下标 \f$ \{1,...,L\} \f$ 表示偏差状态量的所属传感器，第二个下标 \f$ \{1,...,N'\} \f$ 对应着原始状态量中需要估计偏差的状态分量个数。

\subsection 状态转移方程

状态转移方程为：

\f[
    \boldsymbol{x}_k = 
    \boldsymbol{A}'_k \boldsymbol{x}_{k-1} + \boldsymbol{w}'_k = 
    \begin{bmatrix}
    \boldsymbol{A}_k & \boldsymbol{0} \\
    \boldsymbol{0} & \boldsymbol{B}_{k}
    \end{bmatrix}
    \begin{bmatrix} \boldsymbol{x}_k \\ \boldsymbol{b}_k \end{bmatrix}
    +
    \begin{bmatrix}
    \boldsymbol{w}_k \\
    \boldsymbol{\beta}_k
    \end{bmatrix}
\f]

其中， \f$ \boldsymbol{B}_k \f$ 为偏差状态转移矩阵， \f$ \boldsymbol{\beta}_k \f$ 是偏差状态量的过程噪声，偏差如何演进是根据实际的运动模型来确定。

对于偏差变化率较小的场景，我们可以使用等比例衰减的模型，即

\f[
    \boldsymbol{B}_k = 
    \begin{bmatrix}
    \eta _{11} & & & & & & \\
    & \ddots & & & & & \\
    & & \eta _{1N'} & & & & \\
    & & & \ddots & & & \\
    & & & & \eta _{L1} & &\\
    & & & & & \ddots &\\
    & & & & & & \eta _{LN'}
    \end{bmatrix}
\f]

其中，
\f$  \boldsymbol{b}_k = 0 < \eta _{11}, ... , \eta _{1N'}, ..., \eta _{L1}, ... , \eta _{LN'} < 1 \f$

\subsection 观测方程

加入偏差之后，观测方程不再只有一个，而是每一个传感器对应着不同的观测方程，对于传感器 \f$ i \in [1,L] \f$ 的观测，那么有观测方程：

\f[
    \boldsymbol{y}_k(i)
    = \boldsymbol{C}'_{k}(i) \boldsymbol{x}'_k + \boldsymbol{n}'_k
    = \begin{bmatrix} \boldsymbol{C}_k & \boldsymbol{D}(i) \end{bmatrix}
    \begin{bmatrix} \boldsymbol{x}_k \\ \boldsymbol{b}_k \end{bmatrix} 
    + \boldsymbol{n}_k
\f]

\f[
    \boldsymbol{D}_{k}(i) = 
    \begin{bmatrix}
    \delta _{11} & & & & & & \\
    & \ddots & & & & & \\
    & & \delta _{1N'} & & & & \\
    & & & \ddots & & & \\
    & & & & \delta _{L1} & &\\
    & & & & & \ddots &\\
    & & & & & & \delta _{LN'}
    \end{bmatrix},
    \quad
    \delta _{j1},...,\delta _{jN'} = 
    \left\{
    \begin{aligned}
        & 1, \quad if \quad j=i \\
        & 0, \quad else
    \end{aligned}
    \right.
\f]

此时，状态转移方程和观测方程都已经获得，使用滤波器对应的求解方法，即可对目标状态进行估计。
贝叶斯滤波器求解的属于标准方法，这里不再赘述。

\section 衰减系数

在状态转移方程中，对于偏差的演化，我们使用了等比例衰减的策略，即每一次预测，偏差的均值都比上一时刻要小。
如果不这样做，当目标从同时被多个传感器观测的状态切换到只有1个传感器观测的状态时，偏差仍然会一直持续，如图所示。

偏差持续存在的原因是观测方程的解不是唯一的：“1.0的真值加上0的偏差”和“0.9的真值加上0.1的偏差”，都是符合观测方程的。
所以，估计器会理所当然的认为偏差一直存在。

但是，我们之所以能够对于偏差使用常量模型，是因为更新的时间很短，但是偏差实际上是由多个传感器同时观测才能感受得到的，如果长时间没有多传感器的观测，那么偏差的观测实际上是处于未观测状态，时间一长，常量模型并不成立。

直观的说就是，经过了一段时间传感器A和B的同时估计，我们有信心认为传感器A存在某些偏差，并且当由于某种原因，只剩下了A有实际观测时，短时间内，我们仍然认为A的观测有一定偏差，但是时间太长，我们就没有这么强的信心了。

综上所述，我们设计了衰减系数，在只有1个传感器存在实际观测时，让偏差逐渐的归零，如下图所示。


\section 一个实例

两个激光雷达传感器同时对一辆小车进行观测，我们通过传感器的观测得到其位置 \f$ q \f$，但是两个传感器对其观测存在偏差。
我们的目标是要估计其位置 \f$ p \f$以及速度 \f$ v \f$。

首先，状态向量表达为：

\f[
\boldsymbol{x}_k =
    \begin{bmatrix}
    p & v & b_1 & b_2
    \end{bmatrix} ^T
\f]

其中， \f$ p_k \f$ 是带有偏差的状态分量， \f$ b_{1k} \f$ 和 \f$ b_{2k} \f$ 是小车的位置分别在传感器1和传感器2观测中产生的偏差。

状态转移方程为：

\f[
    \boldsymbol{x}_k = 
    \begin{bmatrix}
    1 & \Delta t & 0 & 0 \\
    0 & 1 & 0 & 0 \\
    0 & 0 & \eta _1 & 0 \\
    0 & 0 & 0 & \eta _2
    \end{bmatrix}
    \boldsymbol{x}_{k-1}
    +
    \begin{bmatrix}
    w_{pk} \\ w_{vk} \\ \beta _{1k} \\ \beta _{2k}
    \end{bmatrix}
\f]

其中， \f$ \Delta t \f$ 是第k帧和k-1帧之间的时间间隔，\f$ \eta _1 \f$和 \f$ \eta _2 \f$ 为衰减系数，典型值为0.9， \f$ w_{pk} \f$ 和 \f$ w_{vk} \f$ 分别是位置和速度的过程噪声。

观测方程为

\f[
    q_k(1) = 
    \begin{bmatrix} 1 & 0 & 1 & 0 \end{bmatrix}
    + n_k
\f]

\f[
    q_k(2) = 
    \begin{bmatrix} 1 & 0 & 0 & 1 \end{bmatrix}
    + n_k
\f]

接下来使用标准的卡尔曼滤波器求解方法进行求解即可。