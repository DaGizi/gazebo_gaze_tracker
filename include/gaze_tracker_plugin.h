#ifndef GAZE_TRACKER_PLUGIN_H
#define GAZE_TRACKER_PLUGIN_H

#include <gazebo/math/Rand.hh>
#include <gazebo/gui/GuiIface.hh>
#include <gazebo/rendering/rendering.hh>
#include <gazebo/gazebo.hh>
#include <gazebo/gui/GuiEvents.hh>
#include <gazebo/physics/physics.hh>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>

#define BufferLength 15
#define SERVER "192.168.100.72"
#define SERVPORT 5150

namespace gazebo
{
    class GazeTrackerClient: public WorldPlugin
    {
    public: ~GazeTrackerClient();

    public: void Load(physics::WorldPtr _parent, sdf::ElementPtr /*_sdf*/);

    public: void Update();

    public: void ConnectToServer();

    public: void ListenToServer();

    public: event::ConnectionPtr connection;

    public: rendering::UserCameraPtr mUserCam;

    public: int x, y;

    public: math::Vector2i coords;

    public: std::vector<rendering::VisualPtr> visualVect;

    public: rendering::VisualPtr visual_ptr;

    public: int sd, rc, length = sizeof(int);
    public: struct sockaddr_in serveraddr;
    public: char buffer[BufferLength];
    public: char server[255];
    public: int totalcnt = 0;
    public: struct hostent *hostp;
    };
}
#endif // GAZE_TRACKER_CLIENT_H
