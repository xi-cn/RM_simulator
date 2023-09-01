#include <ros/ros.h>
#include <std_msgs/Int32.h>
#include <std_msgs/String.h>
#include <referee_system/referee_system.h>

ros::Publisher pub;
void countdownCallback(const std_msgs::Int32::ConstPtr& msg)
{
    // int min=msg->data/60;
    // int sec=msg->data%60;
    referee_system::referee_system referee_system_msg;
    referee_system_msg.remaining_time=msg->data; //暂时存在bug,无法修改剩余时间
    //TODO:在此处添加ui，并获取ui界面上参数的值
    // referee_system_msg.color=false;
    // referee_system_msg.bule_base_blood=10;
    // referee_system_msg.red_base_blood=2;
    // referee_system_msg.r1_blood=0;
    // referee_system_msg.b1_blood=1;
    // referee_system_msg.r2_blood=2;
    // referee_system_msg.b2_blood=3;
    // referee_system_msg.r3_blood=4;
    // referee_system_msg.b3_blood=5;
    // referee_system_msg.r4_blood=6;
    // referee_system_msg.b4_blood=7;
    // referee_system_msg.r5_blood=8;
    // referee_system_msg.b5_blood=9;
    // referee_system_msg.r6_blood=0;
    // referee_system_msg.b6_blood=1;
    // referee_system_msg.r7_blood=2;
    // referee_system_msg.b7_blood=3;
    // referee_system_msg.r_outpost=0;
    // referee_system_msg.b_outpost=4;
    // referee_system_msg.r_darts=true;
    // referee_system_msg.b_darts=true;
    // referee_system_msg.energy=false;
    // referee_system_msg.bullet_dose=4;
    pub.publish(referee_system_msg);
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "referee_system_publisher");
    ros::NodeHandle nh;
    ros::Subscriber sub = nh.subscribe("countdown", 10, countdownCallback);
    pub=nh.advertise<referee_system::referee_system>("referee_system",10);
    ros::spin();

    return 0;
}