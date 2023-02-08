# VTK：3D可视化库

## vtk_useful_example 一个有用的第三方Example网站

https://kitware.github.io/vtk-examples/site/

这个网站上有非常多的示例代码，非常推荐，学习vtk一定要过一遍。


## vtk_useless_example 无用的Example

VTK的Example没什么用处，甚至存在误导，不要参考。

VTK源码下存在Example目录，但是即使给cmake加上“-DVTK_BUILD_EXAMPLES=ON”参数，仍然没有编译出example。
一探究竟，发现库的作者已经不怎么维护Example了：
(1)Example使用add_test方式编译，但不知道最后的可执行文件在哪。
(2)Example的CMakeLists.txt存在大量注释掉的Example
(3)Example的相关cmake配置存在不用的接口，主要是8.x版本的接口，已经和9.x版本不适配。例如：只有调用但是没有定义的vtk_module_config（当时还找了半天定义）。

## X11_Xt_LIB could not be found.

完整的问题：
```
CMake Error at Rendering/OpenGL/CMakeLists.txt:304 (message):
   X11_Xt_LIB could not be found.  Required for VTK X lib.
```

解决方法：

```
sudo apt install libxt-dev
```

## Could not find the VTK package with the following required components: RenderingContextOpenGL2

前言：在运行vtk examples时出现这个问题，原因时example貌似使用的8.9版本的vtk，而我们安装的是最新的9.1。

解决方法：移除CMakeLists.txt中，find_package中的vtkRenderingContextOpenGL2依赖，因为新版本9.1已经没有这个东西了。

```
#   vtkRenderingContextOpenGL2

```

## ImageImport->SetImportVoidPointer不更新
不知道为什么，vtkImageImport->SetImportVoidPointer绑定数据地址后，数据更新时，vtk不会更新显示，即使调用了update()方法。
使用vtkImageImport->CopyImportVoidPointer就可以。
