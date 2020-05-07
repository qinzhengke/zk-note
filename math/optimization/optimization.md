优化理论{#optimization}
===========================

此页面是优化理论相关内容的入口。

\section jacobian 雅克比(Jacobian)矩阵

复习一下雅克比矩阵，注意雅克比矩阵中的行是函数分量，列是变量分量。

\f[
\mathbf J = \begin{bmatrix}
    \dfrac{\partial \mathbf{f}}{\partial x_1} & \cdots & \dfrac{\partial \boldsymbol{f}}{\partial x_n} \end{bmatrix}
= \begin{bmatrix}
    \dfrac{\partial f_1}{\partial x_1} & \cdots & \dfrac{\partial f_1}{\partial x_n}\\
    \vdots & \ddots & \vdots\\
    \dfrac{\partial f_m}{\partial x_1} & \cdots & \dfrac{\partial f_m}{\partial x_n} \end{bmatrix}

\f]

对于单值函数，那么雅克比矩阵实际上是一个行向量，如下所示。

\f[
\mathbf J = \begin{bmatrix}
    \dfrac{\partial f}{\partial x_1} & \cdots & \dfrac{\partial f}{\partial x_n} \end{bmatrix}
\f]

\section hessian 单值函数的海森(Hessian)矩阵

复习一下海森矩阵，只讨论单值函数，公式如下所示：

\f[
\mathbf{H} = \begin{bmatrix}\frac {\partial^2 f}{\partial x_1^2} & \frac{\partial^2 f}{\partial x_1\,\partial x_2} & \cdots & \frac{\partial^2 f}{\partial x_1\,\partial x_n} \\  \\
\frac{\partial^2 f}{\partial x_2\,\partial x_1} & \frac{\partial^2 f}{\partial x_2^2} & \cdots & \frac{\partial^2 f}{\partial x_2\,\partial x_n} \\  \\
\vdots & \vdots & \ddots & \vdots \\  \\
\frac{\partial^2 f}{\partial x_n\,\partial x_1} & \frac{\partial^2 f}{\partial x_n\,\partial x_2} & \cdots & \frac{\partial^2 f}{\partial x_n^2}
\end{bmatrix}\,
\f]

\section opt_gradient 梯度下降

梯度下降最简单，计算一阶导数，

\f[
    \boldsymbol{x}_{k} = \boldsymbol{J}^T \boldsymbol{x}_{k-1} 
\f]

\section opt_newton 牛顿法

\section opt_gausian_newton 高斯牛顿法

\section opt_lm 列文博格-马夸尔特方法
