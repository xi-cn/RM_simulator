cmake_minimum_required(VERSION 3.0.2)
project(twr_local_planner)

find_package(catkin REQUIRED
        COMPONENTS
            base_local_planner
            cmake_modules
            costmap_2d
            nav_core
            nav_msgs
            pluginlib
            roscpp
            tf2
            tf2_geometry_msgs
            tf2_ros
        )

remove_definitions(-DDISABLE_LIBUSB-1.0)
include_directories(
    include
    ${catkin_INCLUDE_DIRS}
    ${EIGEN3_INCLUDE_DIRS}
    )
add_definitions(${EIGEN3_DEFINITIONS})

# dynamic reconfigure
# generate_dynamic_reconfigure_options(
#     cfg/DWAPlanner.cfg
# )

catkin_package(
    INCLUDE_DIRS include
    LIBRARIES twr_local_planner
    CATKIN_DEPENDS
        base_local_planner
        nav_msgs
        pluginlib
        roscpp
        tf2
        tf2_ros
)

add_library(twr_local_planner src/twr_planner.cpp)
add_dependencies(twr_local_planner ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(twr_local_planner ${catkin_LIBRARIES})

install(TARGETS twr_local_planner
       ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
       LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
       RUNTIME DESTINATION ${CATKIN_GLOBAL_BIN_DESTINATION}
       )

install(FILES twr_local_planner_plugin.xml
    DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)

install(DIRECTORY include/${PROJECT_NAME}/
  DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
  PATTERN ".svn" EXCLUDE
)
