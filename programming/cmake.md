cmake{#cmake}
=============

<hr>
\section cmake_cpp11 C++11标准的引用
<!-- \section abc -->

方法一（3.11之后，现代写法，推荐）：
~~~{.cmake}
target_compile_features(my_exe PRIVATE cxx_std_11)
~~~

方法二（3.11以前，不推荐）：
~~~{.cmake}
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
~~~

\section  链接OpenCV的库
~~~{.cmake}
target_link_libraroes(exe ${OpenCV_LIBS})
~~~

\section cmake_qt  引用Qt库
在引用带Qt编译的OpenCV的时候，需要告诉cmake工具Qt5Widgets的位置。
~~~{.cmake}
set(CMAKE_PREFIX_PATH /home/zrinker/softs/Qt5.10.1/5.10.1/gcc_64/lib/cmake/Qt5Widgets)
find_package(Qt5Widgets CONFIG REQUIRED)
~~~

\section  cmake_static_std_lib 静态链接标准库
在Linux下，如果不做静态连接，会发现甚至只依赖标准库的程序换个地方都不能运行。
下面是静态连接标准库的方法：
~~~{.cmake}
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libstdc++")
~~~

\section cmake_define  添加预定义宏
有些代码的使用需要使用者改动或者添加一些宏设置，但是作为用户又不想修改被调用代码，那么怎么在编译环境中加入预定义的宏定义呢？

cmake的答案如下所示
~~~{.sh}
add_compile_definitions(exe MY_DEF=1)
~~~

\section cmake_src 添加源文件
aux_source_directories(. a)本来就是追加的形式，不需要额外操作。
```cmake
aux_source_directories(. a)
aux_source_directories(.. a)
aux_source_directories(../.. a)
add_executable(exe_a ${a})
```

\section c99_cpp11 设置C99和C++11
cmake中设置了编译选项
```cmake
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
```
但是对应的Makefile里怎么都找不到对应的改动。

后来通过git来查看，才知道，CMake生成的Makefile中不直接包含flag，flag相关的设置都在CMakeFiles/xxx.dir/flags.make文件中。

这里不得不说git真是神器，让蛛丝马迹无处可藏。

\section cmake_math 引用math.h的lib
首先，cmake建的工程，居然math.h的Lib要手动加。。。。
其次，我们知道gcc -lm是加math库，但是cmake怎么加都不对。
最后，cmake对于加math库有独特的表达式。。。
```camke
target_link_libraries(exe_name, m)
```
没错就是“m”，表示抽象的变量。

\section cmake_install 安装
~~~{.sh}
\# 安装可执行文件
set(CMAKE_INSTALL_PREFIX ..)
install(TARGETS my_exe
        RUNTIME DESTINATION bin
      )
\# 安装一些配置文件
install(FILES xxx/config.txt
        DESTINATION bin
)
~~~

注意，install函数里面的路径如果没有以”/“开头，那么都是以CMAKE_INSTALL_PREFIXX为起点的相对路径。

注意，CMAKE_INSTALL_PREFIX如果设置了相对路劲，那么这个相对路径的起点是cmake运行的地方，而不是CMakeLists.txt所在的地方。

注意，如果install函数所在脚本被更高一级的脚本包含，那么CMAKE_INSTALL_PREFIX一定要在最高一级的脚本设置，在install函数所在脚本设置是无效的。

\section cmake_python 引用Python

前言：使用matplotlib for C++时，需要在cmake中引用Python。

使用3.11以前的版本，直接find_package(Python)是没用的，Python发布包里并不带有cmake config文件。

而3.11以后的版本，可以直接使用find_package(Python/Python2/Python3)，虽然Python官方仍然不提供cmake config文件，但是cmake主动做适配，提供了Python的Find文件。

下面给出引用Python2的实例：
~~~{.cmake}
find_package(Python2 COMPONENTS Development NumPy REQUIRED)

add_executable(opt_demo
    main.cpp)

target_include_directories(opt_demo PRIVATE ${Python2_INCLUDE_DIRS} ${Python2_NumPy_INCLUDE_DIRS})
target_link_libraries(opt_demo Python2::Python Python2::NumPy)
target_compile_features(opt_demo PRIVATE cxx_std_11)
~~~

\section cmake_config_find config文件和find文件。

这里引用网络博客的一段话：
> Config file看似十分美好，由开发者编写CMake脚本，使用者只要能找到Config file即可获取到库的usage requirement。 但现实是，并不是所有的开发者都使用CMake，很多库并没有提供供CMake使用的Config file，但此时我们还可以使用Find file。

> 对于find_package(ABC)命令，如果CMake没有找到Config file，他还会去试着寻找FindABC.cmake。Find file在功能上和Config file相同，区别在于Find file是由其他人编写的，而非库的开发者。 如果你使用的某个库没有提供Config file，你可以去网上搜搜Find file或者自己写一个，然后加入到你的CMake工程中。

> 一个好消息是CMake官方为我们写好了很多Find file，在CMake Documentation这一页面可以看到，OpenGL，OpenMP，SDL这些知名的库官方都为我们写好了Find脚本，因此直接调用find_package命令即可。 但由于库的安装位置并不是固定的，这些Find脚本不一定能找到库，此时根据CMake报错的提示设置对应变量即可，通常是需要提供安装路径，这样就可以通过Find file获取到库的usage requirement。 不论是Config file还是Find file，其目的都不只是找到库这么简单，而是告诉CMake如何使用这个库。

参考 https://ukabuer.me/blog/more-modern-cmake
