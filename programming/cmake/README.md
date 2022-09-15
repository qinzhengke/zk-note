# cmake：C和C++的跨平台构建工具

## 一个典型的CMakeLists.txt

```cmake
# 现代cmake版本
cmake_minimum_required(VERSION 3.12)

# 使用文件夹名称作为project名称
get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" PROJECT_NAME ${PROJECT_NAME})
project(${PROJECT_NAME})

# 使用cmake定义版本号
set(MAJOR_VER 1)
set(MINOR_VER 0)
set(PATCH_VER 0)
set(FULL_VERSION "${MAJOR_VER}.${MINOR_VER}.${PATCH_VER}")

# 设置默认版本为debug
if(NOT CMAKE_BUILD_TYPE STREQUAL "Release")
message("=> cmake ${PROJECT_NAME} in Debug mode")
add_compile_options(-O0 -g -ggdb)
else()
message("=> cmake ${PROJECT_NAME} in Release mode")
endif()

# 构建example开关
option(BUILD_WITH_EXAMPLE ON)
if(BUILD_WITH_EXAMPLE)
# 构建example子目录
add_subdirectory(3rd-party/module_a)
endif()

# 编译某个目录总的所有源文件
file(GLOB my_SRC
    "src/*.cc"
)

# 新增目标静态lib，小工程，项目名称即为目标名称
add_library(${PROJECT_NAME}
    ${my_SRC}
)

# 目标包含路径，现代写法
target_include_directories(${PROJECT_NAME} PUBLIC
    3rd-party/module_a/include
)

# 目标链接库
target_link_libraries(${PROJECT_NAME}
    module_a
)

# 目标编译宏定义，现代写法
target_compile_definitions(${PROJECT_NAME} PUBLIC SOME_MACRO=1)

# 目标编译的C++标准版本
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_11)

# 使用cmake定义版本号
set_target_properties(${PROJECT_NAME} PROPERTIES
    VERSION ${FULL_VERSION})

```

## CMakeLists.txt误定义了宏

add_compile_definitions错误的把exe文件名加入了，实际上这个配置函数不需要指定可执行文件，所以my_exe被错误定义为宏，所以后面用到my_exe类型的变量、命名空间都会报错。

```
add_compile_definitions(DEF=1)  // 正确
add_compile_definitions(my_exe DEF=1)  //错误
```


## 单步调试自动调到程序结束

这种问题很诡异，非常地不合理，有一种可能：构建系统紊乱。
我们经常会使用git在不同分支之间切换，而build目录一般被我们ignore掉，所以很多别的分之的obj文件或者so文件被保存了下来，但是实际上是和我们当前的版本是不一致的。
因为obj文件和代码行号不对应，这就会导致单步调试错误地跳转。

解决方法：清除build目录重新编译。

## 现代cmake

cmake是一个历史悠久的工具，一直在反复迭代，很多特性已经被丢弃，甚至认为是不好的。
cmake近几年也在倡导更优雅的使用方式，即诞生了“现代cmake”，所以我们在使用cmake的时候，同一个目的往往有多种实现方法，每种方法多多少少有些不用，非常地混乱。
所以选择现代cmake使用方法成为了必要，一些非现代写法，就不再记录，以免带来记忆负担。

下面这篇文章详细说明了现代写法的细节。
https://ukabuer.me/blog/more-modern-cmake

## 现代cmake的版本

至少是3.12开始。

## 添加目录里所有源文件

```cmake
cmake_minimum_required(VERSION 3.12)
file(GLOB helloworld_SRC
    "*.h"
    "*.cpp"
)
add_executable(helloworld ${helloworld_SRC})
```


## cmake_cpp11 C++11标准的引用

```cmake
target_compile_features(my_exe PRIVATE cxx_std_11)
```


## 库文件的一般基本引用方法

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

```cmake
target_link_libraries(exe /usr/local/lib/xxx.so)
\# 一般情况下使用make install安装的库都文件都放在/usr/local/lib目录。
```


## find_package引用Eigen3

Eigen3安装的时候，Eigen3Config.cmake文件实际上已经拷贝到系统中，但是该目录无法被cmake的find_package语句感知。
所以如果想要通过find_package来引用Eigen3，则仍然需要拷贝cmake文件，如下所示

    Eigen3安装（源码安装）后的目录：/usr/local/share/eigen3/cmake/Eigen3Config.cmake

    find_package默认目录：/usr/lib/cmake 或者 /usr/lib/local/cmake

##  链接OpenCV的库
```cmake
target_link_libraroes(exe ${OpenCV_LIBS})
```


## cmake_qt  引用Qt库
在引用带Qt编译的OpenCV的时候，需要告诉cmake工具Qt5Widgets的位置。
```cmake
set(CMAKE_PREFIX_PATH /home/zrinker/softs/Qt5.10.1/5.10.1/gcc_64/lib/cmake/Qt5Widgets)
find_package(Qt5Widgets CONFIG REQUIRED)
```


##  cmake_static_std_lib 静态链接标准库
在Linux下，如果不做静态连接，会发现甚至只依赖标准库的程序换个地方都不能运行。
下面是静态连接标准库的方法：
```cmake
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libstdc++")
```


## cmake_define  添加预定义宏
有些代码的使用需要使用者改动或者添加一些宏设置，但是作为用户又不想修改被调用代码，那么怎么在编译环境中加入预定义的宏定义呢？

cmake的答案如下所示
```bash
# 3.14版本之后才能用
add_compile_definitions(exe MY_DEF=1)
```


## cmake_src 添加源文件
aux_source_directories(. a)本来就是追加的形式，不需要额外操作。
```cmake
aux_source_directories(. a)
aux_source_directories(.. a)
aux_source_directories(../.. a)
add_executable(exe_a ${a})
```


## c99_cpp11 设置C99和C++11
cmake中设置了编译选项
```cmake
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
```
但是对应的Makefile里怎么都找不到对应的改动。

后来通过git来查看，才知道，CMake生成的Makefile中不直接包含flag，flag相关的设置都在CMakeFiles/xxx.dir/flags.make文件中。

这里不得不说git真是神器，让蛛丝马迹无处可藏。


## cmake_math 引用math.h的lib
首先，cmake建的工程，居然math.h的Lib要手动加。。。。
其次，我们知道gcc -lm是加math库，但是cmake怎么加都不对。
最后，cmake对于加math库有独特的表达式。。。

```cmake
target_link_libraries(exe_name, m)
```

没错就是“m”，表示抽象的变量。


##  安装

```bash
# 安装可执行文件
set(CMAKE_INSTALL_PREFIX ..)
install(TARGETS my_exe
        RUNTIME DESTINATION bin
      )
# 安装一些配置文件
install(FILES xxx/config.txt
        DESTINATION bin
)
```

注意，install函数里面的路径如果没有以”/“开头，那么都是以CMAKE_INSTALL_PREFIXX为起点的相对路径。

注意，CMAKE_INSTALL_PREFIX如果设置了相对路劲，那么这个相对路径的起点是cmake运行的地方，而不是CMakeLists.txt所在的地方。

注意，如果install函数所在脚本被更高一级的脚本包含，那么CMAKE_INSTALL_PREFIX一定要在最高一级的脚本设置，在install函数所在脚本设置是无效的。


## cmake_python 引用Python

前言：使用matplotlib for C++时，需要在cmake中引用Python。

使用3.11以前的版本，直接find_package(Python)是没用的，Python发布包里并不带有cmake config文件。

而3.11以后的版本，可以直接使用find_package(Python/Python2/Python3)，虽然Python官方仍然不提供cmake config文件，但是cmake主动做适配，提供了Python的Find文件。

下面给出引用Python2的实例：
```cmake
find_package(Python2 COMPONENTS Development NumPy REQUIRED)

add_executable(opt_demo
    main.cpp)

target_include_directories(opt_demo PRIVATE ${Python2_INCLUDE_DIRS} ${Python2_NumPy_INCLUDE_DIRS})
target_link_libraries(opt_demo Python2::Python Python2::NumPy)
target_compile_features(opt_demo PRIVATE cxx_std_11)
```


## cmake_find_package config文件和find文件。

CMake 对 Config file 的命名是有规定的，对于find_package(ABC)这样一条命令，CMake 只会去寻找ABCConfig.cmake或是abc-config.cmake。
CMake 默认寻找的路径和平台有关，在 Linux 下寻找路径包括/usr/lib/cmake以及/usr/lib/local/cmake，在这两个路径下可以发现大量的 Config File，一般在安装某个库时，其自带的 Config file 会被放到这里来。

注意，Config.cmake文件不能直接放在这些目录下，而是必须新建子目录，否则cmake无法识别，例如最终这样放置“/usr/lib/cmake/MyLib/MyLibConfig.cmake”
这点很坑，网上找不到说明，cmake官方文档太乱，找不到有用信息。

这里引用网络博客的一段话：
> Config file看似十分美好，由开发者编写CMake脚本，使用者只要能找到Config file即可获取到库的usage requirement。 但现实是，并不是所有的开发者都使用CMake，很多库并没有提供供CMake使用的Config file，但此时我们还可以使用Find file。

> 对于find_package(ABC)命令，如果CMake没有找到Config file，他还会去试着寻找FindABC.cmake。Find file在功能上和Config file相同，区别在于Find file是由其他人编写的，而非库的开发者。 如果你使用的某个库没有提供Config file，你可以去网上搜搜Find file或者自己写一个，然后加入到你的CMake工程中。

> 一个好消息是CMake官方为我们写好了很多Find file，在CMake Documentation这一页面可以看到，OpenGL，OpenMP，SDL这些知名的库官方都为我们写好了Find脚本，因此直接调用find_package命令即可。 但由于库的安装位置并不是固定的，这些Find脚本不一定能找到库，此时根据CMake报错的提示设置对应变量即可，通常是需要提供安装路径，这样就可以通过Find file获取到库的usage requirement。 不论是Config file还是Find file，其目的都不只是找到库这么简单，而是告诉CMake如何使用这个库。

参考 https://ukabuer.me/blog/more-modern-cmake


## 额外添加源文件

问题：我们都知道add_executalbe可以添加可执行文件目标，同时添加该目标需要的源文件。
有的时候我们需要add_executalbe语句之后再添加源文件，例如工程A在调试阶段需要编译可视化模块，需要可视化的源文件，但是发布出去就不需要。
所以我们会定义一个宏来区分什么时候该加入可视化模块，如果宏生效了，才额外添加源文件。

那么如何操作呢？

答案：

```cmake
target_sources(my_exe PRIVATE my_src.c)
```


## target_link_libraries中的相对路径

在cmake的target_link_libaraies语句中，想要link到某一个库文件，如果直接输入相对路径，那么不管怎么写，cmake都无法找到对应的库文件。

必须使用${CMAKE_BINARY_DIR}来指定当前路径，这个路径也就是我们通常新建的build目录。

```cmake
target_link_libraries(my_exe
        ${CMAKE_BINARY_DIR}/3rd-party/libabc.so)
```


## 如何在cmake中设置程序版本？

```cmake
set_target_properties(my_exe PROPERTIES 
        VERSION XXX)
```

这样编译后的可执行文件或者库文件，都带有版本后缀，例如my_exe-1.0.0，如果是库文件，这是my_lib.so.1.0.0。


## 模块重复编译怎么办？
假设用A<-B表示模块A依赖模块B，那么假设我们有这种结构：A <- B <- C，　A <- C，cmake构建的时候会提示出现重复的模块．

C可能是一个很底层的模块，例如基础组件规范化打印．
不能说因为B依赖了C，A就不依赖C了，A和B依赖的C的版本有可能是不一样的，甚至可能有一天B突然不依赖C了。
所以我们仍然需要能“同时”编译的方法。

使用如下的语句，就可以防止提示重复模块．

```cmake
+if(NOT TARGET ha_util)
 add_subdirectory(3rd-party/ha_util)
+endif()
```

## 依赖option

通过option我们可以进行选择编译，但是比起最基本的单个option，我们也会需要复杂一点的操作，例如BUILD_ALL选项和BUILD_A选项，两者只有有一个开，就编译A模块。

然而，选项是无法进行or操作的，cmake这蛋疼的设计。。。

```cmake

option(BUILD_ALL OFF)
option(BUILD_A OFF)

if(BUILD_ALL or BUILD_A)    #cmake不认这样的语句
...
endif()

```

为此，cmake提供了另一种解决方案，依赖option

```cpp
include(CmakeDependentOption)   # 一定要加入，否则cmake不识别，官网的文档还没提到这点，不得不说cmake官方文档真的不友好。
option(BUILD_ALL OFF)
option(BUILD_A OFF)
cmake_dependent_option(BUILD_A_FINAL "Some description text" OFF "NOT BUILD_ALL; NOT BUILD_A" ON)
if(BUILD_A_FINAL)
endif()
```

含义是，如果BUILD_ALL是OFF，BUILD_A也是OFF，那么BUILD_A_FINAL才是OFF，也就是说BUILD_A_FINAL = BUILD_ALL or BUILD_A
这种特殊写法很绕，而且看起来很像是临时补丁，特别的丑陋，对比rust家庭的cargo，完全不一样。


## IDE能编，terminal却找不到头文件？
明明头文件的路径已经添加，并且在**QtCreator**里能够跟踪到该头文件，但是在**terminal** make的时候仍然找不到头文件？
有一个原因：CMakeList.txt用了环境变量$ENV{}，并且QtCreator设置了这个环境变量，但是bash里面并没有设置！
反过来，能编译，但是QtCreator老是找不到头文件，或者在QtCreator内部构建失败。

## 以tinyply为例讲解现代cmake用法

tinyply是一个用于读取PLY点云文件的开源代码库，使用的是现代风格的cmake语法，本身规模较小，很适合学习。

先来看看tinyply目录下和cmake相关的文件结构

```
tinyply-2.3.2/
|-- CMakeLists.txt
|-- appveyor.yml
|-- cmake
|   |-- Config.cmake.in
|   `-- tinyply.pc.in
|-- readme.md
|-- source
|   |-- Makefile
|   |-- example-utils.hpp
|   |-- example.cpp
|   |-- tests.cpp
|   |-- tinyply.cpp
|   `-- tinyply.h
`-- third-party
    `-- doctest.h

```

先来看看CMakeLists.txt文件，注释就直接写在了代码里。

```cmake
# 设置cmake工具的最低版本，禁止用老版本cmake来构建当前工程。
cmake_minimum_required(VERSION 2.8)

# 设置当前工程的编程语言为C++，如果是纯CPU代码，这一行也可以省略。
project(tinyply LANGUAGES CXX)

# 设置变量，工程版本，后面会用到
set(PROJECT_VERSION 2.3)

include_directories("${CMAKE_SOURCE_DIR}/source")
include_directories("${CMAKE_SOURCE_DIR}/third-party")

set(CMAKE_DEBUG_POSTFIX "d")

# Library
set(SHARED_LIB true CACHE BOOL "Build as shared library")
if(${SHARED_LIB})
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
    add_library(tinyply SHARED source/tinyply.cpp source/tinyply.h)
else()
    add_library(tinyply STATIC source/tinyply.cpp source/tinyply.h)
endif()

set(BUILD_TESTS false CACHE BOOL "Build tests")

# Example Application
add_executable(example source/example.cpp)
target_link_libraries(example PRIVATE tinyply)

# Test Application
if(${BUILD_TESTS})
  add_executable(tests source/tests.cpp)
  target_link_libraries(tests PRIVATE tinyply)
endif()

# pkg-config support
set(generated_dir "${CMAKE_CURRENT_BINARY_DIR}/generated")
if(UNIX)
  configure_file(cmake/tinyply.pc.in ${generated_dir}/tinyply.pc @ONLY)
  install(FILES "${generated_dir}/tinyply.pc" DESTINATION lib/pkgconfig)
endif()

# CMake package config support
include(CMakePackageConfigHelpers)

set(config_install_dir "lib/cmake/${PROJECT_NAME}")
set(version_config "${generated_dir}/${PROJECT_NAME}ConfigVersion.cmake")
set(project_config "${generated_dir}/${PROJECT_NAME}Config.cmake")
set(targets_export_name "${PROJECT_NAME}Targets")
set(namespace "")

write_basic_package_version_file(
  "${version_config}"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion
)
configure_package_config_file(
  "cmake/Config.cmake.in"
  "${project_config}"
  INSTALL_DESTINATION "${config_install_dir}"
)

# Install
install(TARGETS tinyply EXPORT ${targets_export_name}
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib)
install(FILES source/tinyply.h
        DESTINATION include)

install(FILES "${project_config}" "${version_config}"
        DESTINATION "${config_install_dir}")

install(EXPORT "${targets_export_name}"
        NAMESPACE "${namespace}"
        DESTINATION "${config_install_dir}")
```