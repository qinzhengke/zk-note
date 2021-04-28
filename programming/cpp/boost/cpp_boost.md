标准库的后备军：Boost{#cpp_boost}
=====================================



\section 安装方法

\code{.sh}
sudo apt install libboost-all-dev
\endcode

\section 编译方法

截止版本1.7.6（以后大概率也一样），boost不使用cmake构建，而是使用bootstrap和b2方式构建，这种方式在开源代码领域已经比较少见，具体方法如下：

```
./bootstrap.sh --prefix=path/to/installation/prefix
./b2
sudo ./b2 install
```

默认安装在“/usr/local”目录，即头文件放在“/usr/local/include”，库文件和cmake文件放在“/usr/local/lib”。


\section cmake中的使用方法

\code{.cmake}
find_package(Boost REQUIRED COMPONENTS system thread regex)

add_executable(my_exe main.cc)

target_link_libraries(my_exe ${Boost_SYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY} ${Boost_REGEX_LIBRARY})
\endcode