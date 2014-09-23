#include <gaze_tracker_plugin.h>

using namespace gazebo;
using namespace std;

GazeTrackerPlugin::GazeTrackerPlugin() {
	this->gaze_coords[2];
	this->sd = sizeof(int);
	this->rc = sizeof(int);
	this->length = sizeof(int);
	this->buffer[BufferLength];
	this->server[255];
	this->totalcnt = 0;
	for(int i = 0; i < 9; i++)
	{
		this->gazePoints.push_back(math::Vector2i(0,0));
	}
}

GazeTrackerPlugin::~GazeTrackerPlugin() {
	close(this->sd);
	this->connection.reset();//	this->gazeZone[9];
}

void GazeTrackerPlugin::Load(int /*_argc*/, char ** /*_argv*/) {
	cout << "Load" << endl;
	this->ConnectToServer();
	this->connection = event::Events::ConnectPreRender(
			boost::bind(&GazeTrackerPlugin::CreateSpheres, this));
	this->clientThread = new boost::thread(
			boost::bind(&GazeTrackerPlugin::ListenThread, this));

}


void GazeTrackerPlugin::Update() {
	/// the gaze tracker works just if gazebo is in fullscreen, in windowed you are loosing pixels
	this->gaze1.x = this->gaze_coords[0];
	this->gaze1.y = this->gaze_coords[1];

	SetGazeCircleCoords(this->gazePoints,this->gaze1);
	this->mUserCam = gui::get_active_camera();
	for(int i = 0; i < this->sceneNodes.size(); i++)
	{
		if((this->mUserCam->GetScene()->GetFirstContact(this->mUserCam, this->gazePoints.at(i), this->refSpherePos)) == true)
		{
			this->sceneNodes.at(i)->setPosition(this->refSpherePos.x, this->refSpherePos.y, this->refSpherePos.z);
			this->sceneNodes.at(i)->setVisible(true);
			this->entities.at(i)->setVisible(true);
		}
		else
		{
			this->sceneNodes.at(i)->setVisible(false);
			this->entities.at(i)->setVisible(false);
		}
		if((this->mUserCam->GetVisual(this->gazePoints.at(i))) != NULL)

		{
			cout << "Got Visual : " <<  this->mUserCam->GetVisual(this->gazePoints.at(i))->GetName() << endl;
		}
		else
		{
		}
//		if((this->mUserCam->GetScene()->GetVisualAt(this->mUserCam,this->gazePoints.at(i))) != NULL)
//		{
//			cout << "Visual : " << this->mUserCam->GetScene()->GetVisualAt(this->mUserCam,this->gazePoints.at(i))->GetName() << endl;
//		}

	}
}


void GazeTrackerPlugin::SetGazeCircleCoords(std::vector<math::Vector2i> &gazeCoords, math::Vector2i center)
{
	gazeCoords.at(0) = center;

	gazeCoords.at(1).x = center.x + 30;
	gazeCoords.at(1).y = center.y;

	gazeCoords.at(2).x = center.x;
	gazeCoords.at(2).y = center.y + 30;

	gazeCoords.at(3).x = center.x - 30;
	gazeCoords.at(3).y = center.y;

	gazeCoords.at(4).x  = center.x;
	gazeCoords.at(4).y  = center.y - 30;

	gazeCoords.at(5).x  = center.x + 20;
	gazeCoords.at(5).y  = center.y + 20;

	gazeCoords.at(6).x  = center.x - 20;
	gazeCoords.at(6).y  = center.y + 20;

	gazeCoords.at(7).x  = center.x - 20;
	gazeCoords.at(7).y  = center.y - 20;

	gazeCoords.at(8).x  = center.x + 20;
	gazeCoords.at(8).y  = center.y - 20;

}

void GazeTrackerPlugin::CreateSpheres() {
	int n = 9;
	std::stringstream nodeName;
	std::stringstream entityName;
	this->sceneManager = gui::get_active_camera()->GetScene()->GetManager();
	for(int i = 0; i < n; i++)
	{
		nodeName << "node" << i << endl;
		entityName << "entity" << i << endl;
		this->sceneNodes.push_back(this->sceneManager->getRootSceneNode()->createChildSceneNode(nodeName.str()));
		this->entities.push_back(this->sceneManager->createEntity(entityName.str(), Ogre::SceneManager::PT_SPHERE));
		//cout << this->entities.at(i)->getName();
		this->entities.at(i)->setMaterialName("Gazebo/Red");
		this->sceneNodes.at(i)->setScale(0.0005,0.0005,0.0005);
		this->sceneNodes.at(i)->setPosition(0,0,0);
		this->sceneNodes.at(i)->attachObject(this->entities.at(i));
		this->sceneNodes.at(i)->setVisible(false);
		this->entities.at(i)->setVisible(false);
	}
	this->connection = event::Events::ConnectPreRender(
			boost::bind(&GazeTrackerPlugin::Update, this));
}

void GazeTrackerPlugin::ListenThread() {
	boost::posix_time::millisec workTime(1);
	while (1) {
		string server_buffer = GazeTrackerPlugin::ListenToServer();
		int i = 0;
		boost::char_separator<char> sep(",");
		boost::tokenizer<boost::char_separator<char> > tok(server_buffer, sep);
		for (boost::tokenizer<boost::char_separator<char> >::iterator tok_iter =
				tok.begin(); tok_iter != tok.end(); tok_iter++) {
			//TODO to fix
			std::string str = *tok_iter;
			this->gaze_coords[i] = atoi(str.c_str());
			i++;
		}
		boost::this_thread::sleep(workTime);
	}
}

void GazeTrackerPlugin::ConnectToServer() {
	if ((this->sd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("Client-socket() error");
		exit(-1);
	} else
		printf("Client-socket() OK\n");

	strcpy(this->server, SERVER);
	memset(&serveraddr, 0x00, sizeof(struct sockaddr_in));
	this->serveraddr.sin_family = AF_INET;
	this->serveraddr.sin_port = htons(SERVPORT);

	if ((this->serveraddr.sin_addr.s_addr = inet_addr(this->server))
			== (unsigned long) INADDR_NONE ) {
		this->hostp = gethostbyname(server);
		if (this->hostp == (struct hostent *) NULL) {
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

string GazeTrackerPlugin::ListenToServer() {
	this->rc = read(this->sd, &buffer[0], BufferLength);
	if (this->rc < 0) {
		perror("Client-read() error");
		close(sd);
		exit(-1);
	} else if (this->rc == 0) {
		printf("Server program has issued a close()\n");
	}
	return buffer;
}

GZ_REGISTER_SYSTEM_PLUGIN(GazeTrackerPlugin)
