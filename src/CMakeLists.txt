cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

add_library(gaze_tracker_plugin SHARED gaze_tracker_plugin.cpp)
target_link_libraries(gaze_tracker_plugin ${GAZEBO_LIBRARIES} ${OGRE_LIBRARIES})

add_library(gaze_tracker_publisher SHARED gaze_tracker_publisher.cpp)
target_link_libraries(gaze_tracker_publisher ${GAZEBO_LIBRARIES} ${Boost_LIBRARIES} ${PROTOBUF_LIBRARIES})

add_library(cylinder_controller SHARED cylinder_controller.cpp)
target_link_libraries(cylinder_controller ${GAZEBO_LIBRARIES} ${Boost_LIBRARIES} ${PROTOBUF_LIBRARIES})
