#include <ros/ros.h>
#include <std_msgs/Int32.h>


int main(int argc, char** argv)
{
    ros::init(argc, argv, "countdown_publisher");
    ros::NodeHandle nh;
    ros::Publisher pub = nh.advertise<std_msgs::Int32>("countdown", 10);
    ros::Rate rate(1);  // 发布消息的频率，这里设置为1Hz

    int countdown_time = 6*60;  // 倒计时的总时间
    ROS_INFO("The competition is begain");
    while (ros::ok() && countdown_time >= 0)
    {
        std_msgs::Int32 countdown_msg;
        countdown_msg.data = countdown_time;
        pub.publish(countdown_msg);
        countdown_time--;
        rate.sleep();
    }
    ROS_INFO("The competition is end");
    return 0;
}