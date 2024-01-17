#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <std_msgs/Int16.h>
#include <tele_control/tele.h>

class TeleControl
{
public:
  TeleControl();
  double linear_, angular_, l_accel_, a_accel_, l_resis_, a_resis_;
  double l_max_, a_max_;
  double l_last_ = 0, a_last = 0;
  int key = 0;
};

TeleControl::TeleControl()
{
  linear_=0,
  angular_=0,
  l_accel_=3.0,
  a_accel_=20.0,
  l_resis_=1.0,
  a_resis_=100.0,
  l_max_=5.0,
  a_max_=200.0;
  //nh_.param("scale_angular", a_scale_, a_scale_);
  //nh_.param("scale_linear", l_scale_, l_scale_);

  //twist_pub_ = nh_.advertise<geometry_msgs::Twist>(name + "/cmd_vel", 1);
}

TeleControl tel;
double time_last;
double time_now;

void keyread_callback(std_msgs::Int16 key)
{
  double time = ros::Time::now().toSec();
  if (key.data == 1)
  {
    tel.a_last = time;
    tel.key = 1;
  }
  else if (key.data == 2)
  {
    tel.a_last = time;
    tel.key = 2;
  }
  else if (key.data == 3)
  {
    tel.l_last_ = time;
    tel.key = 3;
  }
  else
  {
    tel.l_last_ = time;
    tel.key = 4;
  }
}

int main(int argc, char *argv[])
{
    ros::init(argc, argv, "tele_control");
    std::string name = "";
    if(argc > 2){
      puts("usages: handlebar robot_name");
      return 0;
    }
    else if (argc == 2){
    name = std::string(argv[1]);
    }
    
    ros::NodeHandle nh;
    ros::Subscriber sub = nh.subscribe("keyboard_read", 10, keyread_callback);
    ros::Publisher pub = nh.advertise<geometry_msgs::Twist>(name+"/cmd_vel",1);

    ros::Time time;
    geometry_msgs::Twist twist;
    twist.linear.x = 0;
    twist.linear.y = 0;
    twist.linear.z = 0;
    twist.angular.x = 0;
    twist.angular.y = 0;
    twist.angular.z = 0;

    while (ros::ok)
    {
      time = ros::Time::now();

      if(time.toSec() - time_now < 0.05)
        continue;

      time_last = time_now;
      time_now = time.toSec();

      if (time_now - tel.l_last_ >= 0.1)
      {
        if (abs(tel.linear_) - tel.l_resis_*(time_now - time_last) <= 0)
          tel.linear_ = 0;
        else{
          int coef = tel.linear_ >= 0 ? 1 : -1;
          tel.linear_ = (abs(tel.linear_) - tel.l_resis_*(time_now - time_last)) * coef;
        }
      }
      else if (tel.key == 3){
        double acce = 0;
        if (tel.linear_ < 0)
          acce = tel.l_accel_ + tel.l_resis_;
        else
          acce = tel.l_accel_ - tel.l_resis_;
        tel.linear_ += acce * (time_now - time_last);
        tel.linear_ = tel.linear_ >= tel.l_max_ ? tel.l_max_ : tel.linear_;
      }
      else if (tel.key == 4){
        double acce = 0;
        if (tel.linear_ > 0)
          acce = tel.l_accel_ + tel.l_resis_;
        else
          acce = tel.l_accel_ - tel.l_resis_;
        tel.linear_ -= acce * (time_now - time_last);
        tel.linear_ = tel.linear_ <= tel.l_max_*-1 ? tel.l_max_*-1 : tel.linear_;
      }

      if (time_now - tel.a_last >= 0.1)
      {
        if (abs(tel.angular_) - tel.a_resis_*(time_now - time_last) <= 0)
          tel.angular_ = 0;
        else{
          int coef = tel.angular_ >= 0 ? 1 : -1;
          tel.angular_ = (abs(tel.angular_) - tel.a_resis_*(time_now - time_last)) * coef;
        }
      }
      else if (tel.key == 1){
        tel.angular_ += tel.a_accel_ * (time_now - time_last);
        tel.angular_ = tel.angular_ >= tel.a_max_ ? tel.a_max_ : tel.angular_;
      }
      else if (tel.key == 2){
        tel.angular_ -= tel.a_accel_ * (time_now - time_last);
        tel.angular_ = tel.angular_ <= tel.a_max_*-1 ? tel.a_max_*-1 : tel.angular_;
      }
      
      twist.linear.x = tel.linear_;
      twist.angular.z = tel.angular_;
      
      if (time_now - tel.l_last_ <= 3 || time_now - tel.a_last <= 3){
        pub.publish(twist);
        //ROS_INFO("%f", tel.linear_);
      }
      ros::spinOnce();
    }
    return 0;
}


// #include <ros/ros.h>
// #include <geometry_msgs/Twist.h>
// #include <cmath>

// void twistCallback(const geometry_msgs::Twist &msg)
// {
//   double x = msg.linear.x;
//   double y = msg.linear.y;
//   double v = sqrt(x*x + y*y);
//   ROS_INFO("%.2f", v);
// }

// int main(int argc, char *argv[])
// {
//   ros::init(argc, argv, "read_speed");
//   ros::NodeHandle nh;
//   ros::Subscriber sub = nh.subscribe("/cmd_vel", 10, twistCallback);
//   ros::spin();
//   return 0;
// }
