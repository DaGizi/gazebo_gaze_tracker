#include <gaze_tracker_plugin.h>

using namespace gazebo;
using namespace std;

GazeTrackerPlugin::GazeTrackerPlugin()
{
	this->gaze_coords[2];
	this->sd = sizeof(int);
	this->rc = sizeof(int);
	this->length = sizeof(int);
	this->buffer[BufferLength];
	this->server[255];
	this->totalcnt = 0;
	this->refSpherePos = gazebo::math::Vector3(0, 0, 0);
	this->sceneNode = NULL;
	this->ogreEntity = NULL;
}

GazeTrackerPlugin::~GazeTrackerPlugin()
{
	close(this->sd);
	this->connection.reset();
}

void GazeTrackerPlugin::Load(int /*_argc*/, char ** /*_argv*/)
{
	cout << "Load" << endl;
	this->ConnectToServer();
	this->connection = event::Events::ConnectPreRender(
			boost::bind(&GazeTrackerPlugin::CreateSphere, this));
	this->clientThread = new boost::thread(
			boost::bind(&GazeTrackerPlugin::ListenThread, this));
}

void GazeTrackerPlugin::Update()
{
	/// the gaze tracker works just if gazebo is in fullscreen, in windowed you are loosing pixels
	this->coords.x = this->gaze_coords[0];
	this->coords.y = this->gaze_coords[1];
	this->mUserCam = gui::get_active_camera();
	if (this->mUserCam->GetScene()->GetFirstContact(this->mUserCam,
			this->coords, this->refSpherePos) == true) {
		this->sceneNode->setPosition(this->refSpherePos.x, this->refSpherePos.y,
				this->refSpherePos.z);
		this->sceneNode->setVisible(true);
		this->ogreEntity->setVisible(true);
	} else {
		this->sceneNode->setVisible(false);
		this->ogreEntity->setVisible(false);
		//cout << "no contact...." << endl;
	}
}

void GazeTrackerPlugin::CreateSphere() {
	this->sceneManager = gui::get_active_camera()->GetScene()->GetManager();
	this->sceneNode =
			this->sceneManager->getRootSceneNode()->createChildSceneNode(
					"gazeSphere");
	this->ogreEntity = this->sceneManager->createEntity("gazeSphere",
			Ogre::SceneManager::PT_SPHERE);
	this->ogreEntity->setMaterialName("Gazebo/Red");
	this->sceneNode->setScale(0.001, 0.001, 0.001);
	this->sceneNode->setPosition(0, 0, 0);
	this->sceneNode->attachObject(this->ogreEntity);
	this->sceneNode->setVisible(false);
	this->sceneNode->setVisible(false);
	this->connection = event::Events::ConnectPreRender(
			boost::bind(&GazeTrackerPlugin::Update, this));
}

void GazeTrackerPlugin::ListenThread()
{
	boost::posix_time::millisec workTime(1);
	while (1) {
		string server_buffer = GazeTrackerPlugin::ListenToServer();
		int i = 0;
		boost::char_separator<char> sep(",");
		boost::tokenizer<boost::char_separator<char> > tok(server_buffer, sep);
		for (boost::tokenizer<boost::char_separator<char> >::iterator tok_iter =
				tok.begin(); tok_iter != tok.end(); tok_iter++)
		{
			//TODO to fix
			std::string str = *tok_iter;
			this->gaze_coords[i] = atoi(str.c_str());
			i++;
		}
		boost::this_thread::sleep(workTime);
	}
}

void GazeTrackerPlugin::ConnectToServer()
{
	if ((this->sd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("Client-socket() error");
		exit(-1);
	} else
		printf("Client-socket() OK\n");

	strcpy(this->server, SERVER);
	memset(&serveraddr, 0x00, sizeof(struct sockaddr_in));
	this->serveraddr.sin_family = AF_INET;
	this->serveraddr.sin_port = htons(SERVPORT);

	if ((this->serveraddr.sin_addr.s_addr = inet_addr(this->server))
			== (unsigned long) INADDR_NONE )
	{
		this->hostp = gethostbyname(server);
		if (this->hostp == (struct hostent *) NULL)
		{
			printf("HOST NOT FOUND --> ");
			printf("h_errno = %d\n", h_errno);
			printf("---This is a client program---\n");
			printf("Command usage: <server name or IP>\n");
			close(sd);
			exit(-1);
		}
		memcpy(&serveraddr.sin_addr, this->hostp->h_addr, sizeof(this->serveraddr.sin_addr));
	}
	if ((this->rc = connect(sd, (struct sockaddr *) &serveraddr,
			sizeof(this->serveraddr))) < 0) {
		perror("Client-connect() error");
		close(this->sd);
		exit(-1);
	} else
		printf("Connection established...\n");
}

string GazeTrackerPlugin::ListenToServer()
{
	this->rc = read(this->sd, &buffer[0], BufferLength);
	if (this->rc < 0)
	{
		perror("Client-read() error");
		close(sd);
		exit(-1);
	} else if (this->rc == 0)
	{
		printf("Server program has issued a close()\n");
	}
	return buffer;
}

GZ_REGISTER_SYSTEM_PLUGIN(GazeTrackerPlugin)
