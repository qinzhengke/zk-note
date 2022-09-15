# Symbol（符号）

## 什么是符号？

Symbol（符号）是C、C++语言编译后的二进制文件中的入口，包含目标o文件，静态库a文件，以及共享库so文件。



## 为什么要了解符号？

在应用构建的link阶段、或者so文件动态加载的阶段，（例如插件框架动态加载so文件、python加载so文件）。

这些任务是否能够快速的开发，依赖关于symbol的知识。

举个例子，如果没有妥善的设置函数，那么so文件可能不会包含期望的函数symbol，那么在共享库的动态加载和resolve阶段，就会出现问题。
这些问题往往没有很好的提示，最好的是提示unresolved external symbol，最差的情况是程序直接crash，无从查起。

所以了解symbol相关的知识能够让我们避开或者更快的解决这些问题。

## 用什么工具查看符号？

```
nm --help target.so
nm --extern-only --dynamic target.so
```

## 一个空的cc文件对应的symbol

```
nm target.so
```

结果如下
```
0000000000201020 B __bss_start
0000000000201020 b completed.7698
                 w __cxa_finalize
0000000000000450 t deregister_tm_clones
00000000000004e0 t __do_global_dtors_aux
0000000000200e78 t __do_global_dtors_aux_fini_array_entry
0000000000201018 d __dso_handle
0000000000200e80 d _DYNAMIC
0000000000201020 D _edata
0000000000201028 B _end
000000000000052c T _fini
0000000000000520 t frame_dummy
0000000000200e70 t __frame_dummy_init_array_entry
0000000000000538 r __FRAME_END__
0000000000201000 d _GLOBAL_OFFSET_TABLE_
                 w __gmon_start__
0000000000000410 T _init
                 w _ITM_deregisterTMCloneTable
                 w _ITM_registerTMCloneTable
0000000000000490 t register_tm_clones
0000000000201020 d __TMC_END__

``` 

## 类成员函数的实现和声明放到一起

当类成员函数的实现和声明放到一起的时候，也就是放在class内部，如果没有其他模块调用该成员函数，那么该成员函数的symbol是不会在so文件中出现的。

如下例子所示，funA的定义放在了class内部，so文件中不会出现funA，而会出现funB。

```cpp
class Mylib{
public:
    // 若无调用，该构造函数不会在so文件中出现
    Mylib(){}

    // 若无调用，该函数不会在so文件中出现
    void funA(int x){
        printf("%d\n", x);
    }

    void funB(int x);
};

// 该函数会在so文件中出现
void Mylib::funB(int x){
    printf("%d\n", x);
}
```

nm结果如下所示，

```bash
0000000000201028 B __bss_start
0000000000201028 b completed.7698
                 w __cxa_finalizehttps://github.com/qinzhengke/zk-note/blob/$1GLIBC_2.2.5
0000000000000550 t deregister_tm_clones
00000000000005e0 t __do_global_dtors_aux
0000000000200e08 t __do_global_dtors_aux_fini_array_entry
0000000000201020 d __dso_handle
0000000000200e10 d _DYNAMIC
0000000000201028 D _edata
0000000000201030 B _end
0000000000000654 T _fini
0000000000000620 t frame_dummy
0000000000200e00 t __frame_dummy_init_array_entry
0000000000000700 r __FRAME_END__
0000000000201000 d _GLOBAL_OFFSET_TABLE_
                 w __gmon_start__
0000000000000664 r __GNU_EH_FRAME_HDR
0000000000000500 T _init
                 w _ITM_deregisterTMCloneTable
                 w _ITM_registerTMCloneTable
                 U printfhttps://github.com/qinzhengke/zk-note/blob/$1GLIBC_2.2.5
0000000000000590 t register_tm_clones
0000000000201028 d __TMC_END__
000000000000062a T _ZN5Mylib4funBEi

```