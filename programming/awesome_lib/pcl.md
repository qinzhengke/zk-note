Point Cloud Library（PCL）{#pcl}
===============================


\section pcl_trouble_shooting Trouble Shooting

\subsection 1.82版本编译依赖项

编译PCL的时候，其依赖项必须指定特定版本，太旧或者太新都会导致编译报错。显然PCL代码兼容性不是很好。

可用的PCL、VTK、boost版本组合：

- PCL v1.8.1 + VTK v7.1.1 + boost 1.64

失败的组合：

- PCL v1.8.1 + boost 1.75，random_generator_pure类构造函数不匹配。
- PCL v1.8.1 + VTK v8.2，缺少vtkMapper::ImmediateModeRenderingOn()方法。
- PCL v1.9 + VTK v9.0.1，ContextRenderingOpenGL not found。

\subsection pcl_context_render 1.11版本编译出现ContextRenderingOpenGL not found

v1.11不能依赖太新的VTK，截止此笔记写下的时间，VTK有两大最新版本，9.0和8.2，其中9.0对接口做了较多改动，PCL1.11并未兼容，建议仍然使用VTK8.2。

\subsection vtkMapper ‘class vtkMapper’ has no member named ‘ImmediateModeRenderingOn’

vtk 8.2版本移除了ImmediateModeRenderingOn接口

\subsection pcl_random_gen 编译1.8版本 no matching function for call to random_generator_pure

octree_disk_container.hpp:77:36: error: no matching function for call to ‘boost::uuids::random_generator_pure::random_generator_pure(boost::random::mt19937*)

boost版本降级到1.64

\subsection 内存溢出导致编译报错

平行编译的时候，job数不要定义得太多，否则内存溢出会导致编译失败，“make -j4”就很容易让内存占用达到16G。

\subsection pcl_cmake_location 编译好后的PCLConfig.cmake在哪里？

和其他库不太一样，“make install”后cmake安装默认路径在“/usr/local/share”