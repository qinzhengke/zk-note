C++ json序列化库：iguana{#cpp_lib_iguana}
=======================================

iguana的意思是变色龙

\section REFLECTION的宏定义必须紧接着结构体

如题，REFLECTION的宏定义必须紧接着结构体，否则编译是会报错的，所以这仍然不是完美的非入侵式反射，结构体定义的地方必须依赖iguana。