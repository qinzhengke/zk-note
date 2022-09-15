# 视频库Vireo


## 介绍

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

## 基本概念

<b>demuxing</b> demuxing是demultiplexing的缩写，是指把包含多部分的视频给读取出来。
注意，一个movie文件包含了视频、音频、字幕等等。
不能先入为主的认为moive就只包含图像，movie的读取函数就应该直接得到图像。

## 编译问题
依赖lsmash库，lsmash库本身configure时需要加入参数“--enable-shared”，如下代码所示

```bash
cd lsmash
./configure --enable-shared
```

否则编译会出现以下问题，
```bash
/usr/bin/ld: //usr/local/lib/liblsmash.a(dts.o): relocation R_X86_64_PC32 against symbol `lsmash_remove_dts_reserved_box' can not be used when making a shared object; recompile with -fPIC
```

【问题】

```bash
./.libs/libvireo.so: undefined reference to `mp4a_export_AudioSpecificConfig'
./.libs/libvireo.so: undefined reference to `h264_setup_parser'
./.libs/libvireo.so: undefined reference to `h264_cleanup_parser'
./.libs/libvireo.so: undefined reference to `h264_parse_sps'
```

## 放弃

真 - 从入门到放弃

vireo和它的依赖lsmash都使用autoconf这些linux平台特有的构建工具，而且高度依赖gcc版本和linux发行版。

想要解决问题2，需要特定的gcc和linux发行版。

跨linux版本都出现问题，更别说跨平台了，放弃。
