优化理论{#optimization}
===========================

此页面是优化理论相关内容的入口。

\section jacobian 雅克比(Jacobian)矩阵

复习一下雅克比矩阵，注意雅克比矩阵中的行是函数分量，列是变量分量。

\f[
\boldsymbol J = \begin{bmatrix}
    \dfrac{\partial \boldsymbol{f}}{\partial x_1} & \cdots & \dfrac{\partial \boldsymbol{f}}{\partial x_n} \end{bmatrix}
= \begin{bmatrix}
    \dfrac{\partial f_1}{\partial x_1} & \cdots & \dfrac{\partial f_1}{\partial x_n}\\
    \vdots & \ddots & \vdots\\
    \dfrac{\partial f_m}{\partial x_1} & \cdots & \dfrac{\partial f_m}{\partial x_n} \end{bmatrix}

\f]

对于单值函数，那么雅克比矩阵实际上是一个行向量，如下所示。

\f[
\boldsymbol J = \begin{bmatrix}
    \dfrac{\partial f}{\partial x_1} & \cdots & \dfrac{\partial f}{\partial x_n} \end{bmatrix}
\f]

\section hessian 单值函数的海森(Hessian)矩阵

复习一下海森矩阵，只讨论单值函数，公式如下所示：

\f[
\boldsymbol{H} = \begin{bmatrix}\dfrac {\partial^2 f}{\partial x_1^2} & \dfrac{\partial^2 f}{\partial x_1\,\partial x_2} & \cdots & \dfrac{\partial^2 f}{\partial x_1\,\partial x_n} \\  \\
\dfrac{\partial^2 f}{\partial x_2\,\partial x_1} & \dfrac{\partial^2 f}{\partial x_2^2} & \cdots & \dfrac{\partial^2 f}{\partial x_2\,\partial x_n} \\  \\
\vdots & \vdots & \ddots & \vdots \\  \\
\dfrac{\partial^2 f}{\partial x_n\,\partial x_1} & \dfrac{\partial^2 f}{\partial x_n\,\partial x_2} & \cdots & \dfrac{\partial^2 f}{\partial x_n^2}
\end{bmatrix}\,
\f]

\section opt_gradient 梯度下降

梯度下降最简单，计算一阶导数，直接向梯度负方向搜索，步长为1。

\f[
    \boldsymbol{x}_{k} =  \boldsymbol{x}_{k-1} - \boldsymbol{J}^T
\f]

显然这种搜索方法步长尺度不确定，步长太小，收敛很慢，步长太大，则来回波动，下面引入更优的牛顿法。

\section opt_newton 牛顿法

\f[
    \boldsymbol{x}_{k} =  \boldsymbol{x}_{k-1} - \boldsymbol{H}^{-1} \boldsymbol{J}^T 
\f]

前提条件：

1. \f$ \boldsymbol{H} \f$必须是可逆矩阵，这个条件显然必须满足，否则无法计算 \f$ \Delta \boldsymbol{x} \f$

2. \f$ \boldsymbol{H} \f$必须是正定矩阵， 否则拟合出来的二次函数是是一个非凸的函数，导数为0的点可能不是最小值。

推导过程：
\f[
    f(\boldsymbol{x}+\Delta \boldsymbol{x}) \approx f(\boldsymbol{x}) + \boldsymbol{J}^T \Delta x + \frac{1}{2}{\Delta x}^T\boldsymbol{H}{\Delta x}
\f]

对\f$ \Delta x \f$ 求导后等于0，可以得到二次近似的最小值，即
\f[
    \frac{df(\boldsymbol{x}+\Delta \boldsymbol{x})}{d\Delta \boldsymbol{x}} = \boldsymbol{J} + \boldsymbol{H}{\Delta \boldsymbol{x}} = \boldsymbol{0}
\f]


\f[
    \Delta \boldsymbol{x} = -\boldsymbol{H}^{-1} \boldsymbol{J}
\f]

\section opt_gausian_newton 高斯牛顿法

首先要明确一点，高斯牛顿法并不是针对任意函数的，而是专门针对\f$ \|\boldsymbol{f}(\boldsymbol{x})\|^2 \f$这种形式的目标函数

这里的\f$ \boldsymbol{f}(\boldsymbol{x}) \f$可以是多值函数。

首先，将\f$ \boldsymbol{f}(\boldsymbol{x}) \f$进行一阶的泰勒展开。

\f[
    \boldsymbol{f}(\boldsymbol{x}+\Delta \boldsymbol{x}) \approx \boldsymbol{f}(\boldsymbol{x}) + \boldsymbol{J} \Delta \boldsymbol{x}
\f]

\f$ \boldsymbol{J} \f$是 \f$ \boldsymbol{f}(\boldsymbol{x}) \f$的雅克比矩阵，维度是 \f$ m \times n \f$。

那么现在来求解最优迭代增量。
\f[
    \Delta \boldsymbol{x} ^* = \arg \min_{\Delta \boldsymbol{x}}{\|\boldsymbol{f}(\boldsymbol{x})+\boldsymbol{J}\Delta \boldsymbol{x} \|^2}
\f]

\f[
    \begin{aligned}
    \|\boldsymbol{f}(\boldsymbol{x})+\boldsymbol{J}\Delta \boldsymbol{x} \|^2
    & = (\boldsymbol{f}(\boldsymbol{x})+\boldsymbol{J} \Delta \boldsymbol{x})^T (\boldsymbol{f}(\boldsymbol{x})+\boldsymbol{J} \Delta \boldsymbol{x}) \\
    & = \| \boldsymbol{f}(\boldsymbol{x}) \|^2 + 2 \boldsymbol{f}(\boldsymbol{x})^T \boldsymbol{J} \Delta \boldsymbol{x} + \Delta \boldsymbol{x}^T \boldsymbol{J}^T \boldsymbol{J} \Delta \boldsymbol{x}
    \end{aligned}
\f]

求导后等于0，可得

\f[
    \boldsymbol{J}^T \boldsymbol{f}(\boldsymbol{x}) + \boldsymbol{J}^T \boldsymbol{J}\Delta \boldsymbol{x} = \boldsymbol{0}
\f]

\f[
    \Delta\boldsymbol{x} = -\boldsymbol{J}^T \boldsymbol{f}(\boldsymbol{x}) (\boldsymbol{J}^T \boldsymbol{J})^{-1}
\f]

高斯牛顿法的好处就是不需要计算复杂的Hessian矩阵。

\section opt_lm 列文博格-马夸尔特方法（LM方法）

LM方法主要是为了限制近似带来的步长过长的现象，又被称为阻尼牛顿法。

\f[
    \rho = \frac{f(\boldsymbol{x}+\Delta\boldsymbol{x})-f(\boldsymbol{x})}{\boldsymbol{J}\Delta\boldsymbol{x}}
\f]

如果 \f$ \rho \f$接近于1，则认为近似得比较好，如果 \f$ \rho \f$很大，则实际下降要比近似下降要大，可以考虑增加信赖区域，反之，如果 \f$ \rho \f$很小，则实际下降要比近似的小，需要减小可信赖区域。

优化的步骤为：

1. 给定的初始值 \f$ \boldsymbol{x}_0 \f$，选取一个初始的优化半径 \f$ \mu \f$

2. 求解：
\f[
    \arg\min{\| \boldsymbol{f}(\boldsymbol{x}) + \boldsymbol{J}(\boldsymbol{x}_k)\Delta\boldsymbol{x}_k \|^2}, \quad \| \boldsymbol{D} \Delta\boldsymbol{x}_k \|^2 \leq \mu
\f]

3. 调整信赖区域，如果 \f$ \rho > \rho _2 \f$，则 \f$ \mu \to 2\mu \f$ ， 如果 \f$ \rho < \rho _1 \f$，则 \f$ \mu \to 0.5\mu \f$，其中， \f$ \rho _1 \f$和 \f$ \rho _2 \f$是需要手动设置的阈值。

4. 令 \f$ \boldsymbol{x}_{k+1} = \boldsymbol{x}_k + \Delta\boldsymbol{x}_k \f$

5. 判断是否收敛，不收敛则回到第二步。

列文伯格优化方法中，\f$ \boldsymbol{D} \f$ 取单位阵 \f$ \boldsymbol{I} \f$，即每一个分量维度限制尺度是相等的，我们自然会想到进一步让 \f$ \boldsymbol{D} \f$取一个对角阵，来给每一个分量不同的尺度。

