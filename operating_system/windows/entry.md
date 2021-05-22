Windows{#os_windows}
=======================

\subsection 命令行拷贝文件夹？
例如，想要把a目录下的文件夹c拷贝到目录b下，形成b\c这种结构。
\code{bat}
xcopy d:\a\c d:\b\c\ /S /E
\endcode

其中，第一个参数的“c”不能有反斜杠，然后第二个参数一定要把“c\”给补上，虽然现在b下并不存在c，并且一定要**加上反斜杠**，否则工具会提问c是文件还是文件夹。

这里说一些Linux的拷贝文件夹，就自然很多，不需要自己补一个即将拷贝过去的目录的名字

\code{bash}
cp a/c b
#或者
cp a/c b/
\endcode
记住，第一个参数路径最后也不能有反斜杠，

\subsection 命令行删除文件夹
\code{bat}
rd "e f" /s/q
\endcode