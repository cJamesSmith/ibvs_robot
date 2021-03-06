#include <ros/ros.h>
#include <sensor_msgs/Image.h>
#include <opencv2/opencv.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <iostream>
#include <vector>
#include "std_msgs/Float64MultiArray.h"
#include <controller_manager/controller_manager.h>
#include <ctime>
#include <cmath>
#include <algorithm>
#include <cstdio>

#define P 0.0001
#define I 1.0
#define D 0.00001

ros::Publisher my_publisher;
std_msgs::Float64MultiArray cmd_vel;
cv::Mat last_imgimg;
static double angle(cv::Point pt1, cv::Point pt2, cv::Point pt0);
static double angle(cv::Point pt1, cv::Point pt2, cv::Point pt0)
{
    double dx1 = pt1.x - pt0.x;
    double dy1 = pt1.y - pt0.y;
    double dx2 = pt2.x - pt0.x;
    double dy2 = pt2.y - pt0.y;
    return (dx1 * dx2 + dy1 * dy2) / sqrt((dx1 * dx1 + dy1 * dy1) * (dx2 * dx2 + dy2 * dy2) + 1e-10);
}
void myCallback(const sensor_msgs::ImageConstPtr &img)
{
    int g_nThresh = 30;
    int g_nMaxThresh = 175;
    static int fuck_count;
    cv::Mat imgimg;
    cv::Mat hsv_img, mask_img, green_img;
    cv::Mat gray_img, scharr_img, scharr_imgx, scharr_imgy, absscharr_imgx, absscharr_imgy;
    cv::Mat corner_img, last_corner_img;
    std::vector<cv::Point> msg_img;
    // 绿色过滤
    std_msgs::Float64MultiArray msg;
    std::vector<cv::Point2f> corners, last_corners;
    

    imgimg = cv_bridge::toCvShare(img, "bgr8")->image;
    cvtColor(imgimg, corner_img, CV_BGR2GRAY);
    if(!last_imgimg.empty()){
        cvtColor(last_imgimg, last_corner_img, CV_BGR2GRAY);
        goodFeaturesToTrack(last_corner_img, last_corners,
                        100, 0.01, 10,
                        cv::Mat(), 3,
                        false,
                        0.04);
    }
    
    goodFeaturesToTrack(corner_img, corners,
                        100, 0.01, 10,
                        cv::Mat(), 3,
                        false,
                        0.04);
    int num;
    num = std::min(corners.size(), last_corners.size());
    for(int i = 0; i < num; i++){

    }
    
    

    green_img = cv::Mat::zeros(imgimg.size(), imgimg.type());
    cv::cvtColor(imgimg, hsv_img, CV_BGR2HSV);
    cv::inRange(hsv_img, cv::Scalar(35, 250, 160), cv::Scalar(77, 255, 255), mask_img);
    for (int r = 0; r < imgimg.rows; r++)
    {
        for (int c = 0; c < imgimg.cols; c++)
        {
            if (mask_img.at<uchar>(r, c) == 255)
            {
                green_img.at<cv::Vec3b>(r, c)[0] = imgimg.at<cv::Vec3b>(r, c)[0];
                green_img.at<cv::Vec3b>(r, c)[1] = imgimg.at<cv::Vec3b>(r, c)[1];
                green_img.at<cv::Vec3b>(r, c)[2] = imgimg.at<cv::Vec3b>(r, c)[2];
            }
        }
    }
    // 轮廓绘制
    gray_img = cv::Mat::zeros(green_img.size(), CV_32FC1);
    cv::GaussianBlur(green_img, green_img, cv::Size(3, 3), 0, 0, cv::BORDER_DEFAULT);
    cv::cvtColor(green_img, gray_img, CV_BGR2GRAY);
    cv::Scharr(green_img, scharr_imgx, CV_16S, 1, 0, 1, 0, cv::BORDER_DEFAULT);
    cv::convertScaleAbs(scharr_imgx, absscharr_imgx);
    cv::Scharr(green_img, scharr_imgy, CV_16S, 0, 1, 1, 0, cv::BORDER_DEFAULT);
    cv::convertScaleAbs(scharr_imgy, absscharr_imgy);
    cv::addWeighted(absscharr_imgx, 0.5, absscharr_imgy, 0.5, 0, scharr_img);
    // cv::Scharr(imgimg, scharr_imgx, CV_16S, 1, 0, 1, 0, cv::BORDER_DEFAULT);
    // cv::convertScaleAbs(scharr_imgx, absscharr_imgx);
    // cv::Scharr(imgimg, scharr_imgy, CV_16S, 0, 1, 1, 0, cv::BORDER_DEFAULT);
    // cv::convertScaleAbs(scharr_imgy, absscharr_imgy);
    // cv::addWeighted(absscharr_imgx, 1, absscharr_imgy, 1, 0, scharr_img);
    // 边缘检测
    std::vector<std::vector<cv::Point>> squares;
    std::vector<cv::Point> std_squares;
    std::vector<cv::Point> p_squares;
    static std::vector<cv::Point> old_p_squares;
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    std::vector<cv::Point> approx;
    cv::Mat jiao_img0(imgimg.size(), CV_8U), jiao_img;
    int ch[] = {1, 0}, jiao_N = 5;
    cv::mixChannels(&green_img, 1, &jiao_img0, 1, ch, 1);
    for (int l = 0; l < jiao_N; l++)
    {
        if (l == 0)
        {
            cv::dilate(scharr_img, jiao_img, cv::Mat(), cv::Point(-1, -1));
        }
        else
        {
            jiao_img = jiao_img0 >= (l + 0.5) * 255 / jiao_N;
        }
    }
    cv::findContours(jiao_img, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    // 角点检测
    for (int i = 0; i < contours.size(); i++)
    {
        cv::approxPolyDP(cv::Mat(contours[i]), approx, cv::arcLength(cv::Mat(contours[i]), true) * 0.02, true);
        if (approx.size() == 4 && fabs(cv::contourArea(cv::Mat(approx))) > 400 && cv::isContourConvex(cv::Mat(approx)))
        {
            double maxCosine = 0;
            for (int j = 2; j < 5; j++)
            {
                double cosine = fabs(angle(approx[j % 4], approx[j - 2], approx[j - 1]));
                maxCosine = MAX(maxCosine, cosine);
            }
            if (maxCosine < 0.3)
            {
                squares.push_back(approx);
            }
        }
    }
    if (squares.size() != 0)
    {
        // std::cout << squares[0].size() << "个角点" << std::endl;
        for (int i = 0; i < squares[0].size(); i++)
        {
            p_squares.push_back(squares[0][i]);
        }
    }
    else
    {
        // std::cout << 0 << "个角点" << std::endl;
        for (int i = 0; i < 4; i++)
        {
            p_squares.push_back(cv::Point(0, 0));
        }
    }
    for (int i = 0; i < p_squares.size(); i++)
    {
        old_p_squares.push_back(cv::Point(0, 0));
    }
    const cv::Point *p = &p_squares[0];
    int n = (int)p_squares.size();


    // if (p->x > 3 && p->y > 3)
    // {
    //     std::cout << p->x << std::endl;
    //     cv::polylines(imgimg, &p, &n, 1, true, cv::Scalar(0, 0, 255), 3, cv::LINE_AA);
    //     // cv::circle(imgimg, p[0], 2, cv::Scalar(0,0,255),-1);
    //     // cv::circle(imgimg, p[1], 2, cv::Scalar(0,0,255),-1);
    //     // cv::circle(imgimg, p[2], 2, cv::Scalar(0,0,255),-1);
    //     // cv::circle(imgimg, p[3], 2, cv::Scalar(0,0,255),-1);

    //     int num_point = 10;
    //     double temp_x[num_point];
    //     double temp_y[num_point];
    //     double temp_r = std::sqrt((p[0].x-p[1].x)*(p[0].x-p[1].x)+(p[0].y-p[1].y)*(p[0].y-p[1].y));
    //     double temp_cos = (p[0].x-p[1].x) / temp_r;
    //     double temp_sin = (p[0].y-p[1].y) / temp_r;
    //     for (int i = 0; i < num_point; i++)
    //     {
    //         temp_x[i] = p[0].x;
    //         temp_y[i] = p[0].y;
    //     }
    //     for (int i = 0; i < num_point; i++)
    //     {
    //         // temp_x[i] = ((std::rand()%(p[0].x - p[1].x)+1) / temp_cos / temp_cos + p[1].x);
    //         // temp_y[i] = ((std::rand()%(p[0].y - p[1].y)+1) / temp_sin / temp_sin + p[1].y);
    //         // temp_x[i] = ((std::rand()%((p[0].x - p[1].x)*2)+1) + p[1].x - (p[0].x - p[1].x));
    //         // temp_y[i] = ((std::rand()%((p[0].y - p[1].y)*2)+1) + p[1].y - (p[0].y - p[1].y));
    //         std::cout<<temp_y[i]<<std::endl;
    //     }
    //     for (int i = 0; i < num_point; i++)
    //     {
    //         cv::circle(imgimg, cv::Point(temp_x[i],temp_y[i]), 4, cv::Scalar(0,0,255),-1);
    //     }
    // }


    // for (int i = 0; i < corners.size(); i++)
    // {
    //     cv::circle(imgimg, corners[i], 5, cv::Scalar(0,0,255), -1);
    // }




    // 角点匹配
    std_squares.push_back(cv::Point(520, 280));
    std_squares.push_back(cv::Point(520, 520));
    std_squares.push_back(cv::Point(280, 520));
    std_squares.push_back(cv::Point(280, 280));
    const cv::Point *std_p = &std_squares[0];
    int std_n = (int)std_squares.size();
    if (std_p->x > 0 && std_p->y > 0)
    {
        // cv::polylines(imgimg, &std_p, &std_n, 1, true, cv::Scalar(255, 0, 0), 3, cv::LINE_AA);
        // cv::circle(imgimg, std_p[0], 10, cv::Scalar(0,0,255),-1);
    }
    // 记忆p_mean
    if (p_squares[2].x < 1e-3 && p_squares[2].y < 1e-3)
    {
        fuck_count++;
        if (fuck_count > 50)
        {
            cmd_vel.data[0] = 0;
            cmd_vel.data[1] = 0;
            cmd_vel.data[2] = 0;
            cmd_vel.data[5] = 0;
            my_publisher.publish(cmd_vel);
            cv::waitKey(4);
            return;
        }

        for (int i = 0; i < p_squares.size(); i++)
        {
            p_squares[i].x = old_p_squares[i].x;
            p_squares[i].y = old_p_squares[i].y;
        }
    }
    else
    {
        fuck_count = 0;
        for (int i = 0; i < p_squares.size(); i++)
        {

            old_p_squares[i].x = p_squares[i].x;
            old_p_squares[i].y = p_squares[i].y;
        }
    }
    // 计算平移
    cv::Point p_mean, std_mean;
    std::vector<cv::Point>::iterator p_it, std_it;
    static double old_p_mean_x, old_p_mean_y;
    p_mean.x = 0;
    p_mean.y = 0;
    std_mean.x = 0;
    std_mean.y = 0;
    for (p_it = p_squares.begin(); p_it != p_squares.end(); p_it++)
    {
        p_mean.x += p_it->x;
        p_mean.y += p_it->y;
    }
    for (std_it = std_squares.begin(); std_it != std_squares.end(); std_it++)
    {
        std_mean.x += std_it->x;
        std_mean.y += std_it->y;
    }
    p_mean.x = p_mean.x / p_squares.size();
    p_mean.y = p_mean.y / p_squares.size();
    std_mean.x = std_mean.x / std_squares.size();
    std_mean.y = std_mean.y / std_squares.size();
    // std::cout<<p_mean<<std::endl;
    // 计算旋转
    double angle, angle_1, angle_2;
    double min_angle_p = 0;
    double min_angle_std = 0;
    double x1, y1, x2, y2;
    for (int i = 0; i < p_squares.size(); i++)
    {
        x1 = p_squares[i].x - p_mean.x;
        y1 = p_squares[i].y - p_mean.y;
        angle_1 = atan2l(y1, x1);
        if (angle_1 < min_angle_p)
        {
            min_angle_p = angle_1;
        }
        x2 = std_squares[i].x - std_mean.x;
        y2 = std_squares[i].y - std_mean.y;
        angle_2 = atan2l(y2, x2);
        if (angle_2 < min_angle_std)
        {
            min_angle_std = angle_2;
        }
    }

    angle = min_angle_std - min_angle_p;
    double error_x = p_mean.y - std_mean.y;
    // std::cout << error_x << std::endl;
    // static double integra_x += error_x;
    static double last_error_x;
    double diff_x = error_x - last_error_x;
    last_error_x = error_x;
    double v_x = P * error_x * 2 + D * diff_x * 1;
    // Y****************************
    double error_y = p_mean.x - std_mean.x;
    // static double integra_y += error_y;
    static double last_error_y;
    double diff_y = error_y - last_error_y;
    last_error_y = error_y;
    double v_y = P * error_y * 2 + D * diff_y * 1;
    // Z****************************
    double error_z = std::max(sqrt(pow((p_squares[0].x - p_squares[1].x), 2) + pow((p_squares[0].y - p_squares[1].y), 2)),
                              sqrt(pow((p_squares[0].x - p_squares[3].x), 2) + pow((p_squares[0].y - p_squares[3].y), 2))) -
                     sqrt(pow((std_squares[0].x - std_squares[1].x), 2) + pow((std_squares[0].y - std_squares[1].y), 2));
    // static double integra_z += error_z;
    static double last_error_z;
    double diff_z = error_z - last_error_z;
    last_error_z = error_z;
    double v_z = P * error_z * 1 + D * diff_z * 1;
    // wZ***************************
    static double last_error_wz;
    double diff_wz = angle - last_error_wz;
    last_error_wz = angle;
    double v_wz = P * angle * 10000 + D * diff_wz * 1000;

    // std::cout << v_x << " " << v_y << std::endl;
    cmd_vel.data[0] = v_x;
    cmd_vel.data[1] = v_y;
    cmd_vel.data[5] = v_wz;
    if (std::abs(v_x) > 0.01 || abs(v_y) > 0.01)
    {
        v_z = 0;
        // std::cout << "fuck";
    }
    if (fuck_count != 0)
    {
        v_z = 0.08;
    }
    cmd_vel.data[2] = v_z;

    if (p_mean.x < 1e-3 && p_mean.y < 1e-3)
    {
        cmd_vel.data[0] = 0;
        cmd_vel.data[1] = 0;
        cmd_vel.data[2] = 0;
        cmd_vel.data[5] = 0;
    }
    // std::cout << v_x << " " << v_y << " " << v_z << std::endl;
    my_publisher.publish(cmd_vel);
    // cv::line(imgimg, p_mean, std_mean, cv::Scalar(255, 0, 255), 5, cv::LINE_AA);
    // cv::imshow("imgimg", imgimg);
    // cv::imshow("mask_img", mask_img);
    // cv::imshow("green_img", green_img);
    // cv::imshow("scharr_img", scharr_img);
    // cv::imshow("jiao_img", jiao_img);
    // cv::imshow("jiao_img0", jiao_img0);

    std::vector<cv::Point> eight_pts;
    for(int i = 0; i < p_squares.size(); i++){
        eight_pts.push_back(p_squares[i]);
        eight_pts.push_back( cv::Point( p_squares[i].x + (p_squares[(i+1)%4].x - p_squares[i].x) / 3 , p_squares[i].y + (p_squares[(i+1)%4].y - p_squares[i].y) / 3 ) );
        eight_pts.push_back( cv::Point( p_squares[i].x + 2 * (p_squares[(i+1)%4].x - p_squares[i].x) / 3 , p_squares[i].y + 2 * (p_squares[(i+1)%4].y - p_squares[i].y) / 3 ) );
    }
    for(int i = 0; i < eight_pts.size(); i++){
        cv::line(last_imgimg, eight_pts[i], cv::Point(eight_pts[i].x + 1600 * v_y, eight_pts[i].y + 1600 * v_x), cv::Scalar(0, 0, 255), 2);
        // std::cout << 20 * v_y << "----------" << corners[i].y << std::endl;
    }
    if(!last_imgimg.empty()){
        imshow("last_imgimg", last_imgimg);
    }
    last_imgimg = imgimg;
    

    cv::waitKey(4);
}

void myCallbackdepth(const sensor_msgs::ImageConstPtr &img)
{
    cv::Mat depth_img;
    depth_img = cv_bridge::toCvShare(img, "32FC1")->image;
    if(!depth_img.empty()){
        imshow("depth_img", depth_img);
    }
}

int main(int argc, char *argv[])
{
    ros::init(argc, argv, "imgimgimage");
    ros::NodeHandle n1;
    ros::NodeHandle n2;
    for (int i = 0; i < 6; i++)
    {
        cmd_vel.data.push_back(0.0);
    }
    // ros::ServiceClient my_controller_switcher = n.serviceClient<controller_manager_msgs::SwitchController>("/probot_anno/controller_manager/switch_controller");
    // controller_manager_msgs::SwitchController my_controller_cmd;
    ros::Subscriber my_subscriber1 = n1.subscribe("/probot_anno/camera/rgb/image_raw", 1, myCallback);
    ros::Subscriber my_subscriber2 = n2.subscribe("/probot_anno/camera/depth/image_raw", 1, myCallbackdepth);
    my_publisher = n1.advertise<std_msgs::Float64MultiArray>("/cmd_vel", 1);
    int cnt = -1;
    ros::Rate naptime(2);
    // std::vector<std::string> vel_controller{"arm_vel_controller"};
    // std::vector<std::string> pos_controller{"arm_pos_controller"};
    // my_controller_cmd.request.start_controllers = vel_controller;
    // my_controller_cmd.request.stop_controllers = pos_controller;
    // my_controller_cmd.request.strictness = 1;
    // my_controller_switcher.call(my_controller_cmd);
    // std::cout << int(my_controller_cmd.response.ok) << std::endl;
    while (ros::ok())
    {
        // if (cnt == -1)
        // {
        //     my_controller_cmd.request.start_controllers = vel_controller;
        //     my_controller_cmd.request.stop_controllers = pos_controller;
        //     my_controller_cmd.request.strictness = 1;
        //     my_controller_switcher.call(my_controller_cmd);
        //     std::cout << int(my_controller_cmd.response.ok) << std::endl;
        // }
        // if (cnt == 1)
        // {
        //     my_controller_cmd.request.stop_controllers = vel_controller;
        //     my_controller_cmd.request.start_controllers = pos_controller;
        //     my_controller_cmd.request.strictness = 1;
        //     my_controller_switcher.call(my_controller_cmd);
        //     std::cout << int(my_controller_cmd.response.ok) << std::endl;
        // }
        // cnt *= 1;
        // naptime.sleep();
        ros::spinOnce();
    }
    return 0;
}