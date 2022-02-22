扩展卡尔曼滤波(Extended Kalman Filter){#ekf_entry}
===============================================


# 前言

根据之前的章节，我们知道卡尔曼滤波是针对线性系统+高斯噪声的最优估计器，线性系统包含两个方面：(1)状态转移过程是线性的，(2)观测方程是线性的。

然而，实际情况存在很多非线性系统，例如双目观测 $$ Z=fb/d $$。TODO:举出更多的例子。

那么对于非线性系统，存在什么样的方法呢？

这就是要提到的扩展卡尔曼滤波。



# 介绍

扩展卡尔曼滤波的内容和卡尔曼滤波基本一致，这里只列出不同的地方

状态转移方程：
$$
\boldsymbol{x}_{k} = f(\boldsymbol{x}_{k-1}) + \boldsymbol{w}_k
$$

观测方程：

$$
\boldsymbol{y}_k = g(\boldsymbol{x}_k) + \boldsymbol{n}_k
$$

这里的状态转移方程和观测方程不再能直接用矩阵乘法来表达了，因为状态转移过程是非线性的，这里用抽象的函数符号 $$ f $$ 和 $$ g $$ 来表示。

扩展卡尔曼滤波的想法很简单，就是在当前状态下，对状态转移方程和观测方程进行泰勒展开，取一阶系数矩阵作为近似。

$$
f(\boldsymbol{x}_{k-1}) \approx \check{\boldsymbol{x}} + \boldsymbol{F}(\boldsymbol{x}_k - \boldsymbol{x}_{k-1})
$$

其中

$$

\boldsymbol{F}_{k-1} = \left . \frac{\partial f(\boldsymbol{x}_{k-1})} {\partial \boldsymbol{x}_{k-1}} \right \vert  _{\hat{\boldsymbol{x}}_{k-1}}

$$


TODO:继续完成


