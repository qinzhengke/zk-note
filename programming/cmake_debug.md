cmake语法debug{#cmake_debug}
===========================

<hr>
\section cmake_define CMakeLists.txt误定义了宏

add_compile_definitions错误的把exe文件名加入了，实际上这个配置函数不需要指定可执行文件，所以my_exe被错误定义为宏，所以后面用到my_exe类型的变量、命名空间都会报错。

\code{.cmake}
add_compile_definitions(DEF=1)  // 正确
add_compile_definitions(my_exe DEF=1)  //错误
\endcode