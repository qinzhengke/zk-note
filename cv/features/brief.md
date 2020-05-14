BRIEF描述子{#brief_descriptor}
========================

\section 基本方法
全称：Binary RobustIndependent Elementary Features，（有故意拼凑单词brief的嫌疑）。

step 1: 以关键点为中心选择一个区域S，在S中随机选择N个点对。

step 2: 比较点对里的两个点的灰度大小，如果前者大于后者，输出1，否则输出0。

step 3: 那么一共能够输出N个二进制数，即为特征描述子。

\section 点对的选择

作者试过5种方法：
1. x，y均匀分布
2. x，y高斯分布
3. x高斯分布，y以x为中心高斯分布
4. x和y在极坐标上随机选取
5. x固定在中心，y在极坐标上随机选取

\section 描述子相似度比较

由于是二进制的序列，相似度使用汉明距离表达，参考 \ref hamming_dist 。



参考文献：M Calonder, BRIEF:Binary Robust Independent Elementary Features, 2010
