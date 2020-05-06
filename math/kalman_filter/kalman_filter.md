卡尔曼滤波{#kalman_filter}
========================

<hr>
\section kf_usage 用法

预测状态：

\f[
\label{kf_1}
\hat{\textbf{x}}_{k|k-1} = \textbf{F}_{k}\hat{\textbf{x}}_{k-1|k-1} + \textbf{B}_{k} \textbf{u}_{k}
\f]

预测估计协方差矩阵
\f[
\textbf{P}_{k|k-1} =  \textbf{F}_{k} \textbf{P}_{k-1|k-1} \textbf{F}_{k}^{T} + \textbf{Q}_{k}
\f]

计算最优卡尔曼增益：

測量残差，measurement residual
\f[
\tilde{\textbf{y}}_{k} = \textbf{z}_{k} - \textbf{H}_{k}\hat{\textbf{x}}_{k|k-1} 
\f]

測量残差协方差
\f[
\textbf{S}_{k} = \textbf{H}_{k}\textbf{P}_{k|k-1}\textbf{H}_{k}^{T} + \textbf{R}_{k}
\f]

最优卡尔曼增益
\f[
\textbf{K}_{k} = \textbf{P}_{k|k-1}\textbf{H}_{k}^{T}\textbf{S}_{k}^{-1}
\f]

公式 \\eqref{kf_1} 为预测。

<hr>
\section kf_origin 推导：最大化后验概率

推导