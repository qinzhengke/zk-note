视觉SLAM简介{#vslam_entry}
====================

- [vlsam_intro

SLAM是“Simultaneous Localization And Mapping”的缩写，VSLAM也就是视觉SLAM。

此页面记录VSLAM相关知识。

- [cam_imu_sync

# 主流vslam框架

特征点法

| 一级分类 | 二级分类 | 名称 | 时间 | 状态量 | 特点 | 初始化| 缺陷 |
|---|---|---|---|---|---|---|---|
| 特征点法 | 基于滤波 | MonoSLAM | 2003 | 6DoF相机位姿和3DoF特征点位姿 | 基于滤波方法的代表，使用EKF作为后端优化方法 | 必须要给定一个已知物体进行地图（尺度？）的初始化 | 没有对场景特征点数量做限制，大场景下计算复杂度很高 |
| | 基于优化 | PTAM | 2007 | 6DoF相机位姿和3DoF特征点位姿 | 最先把tracking和mapping线程分开，最先引入key frame策略，最先引入BA作为后端，随机树搜索 | 五点法 | |
| | | ORB-SLAM | 2015 | 7DoF相机位姿（尺度)和3DoF特征点位姿 | 使用ORB特征点，完整度高，支持RGBD相机 | | |
| 直接法 | | DTAM | 2011 | 相机位姿+稠密深度图 | 使用PTAM作为前端tracking，使用多视几何计算稠密深度 |  |  |
| | | LSD-SLAM | 2014 | 相机位姿+半稠密深度图 | 对梯度强的点进行深度估计 | | |
| | | SVO | 2014 | | | | |
| | | DSO | 2016 | | 没有后端，VO only | | |

BA vs EKF?

直接法 vs 特征点法
特征点法不适合若纹理场景、曲线纹理场景

# loop_closing Loop closing

1. 位姿图优化(Pose-graph optimization)

2. 词袋模型

photometric consistency

- [vins

- [orb_slam

- [msckf

- [msckf_ext_reading
