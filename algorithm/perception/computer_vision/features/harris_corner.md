# Harris角点

## 基本思想

想象一个小窗口在图像中滑动，如果窗口沿着所有方向都有比较大的灰度变化，那么我们认为这是一个角点。

反过来如果沿着任何方向灰度变化都比较小，那么我们认为该区域是平坦的，不存在角点。

如果窗口沿着某一个方向变化很大，但是朝着该方向的垂直方向变化很小，则认为遇到了边缘。

在图像 $$ (x,y) $$处进行探索，并计算自相似性。

$$
    c(x,y,\Delta x, \Delta y) = \sum _{(u,v) \in W} {w(u,v)[I(u,v)-I(u+\Delta x, v+\Delta y)]^2}
$$

对于图像 $$ (x+\Delta x, y+\Delta y) $$处基于 $$ (x,y) $$点进行泰勒展开有

$$
    I(x+\Delta x, y+\Delta y) = I(x,y) + \dfrac{\partial I}{\partial x} \Delta x + \dfrac{\partial I}{\partial y} \Delta y + O(\Delta x^2, \Delta y^2)
$$

进行一阶近似，可得

$$
    \begin{aligned}
    c(x,y,\Delta x, \Delta y) & \approx \sum _{(u,v) \in W} {w(u,v)[I_x \Delta x + I_y \Delta y]^2} \\
    & = [\Delta x \quad \Delta y] \left\{ \sum _{(u,v) \in W} w(u,v) \begin{bmatrix} I{_x}^2 & I_x I_y \\
    I_x I_y & I{_y}^2
    \end{bmatrix} \right\}
    \begin{bmatrix}
    \Delta x \\
    \Delta y
    \end{bmatrix}
    \end{aligned}
$$

定义
$$
    M = \sum_{(u,v) \in W} w(u,v) \begin{bmatrix} I{_x}^2 & I_x I_y \\
    I_x I_y & I{_y}^2
    \end{bmatrix}
     = \begin{bmatrix} \sum_{(u,v) \in W}{w(u,v) I{_x}^2} & \sum_{(u,v) \in W} w(u,v) I_x I_y \\
    \sum_{(u,v) \in W} w(u,v) I_x I_y & \sum_{(u,v) \in W} w(u,v)  I{_y}^2
    \end{bmatrix}
$$
