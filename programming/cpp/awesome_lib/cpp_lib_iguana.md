C++ json序列化库：iguana{#cpp_lib_iguana}
=======================================

iguana的意思是变色龙

\section REFLECTION的宏定义必须紧接着结构体

如题，REFLECTION的宏定义必须紧接着结构体，否则编译是会报错的，所以这仍然不是完美的非入侵式反射，结构体定义的地方必须依赖iguana。

\section 数组

不知为何，如果在类中定义原生数组，那么在使用iguana::to_json()方法的时候，就会出现错误。

目前可以绕开的一个方法是使用std::array来代替原生数组。

std::array其实还挺好用的，它可以直接在函数中返回，而原生数组则不行，详情参见 \ref c_return_array 