#ifndef GAZE_TRACKER_PLUGIN_H
#define GAZE_TRACKER_PLUGIN_H

#include <gazebo/math/Rand.hh>
#include <gazebo/gui/GuiIface.hh>
#include <gazebo/rendering/rendering.hh>
#include <gazebo/rendering/Scene.hh>
#include <gazebo/gazebo.hh>

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
#define SERVER "192.168.100.93"
#define SERVPORT 5150

namespace gazebo
{
    class GazeTrackerPlugin: public SystemPlugin
    {
    public:  GazeTrackerPlugin();

    public: ~GazeTrackerPlugin();

    public: void Load(int /*_argc*/, char ** /*_argv*/);

    public: void Update();

    public: void ConnectToServer();

    public: std::string ListenToServer();

    private: void ListenThread();

    private: void CreateSphere();

    private: Ogre::SceneNode *sceneNode;

    private: Ogre::SceneManager *sceneManager;

    private: Ogre::Entity *ogreEntity;

    public: event::ConnectionPtr connection;

    public: rendering::UserCameraPtr mUserCam;

    private: rendering::CameraPtr mCamera;

    public: math::Vector2i coords;

    private: math::Vector3 refSpherePos;

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
#endif // GAZE_TRACKER_CLIENT_H
