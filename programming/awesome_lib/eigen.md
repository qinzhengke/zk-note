# Eigen：强大的纯头文件线性运算库

## 安装

```bash
cd <eigen_dir>
mdkir build
cd build
cmake ..
sudo make install
# 没错，中间没有make，因为eigen是纯头文件组成的，不需要预先编译成库。
# 安装好后，默认的头文件地址在 /usr/local/include/eigen3
```


## 矩阵的初始化（包括全零阵、单位阵）



## 获取子矩阵

参见： https://eigen.tuxfamily.org/dox/group__TutorialBlockOperations.html


## Matrix和C数组之间的转换

参见： https://eigen.tuxfamily.org/dox/group__TutorialMapClass.html


## eigen_auto 使用auto定义Eigen相关的变量得到右值？

使用auto定义Eigen相关的变量时候，有时候会得到右值，这些右值无法被赋值，但是使用类型来定义就成了左值。

```cpp
// 编译失败
auto a = Eigen::Matrix<float,9,9>::Zero();

// 编译成功
Eigen::Matrix<float,9,9> a = Eigen::Matrix<float,9,9>::Zero();

// 编译成功
auto a = Eigen::Matrix3f::Zero();
```

再例如，

```cpp
    auto mat1 = Eigen::Matrix<T,4,4>::Identity();
    // 下面语句报错：lvalue_operand error: lvalue required as left operand of assignment
    mat1(0,3) = 1;

    Eigen::Matrix<T,4,4> mat2 = Eigen::Matrix<T,4,4>::Identity();
    mat2(0,3) = 1; // 编译通过
```

## AngleAxis用于初始化注意事项

```cpp
    // 编译错误
    Matrix3d rot = AngleAxisd(0, Vector3d::UnitZ()) *
        AngleAxisd(0, Vector3d::UnitY()) *
        AngleAxisd(0, Vector3d::UnitX());

    // 编译成功
    Matrix3d rot;
    rot = AngleAxisd(param.view_euler[2], Vector3d::UnitZ()) *
        AngleAxisd(param.view_euler[1], Vector3d::UnitY()) *
        AngleAxisd(param.view_euler[0], Vector3d::UnitX());
```

## 传参时MatrixNd到MatrixXd的转换

函数传参时，MatrixNd要转换成MatrixXd，（N表示具体的数字，例如3），编译器会自动转换，但是转换得到的是一个右值，是一个临时变量。
如果函数的形参是非常量引用，那么编译器就会报错。

```cpp
void func(MatrixXd &a){

}

Matrix3d x;
Matrix
func(x);    // x传入之后自动cast，变成右值。
```


## 四元数与欧拉角的转换

```cpp
// 从欧拉角到四元数
Quaternionf q = AngleAxisf(yaw, Vector3f::UnitZ())
    * AngleAxisf(pitch, Vector3f::UnitY())
    * AngleAxisf(roll, Vector3f::UnitX());

// 从四元数到欧拉角
auto euler = q.toRotationMatrix().eulerAngles(2, 1, 0);
```


## Eigen中四元数到欧拉角的一个一定要注意的问题

使用这种方式计算的欧拉角的三个角度的范围是[0,pi], [-pi,pi], [-pi,pi]。
对于我们的XYZ欧拉角，也就是说yaw角的范围是[0,pi]，机器人的运动领域中是不够用的，因为在平面移动的时候，yaw角通常取值范围是[-pi,pi]。

```cpp
Vector3d euler(Quaterniond q){
    return q.toRotationMatrix().eulerAngles(2,1,0);
}

double yaw(Quaterniond q){
    return getEuler(q)[0];
}

Quaterniond getQ(Vector3d euler){
    return  AngleAxisd(euler[0], Vector3d::UnitZ())
        *AngleAxisd(euler[1], Vector3d::UnitY())
        *AngleAxisd(euler[2], Vector3d::UnitX());
}


cout<<yaw(Quaterniond(0.707,0,0,0.707))<<endl;
cout<<yaw(Quaterniond(0.707,0,0,-0.707))<<endl;
Vector3d e(1.57,0,0);
auto q = getQ(e);
cout<<q.w()<<","<<q.x()<<","<<q.y()<<","<<q.z()<<endl;

```

输出结果：
```
1.57049
1.5711  // 错误结果 这里应该是 -1.57才对
0.707388,0,0,0.706825
```


## eigen中各种旋转表达之间的转换

代码来自 https://blog.csdn.net/weicao1990/article/details/86148828

```cpp
#include <iostream>
 
#include <Eigen/Core>
#include <Eigen/Geometry>
 
using namespace std;
 
#define PI (3.1415926535897932346f)
 
int main(int argc, char **argv) 
{
    /**** 1. 旋转向量 ****/
    cout << endl << "********** AngleAxis **********" << endl;
    //1.0 初始化旋转向量,沿Z轴旋转45度的旋转向量
    Eigen::AngleAxisd rotation_vector1 (M_PI/4, Eigen::Vector3d(0, 0, 1)); 
 
    //1.1 旋转向量转换为旋转矩阵
    //旋转向量用matrix()转换成旋转矩阵
    Eigen::Matrix3d rotation_matrix1 = Eigen::Matrix3d::Identity();
    rotation_matrix1 = rotation_vector1.matrix();
    cout << "rotation matrix1 =\n" << rotation_matrix1 << endl;                
    //或者由罗德里格公式进行转换
    rotation_matrix1 = rotation_vector1.toRotationMatrix();
    cout << "rotation matrix1 =\n" << rotation_matrix1 << endl; 
 
    /*1.2 旋转向量转换为欧拉角*/
    //将旋转向量转换为旋转矩阵,再由旋转矩阵转换为欧拉角,详见旋转矩阵转换为欧拉角
    Eigen::Vector3d eulerAngle1 = rotation_vector1.matrix().eulerAngles(2,1,0);
    cout << "eulerAngle1, z y x: " << eulerAngle1 << endl;
 
    /*1.3 旋转向量转四元数*/
    Eigen::Quaterniond quaternion1(rotation_vector1);
    //或者
    Eigen::Quaterniond quaternion1_1;
    quaternion1_1 = rotation_vector1;
    cout << "quaternion1 x: " << quaternion1.x() << endl;
    cout << "quaternion1 y: " << quaternion1.y() << endl;
    cout << "quaternion1 z: " << quaternion1.z() << endl;
    cout << "quaternion1 w: " << quaternion1.w() << endl;
    
    cout << "quaternion1_1 x: " << quaternion1_1.x() << endl;
    cout << "quaternion1_1 y: " << quaternion1_1.y() << endl;
    cout << "quaternion1_1 z: " << quaternion1_1.z() << endl;
    cout << "quaternion1_1 w: " << quaternion1_1.w() << endl;
 
 
    /**** 2. 旋转矩阵 *****/
    cout << endl << "********** RotationMatrix **********" << endl;
    //2.0 旋转矩阵初始化
    Eigen::Matrix3d rotation_matrix2;
    rotation_matrix2 << 0.707107, -0.707107, 0, 0.707107, 0.707107, 0, 0, 0, 1;
;
    //或直接单位矩阵初始化
    Eigen::Matrix3d rotation_matrix2_1 = Eigen::Matrix3d::Identity();
 
    //2.1 旋转矩阵转换为欧拉角
    //ZYX顺序，即先绕x轴roll,再绕y轴pitch,最后绕z轴yaw,0表示X轴,1表示Y轴,2表示Z轴
    Eigen::Vector3d euler_angles = rotation_matrix2.eulerAngles(2, 1, 0); 
    cout << "yaw(z) pitch(y) roll(x) = " << euler_angles.transpose() << endl;
 
    //2.2 旋转矩阵转换为旋转向量
    Eigen::AngleAxisd rotation_vector2;
    rotation_vector2.fromRotationMatrix(rotation_matrix2);
    //或者
    Eigen::AngleAxisd rotation_vector2_1(rotation_matrix2);
    cout << "rotation_vector2 " << "angle is: " << rotation_vector2.angle() * (180 / M_PI) 
                                << " axis is: " << rotation_vector2.axis().transpose() << endl;
 
    cout << "rotation_vector2_1 " << "angle is: " << rotation_vector2_1.angle() * (180 / M_PI) 
                                  << " axis is: " << rotation_vector2_1.axis().transpose() << endl;
 
    //2.3 旋转矩阵转换为四元数
    Eigen::Quaterniond quaternion2(rotation_matrix2);
    //或者
    Eigen::Quaterniond quaternion2_1;
    quaternion2_1 = rotation_matrix2;
    cout << "quaternion2 x: " << quaternion2.x() << endl;
    cout << "quaternion2 y: " << quaternion2.y() << endl;
    cout << "quaternion2 z: " << quaternion2.z() << endl;
    cout << "quaternion2 w: " << quaternion2.w() << endl;
    
    cout << "quaternion2_1 x: " << quaternion2_1.x() << endl;
    cout << "quaternion2_1 y: " << quaternion2_1.y() << endl;
    cout << "quaternion2_1 z: " << quaternion2_1.z() << endl;
    cout << "quaternion2_1 w: " << quaternion2_1.w() << endl;
 
 
    /**** 3. 欧拉角 ****/
    cout << endl << "********** EulerAngle **********" << endl;
    //3.0 初始化欧拉角(Z-Y-X，即RPY, 先绕x轴roll,再绕y轴pitch,最后绕z轴yaw)
    Eigen::Vector3d ea(0.785398, -0, 0);
 
    //3.1 欧拉角转换为旋转矩阵
    Eigen::Matrix3d rotation_matrix3;
    rotation_matrix3 = Eigen::AngleAxisd(ea[0], Eigen::Vector3d::UnitZ()) * 
                       Eigen::AngleAxisd(ea[1], Eigen::Vector3d::UnitY()) * 
                       Eigen::AngleAxisd(ea[2], Eigen::Vector3d::UnitX());
    cout << "rotation matrix3 =\n" << rotation_matrix3 << endl;   
 
    //3.2 欧拉角转换为四元数,
    Eigen::Quaterniond quaternion3;
    quaternion3 = Eigen::AngleAxisd(ea[0], Eigen::Vector3d::UnitZ()) * 
                  Eigen::AngleAxisd(ea[1], Eigen::Vector3d::UnitY()) * 
                  Eigen::AngleAxisd(ea[2], Eigen::Vector3d::UnitX());
    cout << "quaternion3 x: " << quaternion3.x() << endl;
    cout << "quaternion3 y: " << quaternion3.y() << endl;
    cout << "quaternion3 z: " << quaternion3.z() << endl;
    cout << "quaternion3 w: " << quaternion3.w() << endl;
 
    //3.3 欧拉角转换为旋转向量
    Eigen::AngleAxisd rotation_vector3;
    rotation_vector3 = Eigen::AngleAxisd(ea[0], Eigen::Vector3d::UnitZ()) * 
                       Eigen::AngleAxisd(ea[1], Eigen::Vector3d::UnitY()) * 
                       Eigen::AngleAxisd(ea[2], Eigen::Vector3d::UnitX());  
    cout << "rotation_vector3 " << "angle is: " << rotation_vector3.angle() * (180 / M_PI) 
                                << " axis is: " << rotation_vector3.axis().transpose() << endl;
 
 
    /**** 4.四元数 ****/
    cout << endl << "********** Quaternion **********" << endl;
    //4.0 初始化四元素,注意eigen Quaterniond类四元数初始化参数顺序为w,x,y,z
    Eigen::Quaterniond quaternion4(0.92388, 0, 0, 0.382683);
 
    //4.1 四元数转换为旋转向量
    Eigen::AngleAxisd rotation_vector4(quaternion4);
    //或者
    Eigen::AngleAxisd rotation_vector4_1;
    rotation_vector4_1 = quaternion4;
    cout << "rotation_vector4 " << "angle is: " << rotation_vector4.angle() * (180 / M_PI) 
                                << " axis is: " << rotation_vector4.axis().transpose() << endl;
 
    cout << "rotation_vector4_1 " << "angle is: " << rotation_vector4_1.angle() * (180 / M_PI) 
                                  << " axis is: " << rotation_vector4_1.axis().transpose() << endl;
 
    //4.2 四元数转换为旋转矩阵
    Eigen::Matrix3d rotation_matrix4;
    rotation_matrix4 = quaternion4.matrix();
    Eigen::Matrix3d rotation_matrix4_1;
    rotation_matrix4_1 = quaternion4.toRotationMatrix();
    cout << "rotation matrix4 =\n" << rotation_matrix4 << endl;
    cout << "rotation matrix4_1 =\n" << rotation_matrix4_1 << endl;      
 
 
    //4.4 四元数转欧拉角(Z-Y-X，即RPY)
    Eigen::Vector3d eulerAngle4 = quaternion4.matrix().eulerAngles(2,1,0);
    cout << "yaw(z) pitch(y) roll(x) = " << eulerAngle4.transpose() << endl;
 
    return 0;
}
```

##  编译报错： no type named Return Type

带有模板的报错通常都是一大堆输出刷屏，很难从报错的字面上看出问题是什么，这个问题的原因则是两个矩阵操作时，类型不一样。
比如一个float型，一个是double型。

## Mat::block()与模板类的组合使用方法

对模板类Mat对象调用block()方法，会直接报错，需要一些独特的调用方法，比较少见。

```cpp
void Pack::applyTransform(Eigen::Matrix<T,4,4> mat){
    Matrix<T,3,3> R = mat.template block<3,3>(0,0);
    Matrix<T,3,1> t = mat.template block<3,1>(0,3);
}
```