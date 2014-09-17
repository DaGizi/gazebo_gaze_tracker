cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

FIND_PACKAGE( Boost 1.40 COMPONENTS system REQUIRED )
set (CMAKE_CXX_FLAGS "-g -Wall")

include (FindPkgConfig)

set(ode_include_dir /usr/include/gazebo-4.0/gazebo)
set(ode_lib_dir /usr/lib/x86_64-linux-gnu/libgazebo_physics_ode.so)

if (PKG_CONFIG_FOUND)
  find_package(gazebo REQUIRED)
  find_package(Boost REQUIRED COMPONENTS system)
  find_package(Protobuf REQUIRED)
  pkg_check_modules(OGRE OGRE)
  pkg_check_modules(OGRE-Terrain OGRE-Terrain)
endif()

include(FindBoost)
find_package(Boost ${MIN_BOOST_VERSION} REQUIRED system filesystem regex)


include_directories(
		${ode_include_dir}
        ${GAZEBO_INCLUDE_DIRS}
       	${PROTOBUF_INCLUDE_DIRS}
        ${OGRE_INCLUDE_DIRS}
        ${OGRE-Terrain_INCLUDE_DIRS}
        ${Boost_INCLUDE_DIRS}
        ${PROJECT_SOURCE_DIR}/include
)

link_directories(
		${ode_lib_dir}
        ${GAZEBO_LIBRARY_DIRS}
        ${PROTOBUF_INCLUDE_DIRS}
        ${ode_LIBRARY_DIRS}
        ${OGRE_LIBRARY_DIRS}
        ${Boost_LIBRARY_DIRS}
)

add_subdirectory(src)
