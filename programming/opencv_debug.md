OpenCV调试问题{#opencv_debug}
==========================

\section imshow导致crash

问题：非常简单的imshow代码都会导致crash，就crash在imshow的位置。

\code{.cpp}
Mat img(500,500,CV_8UC3);
img.setTo(Scalar(0xff,0xff,0xff));
imshow("window", img);
waitKey(0);
\endcode

可能的原因是opencv的依赖没有装好，有的时候不装依赖也能完成opencv的编译，但是运行的时候还是会出错，所以还是得按照教程一步一步走。

下面展示了opencv的必要的依赖项，从官网拷贝下来的。

\code{.sh}
sudo apt-get install cmake git libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev
\endcode

安装完这些依赖，然后重新编译安装opencv，再编译应用程序，就没有问题了。