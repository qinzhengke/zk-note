标准库的后备军：Boost{#cpp_boost}
=====================================



\section 安装方法

\code{.sh}
sudo apt install libboost-all-dev
\endcode


\section cmake中的使用方法

\code{.cmake}
find_package(Boost REQUIRED COMPONENTS system thread regex)

add_executable(my_exe main.cc)

target_link_libraries(my_exe ${Boost_SYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY} ${Boost_REGEX_LIBRARY})
\endcode