#include <string>
#include <ros/ros.h>
#include <iostream>
#include <time.h>
#include "vector"
#include "std_msgs/Float32.h"
#include "std_msgs/Float64MultiArray.h"
#include "controller_manager_msgs/SwitchController.h"
#include "controller_manager_msgs/ListControllers.h"
#include "ikfast.h"
#include "probot_anno_manipulator_ikfast_moveit_plugin.cpp"
#include <Eigen/Eigen>
#include <Eigen/Dense>
#include <Eigen/Geometry>
#include <Eigen/Eigenvalues>

double alpha[6] = {0, M_PI/2, 0, M_PI/2, -M_PI/2, M_PI/2};
double a[6] = {0, 0, 0.225, 0, 0, 0};
double d[6] = {0, 0, 0, 0.2289, 0, 0.055};

using namespace ikfast_kinematics_plugin;
using namespace std;
void calPKPos(double link[]){
    link[1] = link[1] + M_PI/2;
    link[4] = link[4] - M_PI/2;
    Eigen::Matrix4d result;
    result.setIdentity(4,4);
    for(int i=0; i<6; i++){
        Eigen::Matrix4d T ;
        T << cos(link[i]), -sin(link[i]), 0, a[i],
            sin(link[i])*cos(alpha[i]), cos(link[i])*cos(alpha[i]), -sin(alpha[i]), -sin(alpha[i])*d[i],
            sin(link[i])*sin(alpha[i]), cos(link[i])*sin(alpha[i]), cos(alpha[i]), cos(alpha[i])*d[i],
            0, 0, 0, 1;
        result = result * T;
    }
    result(2,3) += 0.284;
    cout<<result<<endl<<endl;
}

std::vector<double> myCalIK(std::vector<double> pos)
{
    Eigen::Matrix3d R06;
    R06.row(0)<<cos(pos[4])*cos(pos[5]), cos(pos[3])*sin(pos[4])*cos(pos[5])+sin(pos[3])*sin(pos[5]), -sin(pos[3])*sin(pos[4])*cos(pos[5])+cos(pos[3])*sin(pos[5]);
    R06.row(1)<<cos(pos[4])*sin(pos[5]), cos(pos[3])*sin(pos[4])*sin(pos[5])-sin(pos[3])*cos(pos[5]),-sin(pos[3])*sin(pos[4])*sin(pos[5])-cos(pos[3])*cos(pos[5]);
    R06.row(2)<<-sin(pos[4]), cos(pos[3])*cos(pos[4]), -sin(pos[3])*cos(pos[4]);

    double wx = pos[0] - R06(0,2) * 0.055;
    double wy = pos[1] - R06(1,2) * 0.055;
    double wz = pos[2] - R06(2,2) * 0.055;
    double theta1 = atan2(wy, wx);
    double s3 = ((wy / sin(theta1)) * (wy / sin(theta1)) + (wz - 0.284) * (wz - 0.284) - 0.1031) / 0.1031;
    double theta3 = atan2(s3, sqrt(1 - s3 * s3));
    double k1 = 0.229 * cos(theta3);
    double k2 = 0.225 + 0.229 * sin(theta3);
    double theta2 = atan2(wy, sin(theta1) * (0.284 - wz)) - atan2(k2, k1);
    if(theta2<=-M_PI/2) theta2+=M_PI;
    std::vector<double> angle;
    angle.push_back(theta1);
    angle.push_back(theta2);
    angle.push_back(theta3);
    Eigen::Matrix3d R03;

    R03.setIdentity();
    double alpha[3]={0,M_PI/2,0};
    for(int i=0;i<3;i++){
        Eigen::Matrix3d R;
        R.row(0)<<cos(angle[i]), -sin(angle[i]), 0;
        R.row(1)<<sin(angle[i])*cos(alpha[i]), cos(angle[i])*cos(alpha[i]), -sin(alpha[i]);
        R.row(2)<<sin(angle[i])*sin(alpha[i]), cos(angle[i])*sin(alpha[i]), cos(alpha[i]);
        R03 = R03*R;
    }

    Eigen::Matrix3d R36;
    R36=R03.inverse()*R06;

    double theta6=atan2(-R36(1,1),R36(1,0));
    double theta4=atan2(R36(2,2),R36(0,2));
    double theta5=atan2(R36(0,2)/cos(theta4),-R36(1,2))+1.57;

    if(theta4<=-3.13){
        theta4=0;theta5=M_PI-theta5;
        if(theta6<0) theta6+=M_PI;
        else theta6-=M_PI;
        if(theta6>=6.27) theta6-=2*M_PI;
    }
    angle.push_back(theta4);
    angle.push_back(theta5);
    angle.push_back(theta6);
    angle[1] -= M_PI/2;
    return angle;
}


std::vector<double> calIKPos(double end_pos[]){
    // end_pos[2] = end_pos[2] - 0.284;
    // Eigen::Matrix3d R_X;
    // Eigen::Matrix3d R_Y;
    // Eigen::Matrix3d R_Z;
    // Eigen::Matrix3d R_end;
    // end_pos[3] = end_pos[3]+M_PI/2;
    // R_X << 1, 0, 0,
    //         0, cos(end_pos[3]), -sin(end_pos[3]),
    //         0, sin(end_pos[3]), cos(end_pos[3]);
    // R_Y << cos(end_pos[4]), 0, sin(end_pos[4]),
    //         0, 1, 0,
    //         -sin(end_pos[4]), 0, cos(end_pos[4]);
    // R_Z << cos(end_pos[5]), -sin(end_pos[5]), 0,
    //         sin(end_pos[5]), cos(end_pos[5]), 0,
    //         0, 0, 1;
    // R_end = R_Z*R_Y*R_X;

    // Eigen::Matrix4d T_end;
    // T_end.block<3,3>(0,0) = R_end;
    // for(int i = 0; i < 3; i++){
    //     T_end(i,3) = end_pos[i];
    // }
    // T_end.row(3) << 0, 0, 0, 1;
    // // cout << T_end<<endl<<endl;

    // //储存结果的关节角数组
    // double theta[6];

    // //求取腕部坐标
    // double pos_wrist[3];
    // for(int i = 0; i < 3; i++){
    //     pos_wrist[i] = end_pos[i] - R_end(i,2)*0.055;
    //     cout << pos_wrist[i] << endl;
    // }

    // //求取前三轴
    // theta[0] = atan2(pos_wrist[1], pos_wrist[0]);
    // cout << cos(theta[0]) << endl;
    // double c_2 = ((pos_wrist[0]/cos(theta[0]))*(pos_wrist[0]/cos(theta[0]))
    //                 +pos_wrist[2]*pos_wrist[2]-0.2289*0.2289-0.225*0.225) / (2*0.2289*0.225);
    // cout << c_2 <<endl;
    // theta[2] = acos(c_2);
    // double beta = atan2(pos_wrist[2],pos_wrist[0]/cos(theta[0]));
    // double gama = ((pos_wrist[0]/cos(theta[0]))*(pos_wrist[0]/cos(theta[0]))
    //                 +pos_wrist[2]*pos_wrist[2]-0.2289*0.2289+0.225*0.225)
    //                 / (2*0.225*sqrt((pos_wrist[0]/cos(theta[0]))*(pos_wrist[0]/cos(theta[0]))+pos_wrist[2]*pos_wrist[2]));
    // if(theta[2]<0){
    //     theta[1] = beta + gama;
    // }else
    // {
    //     theta[1] = beta - gama;
    // }
    // theta[2] = -theta[2] + M_PI/2;
    // theta[1] = M_PI/2 - theta[1];
    // //求取后三轴
    // Eigen::Matrix4d T_0_3;
    // T_0_3.setIdentity(4,4);
    // for(int i=0; i<3; i++){
    //     Eigen::Matrix4d T ;
    //     T << cos(theta[i]), -sin(theta[i]), 0, a[i],
    //         sin(theta[i])*cos(alpha[i]), cos(theta[i])*cos(alpha[i]), -sin(alpha[i]), -sin(alpha[i])*d[i],
    //         sin(theta[i])*sin(alpha[i]), cos(theta[i])*sin(alpha[i]), cos(alpha[i]), cos(alpha[i])*d[i],
    //         0, 0, 0, 1;
    //     T_0_3 = T_0_3 * T;
    // }
    // Eigen::Matrix3d R_0_3 = T_0_3.block<3,3>(0,0);
    // Eigen::Matrix3d R_4_6 = R_0_3.inverse() * R_end;
    // // cout << R_4_6<<endl<<endl;

    // theta[4] = atan2(sqrt(R_4_6(2,0)*R_4_6(2,0)+R_4_6(2,1)*R_4_6(2,1)),R_4_6(2,2));
    // theta[3] = atan2(R_4_6(1,2)/sin(theta[4]), R_4_6(0,2)/sin(theta[4]));
    // theta[5] = atan2(R_4_6(2,1)/sin(theta[4]), R_4_6(2,0)/sin(theta[4]));

    // for(int i = 0; i<6; i++){
    //     cout << theta[i] <<endl;
    // }
    // return theta;

    //利用逆运动学求解器
    std::vector<double> result;
    result.resize(6);
    ikfast_kinematics_plugin::IKFastKinematicsPlugin ik;
    //初始化对象，参数是机械臂模型相关的一些内容，link_6即末端连杆，它的坐标系所在位置就是末端中心点，与PPT图片里的一致
    bool ret = ik.IKFastKinematicsPlugin::initialize("robot_description","manipulator","base_link","link_6",0.001);

    geometry_msgs::Pose target_pose;
    target_pose.position.x = end_pos[0];
    target_pose.position.y = end_pos[1];
    target_pose.position.z = end_pos[2];
    target_pose.orientation.w = cos((end_pos[3]+M_PI/2)/2)*cos(end_pos[4]/2)*cos(end_pos[5]/2)+sin((end_pos[3]+M_PI/2)/2)*sin(end_pos[4]/2)*sin(end_pos[5]/2);
    target_pose.orientation.x = sin((end_pos[3]+M_PI/2)/2)*cos(end_pos[4]/2)*cos(end_pos[5]/2)-cos((end_pos[3]+M_PI/2)/2)*sin(end_pos[4]/2)*sin(end_pos[5]/2);
    target_pose.orientation.y = cos((end_pos[3]+M_PI/2)/2)*sin(end_pos[4]/2)*cos(end_pos[5]/2)+sin((end_pos[3]+M_PI/2)/2)*cos(end_pos[4]/2)*sin(end_pos[5]/2);
    target_pose.orientation.z = cos((end_pos[3]+M_PI/2)/2)*cos(end_pos[4]/2)*sin(end_pos[5]/2)-sin((end_pos[3]+M_PI/2)/2)*sin(end_pos[4]/2)*cos(end_pos[5]/2);

    //pose1为vector<geometry_msgs::Pose>类型变量，装入上述位姿信息
    std::vector<geometry_msgs::Pose> pose1;
    pose1.push_back(target_pose);

    //设定关节角参考初值，这是求解函数要求的一个参数，默认为0即可，是vector<double>类型
    std::vector<double> seed1;
    seed1.push_back(0.0);
    seed1.push_back(0.0);
    seed1.push_back(0.0);
    seed1.push_back(0.0);
    seed1.push_back(0.0);
    seed1.push_back(0.0);

    //创建用于保存逆解的变量，它是vector矩阵，vector<std::vector<double>>类型，因为会解出不止一组解装在里面
    //内容六关节角度，每个角度是浮点数，为弧度制
    std::vector<std::vector<double>> sol_rad;
    //这是求解函数要求的一个参数，具体功能不明
    kinematics::KinematicsResult kinematic_result;

    // cout << ret <<endl;
    //计算逆解
    //参数表：（末端位姿，参考关节角初值，关节角，......）
    ret = ik.getPositionIK(pose1, seed1, sol_rad, kinematic_result, kinematics::KinematicsQueryOptions());

    // if(ret)//若求解成功，输出结果
    // {
    //     std::cout << sol_rad.size() << " IK solved successfully." << endl;
    //     //角度制计算，并输出每一组计算结果
    //     for (int q = 0; q < sol_rad.size(); q++)
    //     {
    //         for (int i = 0; i < 6; i++)
    //         {
    //             cout << sol_rad[q][i]<<" ";
    //         }
    //         cout << endl;
    //     }
    // }
    for(int i = 0; i<6 ;i++){
        result[i] = sol_rad[0][i];
    }

    return result;
}