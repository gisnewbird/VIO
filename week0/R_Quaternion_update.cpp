//
// Created by dukeguo on 2019/10/17.
//
#include <iostream>
#include <cmath>
#include <Eigen/Core>
#include <Eigen/Eigen>
#include <Eigen/Dense>
#include <ctime>
#include <sophus/so3.hpp>
#include <sophus/se3.hpp>
////因为win中找不到M_PI，就自己定义了
//#define M_PI   3.14159265358979323846

int main(int argv, char ** argc) {
    ///define a minimized vecter
    Eigen::Vector3d w(0.01, 0.02, 0.03);//[0.01, 0.02, 0.03]^T
    Eigen::AngleAxisd rotation_vetor = Eigen::AngleAxisd(M_PI/2, Eigen::Vector3d(0,0,1));
    Eigen::Matrix3d R = rotation_vetor.toRotationMatrix();
    //std::cout << R * R.transpose() << std::endl;
    Sophus::SO3d SO3_R(R);

    // Quaternion update
    // 四元数更新，使用Eigen库
    clock_t time_quaternion = clock();
    std::cout << "=================================" << std::endl;
    Eigen::Quaterniond q = Eigen::Quaterniond(R);//build the R_Quaternion
    //std::cout << "quaternion from rotation vector : " << q.coeffs().transpose() << std::endl;
    Eigen::Quaterniond w_quaternion(1, w[0] / 2, w[1] / 2, w[2] / 2);//w 2 quaternion
    Eigen::Quaterniond q_update = (q * w_quaternion).normalized();//update the quaternion & normalized
    std::cout << "Eigen Quaternion updated : " << std::endl << q_update.toRotationMatrix() << std::endl;
    std::cout << "使用Eigen库进行Quaternion更新 use time " << 1000 * (clock() - time_quaternion) / (double) CLOCKS_PER_SEC << "ms" << std::endl;

    // 李代数更新
    // 使用Sophus库
    clock_t time_Sophus_so3 = clock();
    std::cout << "=================================" << std::endl;
    Eigen::Vector3d update_so3(0.01, 0.02, 0.03); //更新量
    Sophus::SO3d SO3_updated = SO3_R * Sophus::SO3d::exp(update_so3);
    std::cout<<"Sophus SO3 updated = "<< std::endl << SO3_updated.matrix() <<std::endl;
    std::cout << "使用Sophus库进行SO3更新 use time " << 1000 * (clock() - time_Sophus_so3) / (double) CLOCKS_PER_SEC << "ms" << std::endl;

    // 使用Eigen库
    clock_t time_so3 = clock();
    std::cout << "=================================" << std::endl;
    double Theta = sqrt(w[0] * w[0] + w[1] * w[1] + w[2] * w[2]);     	 // Theta = sqrt(x ^ 2 + y ^ 2 + z ^ 2)
    Eigen::Vector3d a = w.array() / Theta;                               // vecter_a = w / Theta
    Eigen::Matrix3d a_hat,I;                                             //             0    -a3      a2
    a_hat <<        0,       -w[2] / Theta,    w[1] / Theta,             // a_hat <<   a3      0     -a1
            w[2] / Theta,        0,        -w[0] / Theta,               //           -a2     a1       0
            -w[1] / Theta,   w[0] / Theta,         0;
    I << 1, 0, 0,
            0, 1, 0,
            0, 0, 1;
    //Rodrigues's formula: cos(Theta) * I + (1 - cos(Theta)) *  vecter_a * vecter_a ^ T + sin(Theta) * a_hat
    Eigen::Matrix3d exp_w_hat = cos(Theta) * I.array() + (1 - cos(Theta)) * (a * a.transpose()).array() + a_hat.array() * sin(Theta);
    //update:
    Eigen::Matrix3d R_update = R * exp_w_hat;
    std::cout << "Eigen SO3 updated : " << std::endl << R_update << std::endl;
    std::cout << "使用Eigen库进行SO3更新 use time " << 1000 * (clock() - time_so3) / (double) CLOCKS_PER_SEC << "ms" << std::endl;

    return 0;
}
