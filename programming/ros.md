ros{#ros}
===========

此页面记录使用ROS过程中遇到的一些问题。


## QtCreator+cmake工程+ROS
QtCreator可以直接打开cmake工程，但是如果这个工程是以ros框架构建的，那就不那么直接了。
1. 使用终端打开qtcreator，并且在打开之前source一下workspace/devel/setup.bash，qtcreator要在catkin_ws目录启动，不能在其他目录下启动，例如catkin_ws/src。
2. 打开workspace/src/CMakeListstxt，不要打开子目录的CMakeLists.txt，会打不开的。
3.QtCreator中的projects页面，build directory写成catkin_ws/build，否则QtCreator无法解析package工程。## 

## 系统设置与工作目录设置冲突？
发现一个很棘手的问题，source workspace/devel/setup.bash之后，catkin_make就会找不到，$ROS_PACKAGE_PATH也会回到/opt/ros/kinetic/share，这时候需要手动添加工作目录作为寻包路径。
```.sh
export ROS_PACKAGE_PATH=/home/user/projects/catkin_ws:${ROS_PACKAGE_PATH}
```
