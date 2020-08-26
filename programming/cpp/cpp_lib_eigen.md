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

\section 传参时MatrixNd到MatrixXd的转换

函数传参时，MatrixNd要转换成MatrixXd，（N表示具体的数字，例如3），编译器会自动转换，但是转换得到的是一个右值，是一个临时变量。
如果函数的形参是非常量引用，那么编译器就会报错。

\code{.cpp}
void func(MatrixXd &a){

}

Matrix3d x;
Matrix
func(x);    // x传入之后自动cast，变成右值。
\endcode

<hr>
\section 四元数与欧拉角的转换

\code{.cpp}
// 从欧拉角到四元数
Quaternionf q = AngleAxisf(roll, Vector3f::UnitX())
    * AngleAxisf(pitch, Vector3f::UnitY())
    * AngleAxisf(yaw, Vector3f::UnitZ());

// 从四元数到欧拉角
auto euler = q.toRotationMatrix().eulerAngles(0, 1, 2);
\endcode