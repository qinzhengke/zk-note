其他有用的工具{#useful_tools}
===========================

\section pqiv 图片查看工具：pqiv
根据源文件发生变化而自动刷新，对于算法开发来说非常有用！
下载方法：在github上搜索pqiv，按照其教程下载编译安装。


\section github_page github page
github page提供免费的静态网页挂服务，非常好用，如果想要挂载一些静态页面，但是又不想花费时间做一些建站的基础工作，例如VPS购买、https服务搭建、域名购买等操作。
那使用github page正好解决这个问题，这个zk-note就是挂在在github page上的。
搜索github page，上面会有详细的搭建方法，非常简单。

1. 在github上新建一个仓库，名称叫做“username.github.io”，这个username必须是账户名字。

2. 将静态页面内容copy到仓库，注意根目录下一定要存在index.html。

3. 推送到github，访问“https://username.github.io”就可以快乐的查看生成的页面了！

\section space_sniffer SpaceSniffer: Windows下磁盘占用分析终极工具
非常强大，谁用谁知道。

\section baobab Baobab: Linux下磁盘占用分析工具
类似于Windows下的space sniffer

\section image_magick ImageMagick生成动态gif图片

~~~{.bash}
convert -delay 80 -loop 0 my_images_*.bmp -resize 80% output.gif
~~~

\section ffmpeg_merge ffmpeg工具拼接视频

前言：做视频增稳算法开发的时候，常常要对比输入输出视频，把输入输出左右摆放同时播放，是最好的方法。
使用merge video, join video等关键词查找的时候，搜到的内容大部分都是描述从时间轴上拼接视频。
我们需要的这种拼接方式有专门的英文描述“merge videos side by side”。
~~~
ffmpeg -i left.mp4 -i right.mp4 -filter_complex hstack output.mp4
~~~

\section ffmpeg_resize ffmpeg工具缩放视频

~~~{.bash}
ffmpeg -i input.jpg -vf scale=320:-1 output_320.png
~~~
其中，-1表示高度根据宽度和长宽比自动计算。
