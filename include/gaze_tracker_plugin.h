#ifndef GAZE_TRACKER_PLUGIN_H
#define GAZE_TRACKER_PLUGIN_H

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
#define SERVER "192.168.100.75"
#define SERVPORT 5150

namespace gazebo
{
    class GazeTrackerPlugin: public SystemPlugin
    {
    public:  GazeTrackerPlugin();

    public: ~GazeTrackerPlugin();

    public: void Load(int /*_argc*/, char ** /*_argv*/);

    public: void Update();

    public: void SetGazeCircleCoords(std::vector<math::Vector2i> &gazeCoords,math::Vector2i center);

    public: void ConnectToServer();

    public: std::string ListenToServer();

    private: void ListenThread();

    private: void CreateSpheres();

    public: math::Vector2i gaze1;

    private: Ogre::SceneManager *sceneManager;

    public: event::ConnectionPtr connection;

    public: std::vector<Ogre::SceneNode *> sceneNodes;

    public: std::vector<Ogre::Entity *> entities;

    private: std::vector<math::Vector2i> gazePoints;

    public: rendering::UserCameraPtr mUserCam;

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
