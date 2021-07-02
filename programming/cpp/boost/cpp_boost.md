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

\section boost_filesystem boost filesystem 的链接

\code{.cmake}
target_link_libraries(mylib boost_system ${Boost_FILESYSTEM_LIBRARIES})
\endcode

不知道为什么“boost_system”和“${Boost_FIELSYSTEM_LIBRARIES}”两者都必须添加。

如果缺少“boost_system”，则当mylib被其他库引用的时候会报出类似错误

\code
/usr/bin/ld: xxx/xxx.cc.o: undefined reference to symbol '_ZN5boost6system15system_categoryEv'
//usr/lib/x86_64-linux-gnu/libboost_system.so.1.65.1: error adding symbols: DSO missing from command line
collect2: error: ld returned 1 exit status
\endcode

如果缺少“{Boost_FILESYSTEM_LIBRARIES}”，则同样mylib被其他库引用的时候，会报出另一个错误，

\code
../mylib.so: undefined reference to `boost::filesystem::detail::dir_itr_close(void*&, void*&)'
../mylib.so: undefined reference to `boost::filesystem::detail::directory_iterator_construct(boost::filesystem::directory_iterator&, boost::filesystem::path const&, boost::system::error_code*)'
...
\endcode