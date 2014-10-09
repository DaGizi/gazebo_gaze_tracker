gazebo_gaze_tracker
===================
# Intro
~~~
Hi everyone!
In this repository you can find the client code for integrating the Tobii EyeX Controller into the Gazebo simulator.
To be able to run the gazebo plugins from this repository, first please go to https://github.com/DaGizi/gaze_tracker_win
and set up the server code on a windows pc.
~~~
# gaze_tracker_plugin
##NOTE!!!
~~~
1. On the windows pc, on which you are running the gaze tracker, check the IPv4 address of the pc
2. On the ubuntu pc, open the include/gaze_tracker_plugin.hh header file
3. Change the SERVER macro to the windows pc's IPv4 address
4. Build the plugin
~~~
## build
~~~
mkdir build
cd build
cmake ..
make
~~~
## run
~~~

~~~
