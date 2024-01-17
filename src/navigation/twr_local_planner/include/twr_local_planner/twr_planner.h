#include <costmap_2d/costmap_2d_ros.h>
#include <nav_core/base_local_planner.h>
#include <nav_msgs/Path.h>
#include <base_local_planner/local_planner_util.h>

namespace twr_local_planner 
{
    class TWRPlannerROS : public nav_core::BaseLocalPlanner
    {
        public:

            TWRPlannerROS();


            /**
             * @brief 
             * **/
            void initialize(std::string name, tf2_ros::Buffer* tf, costmap_2d::Costmap2DROS* costmap_ros) override;

            bool computeVelocityCommands(geometry_msgs::Twist& cmd_vel) override;

            bool isGoalReached() override;

            bool setPlan(const std::vector<geometry_msgs::PoseStamped>& plan) override;

            void getGlobalPlan(const nav_msgs::Path& global_plan);

            inline bool isInitialized(){ return initialized_; }

        private:

            //void reconfigureCB(DWAPlannerConfig &config, uint32_t level);

            std::vector<geometry_msgs::PoseStamped> path;   /** @brief 储存全局路径规划点*/
            
            bool initialized_;      /** @brief 确认已经被初始化*/

            geometry_msgs::PoseStamped current_pose_;       /** @brief 机器人当前位置*/

            base_local_planner::LocalPlannerUtil planner_util_;
            costmap_2d::Costmap2DROS* costmap_ros_;
    };
}