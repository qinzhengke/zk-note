C语言Debug教训{#c_debug_lesson}
======================

<hr>
\section printf打印64位整型

不要使用"%d"或者"%u"，这会使得printf只输出变量的低位。

一定要使用"%lu"和"%ld"进行打印。

别看这错误很小，但是可能让你很长一段时间以为自己的数据来源错了。