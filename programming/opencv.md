OpenCV{#opencv}
==================

此页面记录opencv使用过程中遇到的问题。

## Linux离线环境编译opencv
cmake会在编译opencv时下载ippicv和ffmpeg库，这会导致离线环境编译opencv被卡在下述语句。
```{.sh}
IPPICV:Download:ippicv_2017u3_lnx_intel64_general_20180518.tgz
```

解法：
setp1:在github上搜索opencv/opencv_3rdparty，然后在文件分支下拉框中选择“ippicv/xxx”，否则默认分支是不会显示任何文件的（这个很坑）。
step2:下载对应版本的包，然后打开opencv/3rdparty/ippicv/ippicv.cmake，替换其中的下载路径，然后重新运行cmake即可

```{.cmake}
"https://raw.githubusercontent.com/opencv/opencv_3rdparty/${IPPICV_COMMIT}/ippicv/"
#改成
"file:///home/zrinker/.../ippicv....tar.gz"
```

## 编译contrib库
如果要使用contrib库，就需要自己行编译，编译时指定opencv_contrib/modules路径即可
~~~{.cmake}
set(OPENCV_EXTRA_MODULES_PATH ~/opencv_contrib/modules)
~~~

## 带Qt编译
带Qt编译后，imshow时opencv会调用更高级的一个gui，还是比较方便的。
step1:在cmake-gui中搜索WITH-QT，勾上，然后重新configure；
step2:步骤一会弹出错误并且将接下来要设置的几个路径都高亮显示， 大概是Qt5Concurrent_DIR、QtCore_DIR、Qt5Gui_DIR等等，设置到对应cmake路径，这里再举个例子
~~~{.cmake}
~/Qt5.11.2/gcc_64/lib/cmake/Qt5Concurrent
~~~
其他几个路径举一反三即可。