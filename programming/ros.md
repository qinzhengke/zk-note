ros{#ros}
===========

此页面记录使用ROS过程中遇到的一些问题。


## QtCreator+cmake工程+ROS
QtCreator可以直接打开cmake工程，但是如果这个工程是以ros框架构建的，那就不那么直接了。
1. 使用终端打开qtcreator，并且在打开之前source一下workspace/devel/setup.bash，qtcreator要在catkin_ws目录启动，不能在其他目录下启动，例如catkin_ws/src。
2. 打开workspace/src/CMakeListstxt，不要打开子目录的CMakeLists.txt，会打不开的。
3.QtCreator中的projects页面，build directory写成catkin_ws/build，否则QtCreator无法解析package工程。## 

## 系统设置与工作目录设置冲突？
发现一个很棘手的问题，source workspace/devel/setup.bash之后，catkin_make就会找不到。
当前的可用方法是：在~/.bashrc里只写source系统的setup.bash，每打开一个terminal，删除build文件夹，回到catkin_ws运行catkin_make，然后再source devel/setup.bash。


$ROS_PACKAGE_PATH也会回到/opt/ros/kinetic/share，这时候需要手动添加工作目录作为寻包路径。
```.sh
export ROS_PACKAGE_PATH=/home/user/projects/catkin_ws:${ROS_PACKAGE_PATH}
```

根本原因：
打开terminal后先source了devel目录下的setup.sh，然后source系统目录的setup.sh。
只要一开始顺序搞反了，这两个source操作就会相互排斥，即如果source devel/setup.bash，catkin_make就无法找到，如果source opt/ros/kinetic/setup.bash，当前工作空间就无法被catkin_make正确编译，开发者的package也无法找到，相应的launch文件也无法使用。

解决的根本方法是：打开terminal之后，一定要先source系统setup.bash，然后再source工作空间的setup.bash，一旦顺序搞反了，必须重新启动terminal。

## QtCreator工程为空？
QtCreator打开CMakeLists.txt之后，虽然自动cmake运行没问题，但是project目录树显示为空，无法编辑任何代码。
方法？：
再source一遍devel/setup.bash,
build选项选择Qt默认的，也就是Desktop Qt5.11.2 GCC 64bit。
build目录选择catkin_ws/build


## Clock skew detected. Your build may be incomplete.
```.sh
caktin_make clean
catkin_make
```

## 断点调试
就像使用cmake一样使用catkin_make，
```sh
catkin_make -DCMAKE_BUILD_TYPE=Debug
```


