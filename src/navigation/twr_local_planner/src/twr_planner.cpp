#include "twr_local_planner/twr_planner.h"
#include <cmath>
#include <pluginlib/class_list_macros.hpp>


PLUGINLIB_EXPORT_CLASS(twr_local_planner::TWRPlannerROS, nav_core::BaseLocalPlanner);


namespace twr_local_planner
{
    TWRPlannerROS::TWRPlannerROS() : initialized_(false)
    {
    }

    void TWRPlannerROS::initialize(
        std::string name, 
        tf2_ros::Buffer* tf, 
        costmap_2d::Costmap2DROS* costmap_ros)
    {
        if (!isInitialized())
        {
            ros::NodeHandle private_nh("~/" + name);

            costmap_ros_ = costmap_ros;
            costmap_ros_->getRobotPose(current_pose_);

            costmap_2d::Costmap2D* costmap = costmap_ros_->getCostmap();
            planner_util_.initialize(tf, costmap, costmap_ros_->getGlobalFrameID());


            initialized_ = true;

        }
    }

    bool TWRPlannerROS::computeVelocityCommands(geometry_msgs::Twist& cmd_vel)
    {
        geometry_msgs::PoseStamped start = path[0];
        geometry_msgs::PoseStamped end = path[10];

        double x = end.pose.position.x - start.pose.position.x;
        double y = end.pose.position.y - start.pose.position.y;

        double theta = 0;
        if (x == 0 && y >= 0){
            theta = M_PI_2;
        }
        else if (x == 0 && y < 0){
            theta = -M_PI_2;
        }
        else{
            theta = atan(y/x);
            if (x < 0){
                theta += M_PI;
            }
        }

        cmd_vel.linear.x = 0.5 * cos(theta);
        cmd_vel.linear.y = 0.5 * sin(theta);

        return true;
    }

    bool TWRPlannerROS::isGoalReached()
    {
        geometry_msgs::PoseStamped goal;
        planner_util_.getGoal(goal);
        
        double goal_x = goal.pose.position.x;
        double goal_y = goal.pose.position.y;

        costmap_ros_->getRobotPose(current_pose_);
        double robot_x = current_pose_.pose.position.x;
        double robot_y = current_pose_.pose.position.y;

        ROS_INFO("%.2f, %.2f", robot_x, robot_y);
        
        return (robot_x-goal_x) * (robot_x-goal_x) + (robot_y-goal_y) * (robot_y-goal_y) <= 0.04;
    }

    bool TWRPlannerROS::setPlan(const std::vector<geometry_msgs::PoseStamped>& plan)
    {
        planner_util_.setPlan(plan);
        path = plan;
        return true;
    }

    void TWRPlannerROS::getGlobalPlan(const nav_msgs::Path& global_plan)
    {
        path = global_plan.poses;
    }
}