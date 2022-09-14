# Docker：开源容器工具


# 基于Docker的开发


## Docker容器和宿主机之间共享文件

有些比较依赖环境的库会以docker镜像的形式提供给开发者，例如GPU版本的Tensorflow，这意味这我们必须要其对应的容器中进行开发。
那就会涉及到代码文件在哪里保存，以及怎么编辑，怎么传送到容器中的问题了。

容器和宿主机之间共享文件的方式很简单，只需要在启动的时候挂载宿主目录，即使用“-v”选项。

\code{bash}
docker run -it -v /home/haha/下载:/share microsoft/dotnet:latest /bin/bash
\endcode

上述代码把宿主机的“/home/haha/下载”目录挂载到docker中的“/share”目录。

## 如何减少容器commit后得到镜像的大小？

和git类似，为了保证回溯，每一次commit，docker会把容器的所有历史内容都记录下来，哪怕将来的某个commit会将对应的内容删除。
这样也会导致每一次commit，镜像变得很大，一个5-6G的镜像，最终可能达到25-30G。

一些情况下，为了减少体积，我们可以不需要回溯功能，可以这样压缩（shrink或flatten）容器。

```
docker export <CONTAINER ID> | docker import - some-image-name:latest
```
