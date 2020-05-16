#include <visp3/gui/vpDisplayGDI.h>
#include <visp3/gui/vpDisplayOpenCV.h>
#include <visp3/gui/vpDisplayX.h>
#include <visp3/gui/vpProjectionDisplay.h>
#include <visp3/robot/vpSimulatorCamera.h>
#include <visp3/visual_features/vpFeatureBuilder.h>
#include <visp3/vs/vpServo.h>
#include <visp3/vs/vpServoDisplay.h>
#include <ros/ros.h>
#include <sensor_msgs/Image.h>
#include "std_msgs/Float64MultiArray.h"

#define NUM 4
vpColVector v;

void myCallback(const sensor_msgs::ImageConstPtr &img)
{
}

void display_trajectory(const vpImage<unsigned char> &I, std::vector<vpPoint> &point, const vpHomogeneousMatrix &cMo,
                        const vpCameraParameters &cam)
{
    static std::vector<vpImagePoint> traj[NUM];
    vpImagePoint cog;
    for (unsigned int i = 0; i < NUM; i++)
    {
        point[i].project(cMo);
        vpMeterPixelConversion::convertPoint(cam, point[i].get_x(), point[i].get_y(), cog);
        traj[i].push_back(cog);
    }
    for (unsigned int i = 0; i < NUM; i++)
    {
        for (unsigned int j = 1; j < traj[i].size(); j++)
        {
            vpDisplay::displayLine(I, traj[i][j - 1], traj[i][j], vpColor::green);
        }
    }
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "ibvs_analysis");
    ros::NodeHandle n;
    ros::Subscriber my_subscriber = n.subscribe("/probot_anno/camera/image_raw", 1, myCallback);
    ros::Publisher my_publisher = n.advertise<std_msgs::Float64MultiArray>("/cmdvel", 100);
    std_msgs::Float64MultiArray msg;
    for (int i = 0; i < 3; i++)
    {
        msg.data.push_back(0);
    }
    vpHomogeneousMatrix cdMo(0, 0, 0.75, 0, 0, 0);
    vpHomogeneousMatrix cMo(0.15, -0.1, 1., vpMath::rad(10), vpMath::rad(-10), vpMath::rad(50));
    std::vector<vpPoint> point;
    point.push_back(vpPoint(0.1, -0.1, 0));
    point.push_back(vpPoint(0.1, 0.1, 0));
    point.push_back(vpPoint(-0.1, 0.1, 0));
    point.push_back(vpPoint(-0.1, -0.1, 0));
    vpServo task;
    task.setServo(vpServo::EYEINHAND_CAMERA);
    task.setInteractionMatrixType(vpServo::CURRENT);
    task.setLambda(0.5);
    vpFeaturePoint p[NUM], pd[NUM];
    for (unsigned int i = 0; i < NUM; i++)
    {
        point[i].track(cdMo);
        vpFeatureBuilder::create(pd[i], point[i]);
        point[i].track(cMo);
        vpFeatureBuilder::create(p[i], point[i]);
        task.addFeature(p[i], pd[i]);
    }
    vpHomogeneousMatrix wMc, wMo;
    vpSimulatorCamera robot;
    robot.setSamplingTime(0.040);
    robot.getPosition(wMc);
    wMo = wMc * cMo;
    vpImage<unsigned char> Iint(480, 640, 255);
    vpImage<unsigned char> Iext(480, 640, 255);
    vpDisplayX displayInt(Iint, 0, 0, "Internal view");
    vpDisplayX displayExt(Iext, 670, 0, "External view");
    vpProjectionDisplay externalview;
    for (unsigned int i = 0; i < NUM; i++)
        externalview.insert(point[i]);
    vpCameraParameters cam(840, 840, Iint.getWidth() / 2, Iint.getHeight() / 2);
    vpHomogeneousMatrix cextMo(0, 0, 3, 0, 0, 0);
    while (ros::ok())
    {
        ros::spinOnce();

        robot.getPosition(wMc);
        cMo = wMc.inverse() * wMo;
        for (unsigned int i = 0; i < NUM; i++)
        {
            point[i].track(cMo);
            vpFeatureBuilder::create(p[i], point[i]);
        }
        v = task.computeControlLaw();
        robot.setVelocity(vpRobot::CAMERA_FRAME, v);
        for (int i = 0; i < 3; i++)
        {
            msg.data.at(i) = v.data[i];
        }
        my_publisher.publish(msg);
        // std::cout << "----------------------" << std::endl;
        // std::cout << v << std::endl;
        vpDisplay::display(Iint);
        vpDisplay::display(Iext);
        display_trajectory(Iint, point, cMo, cam);
        vpServoDisplay::display(task, cam, Iint, vpColor::green, vpColor::red);
        externalview.display(Iext, cextMo, cMo, cam, vpColor::red, true);
        vpDisplay::flush(Iint);
        vpDisplay::flush(Iext);
        // A click to exit
        if (vpDisplay::getClick(Iint, false) || vpDisplay::getClick(Iext, false))
            break;
        // vpTime::wait(robot.getSamplingTime() * 1000);
    }
    task.kill();
}
