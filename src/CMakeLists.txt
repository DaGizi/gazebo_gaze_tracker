cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

add_library(gaze_tracker_plugin SHARED gaze_tracker_plugin.cpp)
target_link_libraries(gaze_tracker_plugin ${OGRE_LIBRARIES} ${OGRE_LIBRARIES})
