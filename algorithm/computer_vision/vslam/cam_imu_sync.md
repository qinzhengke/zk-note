相机和IMU数据的同步{#cam_imu_sync}
===============================

相机曝光的中间时刻和IMU采集时刻之间差 $$ \Delta t $$，是如何估计出来的？


# 实时估计时间差

文章：

[1] Qin T, Shen S. Online Temporal Calibration for Monocular Visual-Inertial Systems[J]. arXiv preprint arXiv:1808.00692, 2018.

把时间差一起放到优化框架里估计。