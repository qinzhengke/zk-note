ros{#ros}
===========

此页面记录使用ROS过程中遇到的一些问题。


## QtCreator+cmake工程+ROS
QtCreator可以直接打开cmake工程，但是如果这个工程是以ros框架构建的，那就不那么直接了。
1. 使用终端打开qtcreator，并且在打开之前source一下workspace/devel/setup.bash。
2. 打开workspace/src/CMakeListstxt，不要打开子目录的CMakeLists.txt，会打不开的。
