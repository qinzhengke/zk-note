cmake{#cmake}
===========================

## 使用c++11
~~~{.cmake}
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
~~~

## 链接OpenCV的库
~~~{.cmake}
target_link_libraroes(exe ${OpenCV_LIBS})
~~~

## 调用Qt库
在引用带Qt编译的OpenCV的时候，需要告诉cmake工具Qt5Widgets的位置。
~~~{.cmake}
set(CMAKE_PREFIX_PATH /home/zrinker/softs/Qt5.10.1/5.10.1/gcc_64/lib/cmake/Qt5Widgets)
find_package(Qt5Widgets CONFIG REQUIRED)
~~~

## 静态链接标准库
在Linux下，如果不做静态连接，会发现甚至只依赖标准库的程序换个地方都不能运行。
下面是静态连接标准库的方法：
~~~{.cmake}
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libstdc++")
~~~
