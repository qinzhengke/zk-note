# 估计理论综述（Estimation Theory）

＂估计理论(Estimation Theory)＂这个词来源于\cite wiki:estimation_theory。

个人认为，不同领域中包含了不同的估计对象，例如

- 概率论中的随机变量本身的估计
- 随机变量的相关属性（期望、方差）的估计
- 非随机变量

举一个关于最后一点的例子，在最大似然估计中，待估计的对象是模型的参数，比如高斯分布的 $$ \mu $$ 和 $$ \sigma $$，这些参数并没有被看成随机变量，而是固定的、未知的参数。
虽然，在贝叶斯估计中，这些参数也被看成随机变量。

- [mle

- [bayesian_estimation

- [bayesian_filter

- [kalman_filter

- [biased_kmf

- [ekf_entry

- [ukf_entry

- [particle_filter