Docker：开源容器工具{#docker}
==========================

<hr>
\section 基于Docker的开发

<hr>
\subsection Docker容器和宿主机之间共享文件

有些比较依赖环境的库会以docker镜像的形式提供给开发者，例如GPU版本的Tensorflow，这意味这我们必须要其对应的容器中进行开发。
那就会涉及到代码文件在哪里保存，以及怎么编辑，怎么传送到容器中的问题了。

容器和宿主机之间共享文件的方式很简单，只需要在启动的时候挂载宿主目录，即使用“-v”选项。

\code{bash}
docker run -it -v /home/haha/下载:/share microsoft/dotnet:latest /bin/bash
\endcode

上述代码把宿主机的“/home/haha/下载”目录挂载到docker中的“/share”目录。