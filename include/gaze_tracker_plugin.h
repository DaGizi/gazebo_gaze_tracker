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
#define SERVER "192.168.100.68"
#define SERVPORT 5150

namespace gazebo
{
    class GazeTrackerPlugin: public SystemPlugin
    {
    public:  GazeTrackerPlugin();

    public: ~GazeTrackerPlugin();

    public: void Load(int /*_argc*/, char ** /*_argv*/);

    public: void Update();

    public: void SetGazeCircleCoords(math::Vector2i center);

    public: void ConnectToServer();

    public: std::string ListenToServer();

    private: void ListenThread();

    private: void CreateSphere();

    private: Ogre::SceneNode *node1;

    private: Ogre::SceneNode *node2;

    private: Ogre::SceneNode *node3;

    private: Ogre::SceneNode *node4;

    private: Ogre::SceneNode *node5;

    private: Ogre::SceneNode *node6;

    private: Ogre::SceneNode *node7;

    private: Ogre::SceneNode *node8;

    private: Ogre::SceneNode *node9;

    private: Ogre::Entity *entity1;

    private: Ogre::Entity *entity2;

    private: Ogre::Entity *entity3;

    private: Ogre::Entity *entity4;

    private: Ogre::Entity *entity5;

    private: Ogre::Entity *entity6;

    private: Ogre::Entity *entity7;

    private: Ogre::Entity *entity8;

    private: Ogre::Entity *entity9;

    public: math::Vector2i gaze1;

    public: math::Vector2i gaze2;

    public: math::Vector2i gaze3;

    public: math::Vector2i gaze4;

    public: math::Vector2i gaze5;

    public: math::Vector2i gaze6;

    public: math::Vector2i gaze7;

    public: math::Vector2i gaze8;

    public: math::Vector2i gaze9;

    private: Ogre::SceneManager *sceneManager;

    public: event::ConnectionPtr connection;

//    public: std::vector<Ogre::SceneNode *> sceneNodes (9);
//
//    public: std::vector<Ogre::Entity *> entities (9);

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
