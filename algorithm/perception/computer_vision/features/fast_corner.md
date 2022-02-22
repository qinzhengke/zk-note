FAST角点{#fast_corner}
======================

# 原理

<center>
![](./figures/fast_corner.png)
图：FAST角点原理示意图，图像来源为 \cite rosten2006machine 。
</center>

step 1: 以p点为中心，考虑半径为3的圆上，一共有16个点，编号为1到16。

step 2: 设定一个灰度阈值，在点{1,5,9,13}中，如果有3个点和中心点灰度差绝对值大于阈值，则定为候选点，否则直接排除。

step 3: 对于候选点，如果存在连续9个点和中心点灰度差绝对值大于阈值，则认为是角点。

step 4: 非最大值抑制，16个点与中心点灰度差绝对值之和为FAST得分，对于一个特征点，如果角点附近（3x3或者5x5）存在其他角点，则比较FAST得分，如果当前点不是最大的，则抑制，否则保留。

# 优缺点

优点：计算量小


1. http://www.edwardrosten.com/work/fast.html

2. E Rosten, Machine learning for high-speed corner detection, 2006
