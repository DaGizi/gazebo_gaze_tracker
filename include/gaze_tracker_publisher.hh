#ifndef GAZE_TRACKER_PUBLISHER_HH
#define GAZE_TRACKER_PUBLISHER_HH

#include <gazebo/gazebo.hh>
#include <gazebo/rendering/RenderingIface.hh>
#include <gazebo/rendering/rendering.hh>
#include <gazebo/transport/TransportIface.hh>
#include <gazebo/msgs/msgs.hh>
#include <gazebo/gui/GuiIface.hh>
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

#include <boost/tokenizer.hpp>
#include <boost/thread.hpp>

#define BufferLength 15

/// \IMPORTANT!!! Every time, check the IP address on the server PC
#define SERVER "192.168.100.75"
#define SERVPORT 5150

namespace gazebo
{
	class GazeTrackerPublisher: public SystemPlugin
	{
	public: GazeTrackerPublisher();

	public: ~GazeTrackerPublisher();

	public: void Load(int /*_argc*/, char ** /*_argv*/);

	public: void Init();

    public: void Update();

    public: void ConnectToServer();

    public: std::string ListenToServer();

    public: void ListenThread();

    public: math::Quaternion QuatFromTwoVectors(math::Vector3 u, math::Vector3 v);

    public: event::ConnectionPtr connection;

    private: gazebo::transport::NodePtr node;

    private: gazebo::transport::PublisherPtr pub;

    public: rendering::UserCameraPtr mUserCam;

    public: math::Vector2i coords;

    private: math::Vector3 contactPoint;

    private: math::Vector3 origin;

    private: math::Vector3 direction;

    private: math::Vector3 cylinderCenter;

    //private: math::Pose cylinderPose;

    private: gazebo::msgs::Pose msg;

    private: int gaze_coords[];

    public: rendering::VisualPtr visual_ptr;

    private: int sd, rc, length;

    private: struct sockaddr_in serveraddr;

    private: char buffer[];

    private: char server[];

    private: int totalcnt;

    private: struct hostent *hostp;

    private: boost::thread *clientThread;
	};
}

#endif
