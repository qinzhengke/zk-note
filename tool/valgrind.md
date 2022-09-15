# 代码内存检测工具valgrind

使用方法：
```
valgrind --tool=memcheck --leak-check=yes --log-file=result.log ./my_exe
```

log中要注意两点：
1. 内存泄漏，关键字：definitely lost in loss record
2. 写非法地址，关键字：Invalid write
