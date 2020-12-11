C++ debug教训{#cpp_debug}
========================


\section 企图通过memcpy对象进行拷贝操作。

类成员对象里面如果有容器类型，就会出现coredump，具体原因待研究。