Eigen：强大的纯头文件线性运算库{#cpp_lib_eigen}
==============================

<hr>
\section 安装

~~~{.bash}
cd <eigen_dir>
mdkir build
cd build
cmake ..
sudo make install
# 没错，中间没有make，因为eigen是纯头文件组成的，不需要预先编译成库。
# 安装好后，默认的头文件地址在 /usr/local/include/eigen3
~~~

<hr>
\section 矩阵的初始化（包括全零阵、单位阵）


<hr>
\section 获取子矩阵

参见： https://eigen.tuxfamily.org/dox/group__TutorialBlockOperations.html

<hr>
\section Matrix和C数组之间的转换

参见： https://eigen.tuxfamily.org/dox/group__TutorialMapClass.html

<hr>
\section auto获取矩阵失效？

~~~{.cpp}
// 编译失败
auto a = Eigen::Matrix<float,9,9>::Zero();

// 编译成功
Eigen::Matrix<float,9,9> a = Eigen::Matrix<float,9,9>::Zero();

// 编译成功
auto a = Eigen::Matrix3f::Zero();
~~~