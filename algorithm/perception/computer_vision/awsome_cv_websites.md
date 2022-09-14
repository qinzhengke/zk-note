# 有趣的CV相关的网站

# cv_forum 论坛

| 名称 | 地址 | 说明 |
|---|---|---|
| 泡泡机器人 | http://paopaorobot.org/bbs/ | 非常经典的SLAM入门论坛，过一遍论坛公开课，可以快速了解SLAM的大部分基础概念 | 
| 四元数在线demo | https://quaternions.online/ | 3D展示四元数和欧拉角的变换关系 |
| CrazePony开源四轴飞行器 | http://www.crazepony.com/book | 开源四旋翼飞行器原理 |
| Middlebury |（米德尔伯里）学院提供的关于立体视觉的一个网站，提供一些标准双目数据benchmark，以及在线算法评价方法，目前该网站已经记录了一百多种双目匹配方法，并且进行了排序。


# 一些电子图书：

- Computer Vision Algorithms and Applications，作者Richard Szeliski

> http://szeliski.org/Book/

这本书讲述了计算机视觉综述性的内容，但是很新，很多近几年的东西都能找得到。

- Multiple View geometry in Computer Vision，作者Richard Hartley

这本书主要讲双目视觉中的一些几何问题，图像部分没有讲述。

- 《计算机视觉中的数学方法》，作者吴褔朝

一些数学和算法基础，非常有必要通读一遍。

# pdf课件

> Stereo Vision: Algorithms and Applications 作者：Stefano Mattoccia, University of bologna （博洛尼亚大学，意大利）

长达208页的课件包含了很多Stereo Vision的基础知识。

# 一些网站

http://vision.middlebury.edu/stereo/


Liulina 603
> http://blog.csdn.net/liulina603/article/details/53302168#comments

这篇博文介绍了双目匹配方法的原理，包括SAD匹配，BM算法、SGBM算法以及GC算法，有图有代码，很好理解，虽然很Navie。

## 一些paper

- GCPR 2014年的一篇文章，提供了一个带有非常精准的grouth truth的数据集，使用结构光系统实现，效果非常惊艳，被收录在Middlebury中。

> High-Resolution Stereo Datasets with Subpixel-Accurate Ground Truth.

## 一些基础名词

**homologous**：同源的，例如左右两图中的两个点是同源的，表示它们对应的三维空间点是同一个。

**stereo rig**：双目系统，双目装备，rig是装备的意思。

**DSLR camera**：Digital Single Len Reflex camera，数码单镜头反光相机，也就是常说的单反。

**round to integer**：转换成整数，rounding a numerical value是一个数学上的专用词汇，意思是将高精度的数转换成低精度的表达，过程可能会丢失精度。

**ambient**：

**KLT**：Kanade-Lucas-Tomasi目标跟踪算法，主要基于光流法。
