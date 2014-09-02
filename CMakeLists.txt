cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

include (FindPkgConfig)
if (PKG_CONFIG_FOUND)
  find_package(gazebo REQUIRED)
  find_package(Boost REQUIRED COMPONENTS system)
  pkg_check_modules(OGRE OGRE)
  pkg_check_modules(OGRE-Terrain OGRE-Terrain)
endif()

include_directories(
        ${GAZEBO_INCLUDE_DIRS}
        ${OGRE_INCLUDE_DIRS}
        ${OGRE-Terrain_INCLUDE_DIRS}
        ${Boost_INCLUDE_DIRS}
        ${PROJECT_SOURCE_DIR}/include
)

link_directories(
        ${GAZEBO_LIBRARY_DIRS}
        ${OGRE_LIBRARY_DIRS}
        ${Boost_LIBRARY_DIRS}
)

add_subdirectory(src)
