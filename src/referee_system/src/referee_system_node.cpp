#include <ros/ros.h>
#include <std_msgs/Int32.h>
#include <std_msgs/String.h>
#include <referee_system/referee_system.h>
#include <jsoncpp/json/json.h>

int traverse(std::string s)
{
    int n = 0;
    for (int i = 0; i < s.size(); ++i)
    {
        n = n*10 + s[i]-'0';
    }
    return n;
}

ros::Publisher pub;
void countdownCallback(const std_msgs::String& msg)
{
    std::string referee_system_str=msg.data;
    Json::Value root;  // 创建根节点
    Json::CharReaderBuilder reader;
    std::istringstream jsonStream(referee_system_str);
    std::string parseError;
    Json::parseFromStream(reader, jsonStream, &root, &parseError);
    referee_system::referee_system referee_system_msg;
    referee_system_msg.remaining_time=traverse(root["remaining_time"].asString());
    referee_system_msg.color=traverse(root["color"].asString());
    referee_system_msg.blue_base_blood=traverse(root["blue_base_blood"].asString());
    referee_system_msg.blue_base_protected=traverse(root["blue_base_protected"].asString());
    referee_system_msg.blue_base_shield=traverse(root["blue_base_shield"].asString());
    referee_system_msg.red_base_blood=traverse(root["red_base_blood"].asString());
    referee_system_msg.red_base_proteted=traverse(root["red_base_proteted"].asString());
    referee_system_msg.red_base_shield=traverse(root["red_base_shield"].asString());
    referee_system_msg.r1_blood=traverse(root["r1_blood"].asString());
    referee_system_msg.r1_bullet=traverse(root["r1_bullet"].asString());
    referee_system_msg.b1_blood=traverse(root["b1_blood"].asString());
    referee_system_msg.b1_bullet=traverse(root["b1_bullet"].asString());
    referee_system_msg.r3_blood=traverse(root["r3_blood"].asString());
    referee_system_msg.r3_bullet=traverse(root["r3_bullet"].asString());
    referee_system_msg.b3_blood=traverse(root["b3_blood"].asString());
    referee_system_msg.b3_bullet=traverse(root["b3_bullet"].asString());
    referee_system_msg.r4_blood=traverse(root["r4_blood"].asString());
    referee_system_msg.r4_bullet=traverse(root["r4_bullet"].asString());
    referee_system_msg.b4_blood=traverse(root["b4_blood"].asString());
    referee_system_msg.b4_bullet=traverse(root["b4_bullet"].asString());
    referee_system_msg.r7_blood=traverse(root["r7_blood"].asString());
    referee_system_msg.r7_bullet=traverse(root["r7_bullet"].asString());
    referee_system_msg.b7_blood=traverse(root["b7_blood"].asString());
    referee_system_msg.b7_bullet=traverse(root["b7_bullet"].asString());
    referee_system_msg.benefit_area_open=traverse(root["benefit_area_open"].asString());
    referee_system_msg.red_energy=traverse(root["red_energy"].asString());
    referee_system_msg.blue_energy=traverse(root["blue_energy"].asString());
    
    pub.publish(referee_system_msg);
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "referee_system_publisher");
    ros::NodeHandle nh;
    ros::Subscriber sub = nh.subscribe("refree_system_data", 10, countdownCallback);
    pub=nh.advertise<referee_system::referee_system>("referee_system",10);
    ros::spin();

    return 0;
}