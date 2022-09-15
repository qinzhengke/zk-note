# OpenCV：经典开源计算机视觉库{#cpp_lib_opencv}

此页面记录opencv使用过程中遇到的问题。


## Linux离线环境编译opencv
cmake会在编译opencv时下载ippicv和ffmpeg库，这会导致离线环境编译opencv被卡在下述语句。
```{bash}
IPPICV:Download:ippicv_2017u3_lnx_intel64_general_20180518.tgz
```

解法：
setp1:在github上搜索opencv/opencv_3rdparty，然后在文件分支下拉框中选择“ippicv/xxx”，否则默认分支是不会显示任何文件的（这个很坑）。
step2:下载对应版本的包，然后打开opencv/3rdparty/ippicv/ippicv.cmake，替换其中的下载路径，然后重新运行cmake即可

```cmake
"https://raw.githubusercontent.com/opencv/opencv_3rdparty/${IPPICV_COMMIT}/ippicv/"
#改成
"file:///home/zrinker/.../ippicv....tar.gz"
```


## 编译contrib库
如果要使用contrib库，就需要自己行编译，编译时指定opencv_contrib/modules路径即可
```cmake
set(OPENCV_EXTRA_MODULES_PATH ~/opencv_contrib/modules)
```


## 带Qt编译
带Qt编译后，imshow时opencv会调用更高级的一个gui，还是比较方便的。
step1:在cmake-gui中搜索WITH-QT，勾上，然后重新configure；
step2:步骤一会弹出错误并且将接下来要设置的几个路径都高亮显示， 大概是Qt5Concurrent_DIR、QtCore_DIR、Qt5Gui_DIR等等，设置到对应cmake路径，这里再举个例子
```cmake
~/Qt5.11.2/gcc_64/lib/cmake/Qt5Concurrent
```
其他几个路径举一反三即可。


## CV_EXPORT宏

```cpp
#if (defined _WIN32 || defined WINCE || defined __CYGWIN__) && defined CVAPI_EXPORTS
#  define CV_EXPORTS __declspec(dllexport)
#elif defined __GNUC__ && __GNUC__ >= 4
#  define CV_EXPORTS __attribute__ ((visibility ("default")))
#else
#  define CV_EXPORTS
#endif
```

那么，visibility属性又是什么呢？下面的网页解释的很好。

> https://www.ibm.com/developerworks/cn/aix/library/au-aix-symbol-visibility/index.html


## 如何填充Mat？

```cpp
Mat a(100,100,CV_8UC3);
a.setTo(Scalar(0xff,0xff,0xff)) // 填充成白色
```
## imshow_crash imshow导致crash

问题：非常简单的imshow代码都会导致crash，就crash在imshow的位置。

```cpp
Mat img(500,500,CV_8UC3);
img.setTo(Scalar(0xff,0xff,0xff));
imshow("window", img);
waitKey(0);
```

可能的原因是opencv的依赖没有装好，有的时候不装依赖也能完成opencv的编译，但是运行的时候还是会出错，所以还是得按照教程一步一步走。

下面展示了opencv的必要的依赖项，从官网拷贝下来的。

```bash
sudo apt-get install cmake git libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev
```

安装完这些依赖，然后重新编译安装opencv，再编译应用程序，就没有问题了。


## VideoWriter无法打开文件

VideoWriter无法打开文件，一些常规原因：

1. 目录没有写的权限
2. 磁盘满了
3. 目录不存在

这些原因不值得用专门记下来，这里记录另外一个不寻常的原因：OpenCV的依赖没有安装好。

解决方法参考 \ref imshow_crash