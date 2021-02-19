VTK：3D可视化库{#cpp_lib_vtk}
===================

\section vtk_useful_example 一个有用的第三方Example网站

https://kitware.github.io/vtk-examples/site/

这个网站上有非常多的示例代码，非常推荐，学习vtk一定要过一遍。


\section vtk_useless_example 无用的Example

VTK的Example没什么用处，甚至存在误导，不要参考。

VTK源码下存在Example目录，但是即使给cmake加上“-DVTK_BUILD_EXAMPLES=ON”参数，仍然没有编译出example。
一探究竟，发现库的作者已经不怎么维护Example了：
(1)Example使用add_test方式编译，但不知道最后的可执行文件在哪。
(2)Example的CMakeLists.txt存在大量注释掉的Example
(3)Example的相关cmake配置存在不用的接口，主要是8.x版本的接口，已经和9.x版本不适配。例如：只有调用但是没有定义的vtk_module_config（当时还找了半天定义）。
