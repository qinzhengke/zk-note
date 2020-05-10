卡尔曼滤波{#kalman_filter}
========================

<hr>
\section kf_usage 用法

预测状态：

\f[
\label{kf_1}
\hat{\boldsymbol{x}}_{k|k-1} = \boldsymbol{F}_{k}\hat{\boldsymbol{x}}_{k-1|k-1} + \boldsymbol{B}_{k} \boldsymbol{u}_{k}
\f]

预测估计协方差矩阵
\f[
\boldsymbol{P}_{k|k-1} =  \boldsymbol{F}_{k} \boldsymbol{P}_{k-1|k-1} \boldsymbol{F}_{k}^{T} + \boldsymbol{Q}_{k}
\f]

计算最优卡尔曼增益：

測量残差，measurement residual
\f[
\tilde{\boldsymbol{y}}_{k} = \boldsymbol{z}_{k} - \boldsymbol{H}_{k}\hat{\boldsymbol{x}}_{k|k-1} 
\f]

測量残差协方差
\f[
\boldsymbol{S}_{k} = \boldsymbol{H}_{k}\boldsymbol{P}_{k|k-1}\boldsymbol{H}_{k}^{T} + \boldsymbol{R}_{k}
\f]

最优卡尔曼增益
\f[
\boldsymbol{K}_{k} = \boldsymbol{P}_{k|k-1}\boldsymbol{H}_{k}^{T}\boldsymbol{S}_{k}^{-1}
\f]

更新：

更新的状态估计
\f[
\hat{\boldsymbol{x}}_{k|k} = \hat{\boldsymbol{x}}_{k|k-1} + \boldsymbol{K}_{k}\tilde{\boldsymbol{y}}_{k}
\f]

更新的协方差估计
\f[
\boldsymbol{P}_{k|k} =(I - \boldsymbol{K}_{k} \boldsymbol{H}_{k}) \boldsymbol{P}_{k|k-1}
\f]


<hr>
\section kf_origin 推导：最大化后验概率

推导