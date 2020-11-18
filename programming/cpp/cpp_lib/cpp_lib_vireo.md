视频库Vireo{#cpp_lib_vireo}
==========================

<hr>
\section 介绍

Vireo是Twitter团队开发维护的一个轻量级的、通用的视频处理库，基于C++11提供API接口。

【问题】为什么不使用ffmpeg？

ffmpeg确实是一个经典的代码库，而且支持的视频格式种类很多。
但是ffmpeg只提供C语言的API，而且在示例代码中使用goto，这让我不太喜欢。
相比之下，vireo提供了基于C++11的API和示例程序，更morden一些，一边使用vireo一边实战C++11，岂不美哉？
另外，vireo是Twitter团队开发维护的，同时被Github Awesome C++ project收录，质量应该是有保障的。

【问题】为什么不用OpenCV？

OpenCV读视频那完全不是主业，只是为了方便自身处理以视频作为来源的数据。
视频格式支持是较少的，而且不支持音频等。
并且OpenCV还是太重了，用起来不够短小精悍。

\section 基本概念

<b>demuxing</b> demuxing是demultiplexing的缩写，是指把包含多部分的视频给读取出来。
注意，一个movie文件包含了视频、音频、字幕等等。
不能先入为主的认为moive就只包含图像，movie的读取函数就应该直接得到图像。


