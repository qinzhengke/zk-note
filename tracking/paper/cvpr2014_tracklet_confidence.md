Robust Online Multi-Object Tracking based on Tracklet Confidence and Online Discriminative Appearance Learning{#cvpr2014_tracklet_confidence}
=============

文章提出了：

1. 一种基于detecability和continuity的置信度计算方法。
2. 给出了基于置信度的多目标跟踪问题的数学表达，并且给出多种求解方法。
3. we also propose a novel online learning method using an incremental linear discriminant analysis for discriminating the appearances of objects.

\section Affinity

\subsection Appearance

Appearance Model

Discriminative appearance model

举例：使用颜色或者其他特征直方图

Appearance是和原始数据相关的，例如图像。

LDA，Linear Discriminative Analysis，可以分为batch的和incrmental的。

ILDA，Incremental Linear Discriminative Analysis，基于该方法，可以将高维度的特征投影到低维度的子空间。
高维向量转换为低维向量，而且是线性的，那么就是通过矩阵变换了。
而这个变换矩阵U是基于学习得来的。学习的loss是最大化类别的separability。

把每一个tracklet看成一个类，一个tracklet的历史数据作为样本，然后进行分类。

Discriminative projection space.

Local Association of Tracklets.

Global Association of tracklets.

\subsection Shape

\subsection motion_model Motion Model

Motion affinity，

Frame gap，