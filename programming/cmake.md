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

## 添加预定义宏
有些代码的使用需要使用者改动或者添加一些宏设置，但是作为用户又不想修改被调用代码，那么怎么在编译环境中加入预定义的宏定义呢？

cmake的答案如下所示
~~~{.sh}
add_compile_definitions(exe MY_DEF=1)
~~~

### cmake 添加源文件
aux_source_directories(. a)本来就是追加的形式，不需要额外操作。
```cmake
aux_source_directories(. a)
aux_source_directories(.. a)
aux_source_directories(../.. a)
add_executable(exe_a ${a})
```

### 设置 C99和C++11
cmake中设置了编译选项
```cmake
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
```
但是对应的Makefile里怎么都找不到对应的改动。

后来通过git来查看，才知道，CMake生成的Makefile中不直接包含flag，flag相关的设置都在CMakeFiles/xxx.dir/flags.make文件中。

这里不得不说git真是神器，让蛛丝马迹无处可藏。

### cmake 工程引入math.h的lib
首先，cmake建的工程，居然math.h的Lib要手动加。。。。
其次，我们知道gcc -lm是加math库，但是cmake怎么加都不对。
最后，cmake对于加math库有独特的表达式。。。
```camke
target_link_libraries(exe_name, m)
```
没错就是“m”，表示抽象的变量。

