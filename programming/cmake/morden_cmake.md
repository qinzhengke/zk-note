# 现代cmake

cmake是一个历史悠久的工具，一直在反复迭代，很多特性已经被丢弃，甚至认为是不好的。
cmake近几年也在倡导更优雅的使用方式，即诞生了“现代cmake”，所以我们在使用cmake的时候，同一个目的往往有多种实现方法，每种方法多多少少有些不用，非常地混乱。
所以选择现代cmake使用方法成为了必要，一些非现代写法，就不再记录，以免带来记忆负担。

下面这篇文章详细说明了现代写法的细节。
https://ukabuer.me/blog/more-modern-cmake

# 现代cmake的版本

至少是3.12开始。

# 添加目录里所有源文件

\code{.cmake}
cmake_minimum_required(VERSION 3.12)
file(GLOB helloworld_SRC
    "*.h"
    "*.cpp"
)
add_executable(helloworld ${helloworld_SRC})
\endcode


# cmake_cpp11 C++11标准的引用

\code{cmake}
target_compile_features(my_exe PRIVATE cxx_std_11)
\endcode


# 库文件的一般基本引用方法

步骤1：使用find_package()
来查找相应的库。find_package依赖find.cmake或者config.cmake文件。
一般来说，如果库的作者也使用cmake来构建，那么一般make install的时候，config.cmake文件会被拷贝到
/usr/lib/cmake，或者/usr/local/lib/cmake，装在哪里取决于INSTALL时候的PREFIX参数。

如果库的作者没有提供config.cmake文件，则可以找第三方的Config.cmake文件，如果实在还是没有，可以自己编写。

步骤2：target_link_libraries()
链接的时候，需要找到依赖库的target名称，这个名称是一个坑点，它可能和库的名字不太一样，并且网上没有看到教我们如何找到target名称的资料，一般的教程上来就是授人以鱼，下次遇到别的库，还得再次搜索。

举个例子，引用ZeroMQ库，链接配置如下所示：

```
target_link_libraries(my_lib PRIVATE libzmq)
```

这个“libzmq”到底是怎么来的？以后引用别的库的时候应该如何查找？

这个“libzmq”是在ZeroMQTargets.cmake中定义的，如果将Targets.cmake中的target名改名成xxx，那么我们的target_link_libraries()填入xxx时，同样能够链接成功，请自行实验。

## 不好的方法

下面是最直白的引用方法，直接提供so文件的路径，但是这种写法很差，设想一下，当同事尝试编译我们的代码时，so文件的路径未必和我们的环境相同。
此时，使用者只能修改CMakeLists.txt文件。当同事在我们的基础上修改了代码，提交commit时，面对CMakeLists.txt中的so文件路径。
他面临两种选择：一是保留他的设置，把改动推到主干，这时候我们使用他更新后的代码就会出问题。二是他手动撤回他的本地改动，这样做很繁琐，当提交次数多的时候会带来负担，并且很容易忘记。
不论哪种选择，都不是最好的方法。

\code{cmake}
target_link_libraries(exe /usr/local/lib/xxx.so)
\# 一般情况下使用make install安装的库都文件都放在/usr/local/lib目录。
\endcode


# find_package引用Eigen3

Eigen3安装的时候，Eigen3Config.cmake文件实际上已经拷贝到系统中，但是该目录无法被cmake的find_package语句感知。
所以如果想要通过find_package来引用Eigen3，则仍然需要拷贝cmake文件，如下所示

    Eigen3安装（源码安装）后的目录：/usr/local/share/eigen3/cmake/Eigen3Config.cmake

    find_package默认目录：/usr/lib/cmake 或者 /usr/lib/local/cmake

#  链接OpenCV的库
\code{cmake}
target_link_libraroes(exe ${OpenCV_LIBS})
\endcode


# cmake_qt  引用Qt库
在引用带Qt编译的OpenCV的时候，需要告诉cmake工具Qt5Widgets的位置。
\code{cmake}
set(CMAKE_PREFIX_PATH /home/zrinker/softs/Qt5.10.1/5.10.1/gcc_64/lib/cmake/Qt5Widgets)
find_package(Qt5Widgets CONFIG REQUIRED)
\endcode


#  cmake_static_std_lib 静态链接标准库
在Linux下，如果不做静态连接，会发现甚至只依赖标准库的程序换个地方都不能运行。
下面是静态连接标准库的方法：
\code{cmake}
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libstdc++")
\endcode


# cmake_define  添加预定义宏
有些代码的使用需要使用者改动或者添加一些宏设置，但是作为用户又不想修改被调用代码，那么怎么在编译环境中加入预定义的宏定义呢？

cmake的答案如下所示
\code{bash}
# 3.14版本之后才能用
add_compile_definitions(exe MY_DEF=1)
\endcode


# cmake_src 添加源文件
aux_source_directories(. a)本来就是追加的形式，不需要额外操作。
\code{cmake}
aux_source_directories(. a)
aux_source_directories(.. a)
aux_source_directories(../.. a)
add_executable(exe_a ${a})
\endcode


# c99_cpp11 设置C99和C++11
cmake中设置了编译选项
\code{cmake}
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
\endcode
但是对应的Makefile里怎么都找不到对应的改动。

后来通过git来查看，才知道，CMake生成的Makefile中不直接包含flag，flag相关的设置都在CMakeFiles/xxx.dir/flags.make文件中。

这里不得不说git真是神器，让蛛丝马迹无处可藏。


# cmake_math 引用math.h的lib
首先，cmake建的工程，居然math.h的Lib要手动加。。。。
其次，我们知道gcc -lm是加math库，但是cmake怎么加都不对。
最后，cmake对于加math库有独特的表达式。。。

code{cmake}
target_link_libraries(exe_name, m)
\endcode

没错就是“m”，表示抽象的变量。


# cmake_install 安装
\code{bash}
\# 安装可执行文件
set(CMAKE_INSTALL_PREFIX ..)
install(TARGETS my_exe
        RUNTIME DESTINATION bin
      )
\# 安装一些配置文件
install(FILES xxx/config.txt
        DESTINATION bin
)
\endcode

注意，install函数里面的路径如果没有以”/“开头，那么都是以CMAKE_INSTALL_PREFIXX为起点的相对路径。

注意，CMAKE_INSTALL_PREFIX如果设置了相对路劲，那么这个相对路径的起点是cmake运行的地方，而不是CMakeLists.txt所在的地方。

注意，如果install函数所在脚本被更高一级的脚本包含，那么CMAKE_INSTALL_PREFIX一定要在最高一级的脚本设置，在install函数所在脚本设置是无效的。


# cmake_python 引用Python

前言：使用matplotlib for C++时，需要在cmake中引用Python。

使用3.11以前的版本，直接find_package(Python)是没用的，Python发布包里并不带有cmake config文件。

而3.11以后的版本，可以直接使用find_package(Python/Python2/Python3)，虽然Python官方仍然不提供cmake config文件，但是cmake主动做适配，提供了Python的Find文件。

下面给出引用Python2的实例：
\code{cmake}
find_package(Python2 COMPONENTS Development NumPy REQUIRED)

add_executable(opt_demo
    main.cpp)

target_include_directories(opt_demo PRIVATE ${Python2_INCLUDE_DIRS} ${Python2_NumPy_INCLUDE_DIRS})
target_link_libraries(opt_demo Python2::Python Python2::NumPy)
target_compile_features(opt_demo PRIVATE cxx_std_11)
\endcode


# cmake_find_package config文件和find文件。

CMake 对 Config file 的命名是有规定的，对于find_package(ABC)这样一条命令，CMake 只会去寻找ABCConfig.cmake或是abc-config.cmake。
CMake 默认寻找的路径和平台有关，在 Linux 下寻找路径包括/usr/lib/cmake以及/usr/lib/local/cmake，在这两个路径下可以发现大量的 Config File，一般在安装某个库时，其自带的 Config file 会被放到这里来。

注意，Config.cmake文件不能直接放在这些目录下，而是必须新建子目录，否则cmake无法识别，例如最终这样放置“/usr/lib/cmake/MyLib/MyLibConfig.cmake”
这点很坑，网上找不到说明，cmake官方文档太乱，找不到有用信息。

这里引用网络博客的一段话：
> Config file看似十分美好，由开发者编写CMake脚本，使用者只要能找到Config file即可获取到库的usage requirement。 但现实是，并不是所有的开发者都使用CMake，很多库并没有提供供CMake使用的Config file，但此时我们还可以使用Find file。

> 对于find_package(ABC)命令，如果CMake没有找到Config file，他还会去试着寻找FindABC.cmake。Find file在功能上和Config file相同，区别在于Find file是由其他人编写的，而非库的开发者。 如果你使用的某个库没有提供Config file，你可以去网上搜搜Find file或者自己写一个，然后加入到你的CMake工程中。

> 一个好消息是CMake官方为我们写好了很多Find file，在CMake Documentation这一页面可以看到，OpenGL，OpenMP，SDL这些知名的库官方都为我们写好了Find脚本，因此直接调用find_package命令即可。 但由于库的安装位置并不是固定的，这些Find脚本不一定能找到库，此时根据CMake报错的提示设置对应变量即可，通常是需要提供安装路径，这样就可以通过Find file获取到库的usage requirement。 不论是Config file还是Find file，其目的都不只是找到库这么简单，而是告诉CMake如何使用这个库。

参考 https://ukabuer.me/blog/more-modern-cmake


# 额外添加源文件

问题：我们都知道add_executalbe可以添加可执行文件目标，同时添加该目标需要的源文件。
有的时候我们需要add_executalbe语句之后再添加源文件，例如工程A在调试阶段需要编译可视化模块，需要可视化的源文件，但是发布出去就不需要。
所以我们会定义一个宏来区分什么时候该加入可视化模块，如果宏生效了，才额外添加源文件。

那么如何操作呢？

答案：

\code{cmake}
target_sources(my_exe PRIVATE my_src.c)
\endcode


# target_link_libraries中的相对路径

在cmake的target_link_libaraies语句中，想要link到某一个库文件，如果直接输入相对路径，那么不管怎么写，cmake都无法找到对应的库文件。

必须使用${CMAKE_BINARY_DIR}来指定当前路径，这个路径也就是我们通常新建的build目录。

\code{cmake}
target_link_libraries(my_exe
        ${CMAKE_BINARY_DIR}/3rd-party/libabc.so)
\endcode


# 如何在cmake中设置程序版本？

\code{.cmake}
set_target_properties(my_exe PROPERTIES 
        VERSION XXX)
\endcode

这样编译后的可执行文件或者库文件，都带有版本后缀，例如my_exe-1.0.0，如果是库文件，这是my_lib.so.1.0.0。


# 模块重复编译怎么办？
假设用A<-B表示模块A依赖模块B，那么假设我们有这种结构：A <- B <- C，　A <- C，cmake构建的时候会提示出现重复的模块．

C可能是一个很底层的模块，例如基础组件规范化打印．
不能说因为B依赖了C，A就不依赖C了，A和B依赖的C的版本有可能是不一样的，甚至可能有一天B突然不依赖C了。
所以我们仍然需要能“同时”编译的方法。

使用如下的语句，就可以防止提示重复模块．

\code{.cmake}
+if(NOT TARGET ha_util)
 add_subdirectory(3rd-party/ha_util)
+endif()
\endcode

# 依赖option

通过option我们可以进行选择编译，但是比起最基本的单个option，我们也会需要复杂一点的操作，例如BUILD_ALL选项和BUILD_A选项，两者只有有一个开，就编译A模块。

然而，选项是无法进行or操作的，cmake这蛋疼的设计。。。

\code{cmake}

option(BUILD_ALL OFF)
option(BUILD_A OFF)

if(BUILD_ALL or BUILD_A)    #cmake不认这样的语句
...
endif()

\endcode

为此，cmake提供了另一种解决方案，依赖option

\code{cpp}
include(CmakeDependentOption)   # 一定要加入，否则cmake不识别，官网的文档还没提到这点，不得不说cmake官方文档真的不友好。
option(BUILD_ALL OFF)
option(BUILD_A OFF)
cmake_dependent_option(BUILD_A_FINAL "Some description text" OFF "NOT BUILD_ALL; NOT BUILD_A" ON)
if(BUILD_A_FINAL)
endif()
\endcode

含义是，如果BUILD_ALL是OFF，BUILD_A也是OFF，那么BUILD_A_FINAL才是OFF，也就是说BUILD_A_FINAL = BUILD_ALL or BUILD_A
这种特殊写法很绕，而且看起来很像是临时补丁，特别的丑陋，对比rust家庭的cargo，完全不一样。


# 静态库


# IDE能编，terminal却找不到头文件？
明明头文件的路径已经添加，并且在**QtCreator**里能够跟踪到该头文件，但是在**terminal** make的时候仍然找不到头文件？
有一个原因：CMakeList.txt用了环境变量$ENV{}，并且QtCreator设置了这个环境变量，但是bash里面并没有设置！
反过来，能编译，但是QtCreator老是找不到头文件，或者在QtCreator内部构建失败。