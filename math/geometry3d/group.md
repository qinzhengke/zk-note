特殊正交群及其李代数{#so3}
==================

\section 群

群(Group)是一种集合加上一种运算的代数结构。

<b> 特殊正交群SO(3)</b>

\f[
    SO(3) = \left\{ \boldsymbol{R} \in \mathbb{R}^{3 \times 3} | \boldsymbol{R}\boldsymbol{R}^T = \boldsymbol{I},
    \det(\boldsymbol{R})=1 \right\}
\f]

<b> 特殊欧式群 </b>

\f[
    SE(3) = \left\{ \boldsymbol{T} = \begin{bmatrix} 
    \boldsymbol{R} & \boldsymbol{t} \\
    \boldsymbol{0}^T & 1
    \end{bmatrix} \in \mathbb{R}^{4 \times 4} | \boldsymbol{R} \in SO(3), \boldsymbol{t} \in \mathbb{R}^3
      \right\}
\f]

\section 矩阵指数

矩阵指数的定义为：

\f[
    \exp(\boldsymbol{X}) = \sum_{k=0}^{\infty}{\frac{1}{k!} \boldsymbol{X}^k}
\f]

指数矩阵有如下性质：

\f[
    \exp(\boldsymbol{0}) = \boldsymbol{I}
\f]

关于指数矩阵的微分方程求解：
\f[
    \frac{d\boldsymbol{y}(t)}{dt} = \boldsymbol{A} \boldsymbol{y}(t), \quad \boldsymbol{y}(0) = \boldsymbol{y}_0
\f]

\f[
    \boldsymbol{y}(t) = \exp(\boldsymbol{A}t) \boldsymbol{y}_0
\f]

\section 李群
李群是指具有连续光滑性质的群。