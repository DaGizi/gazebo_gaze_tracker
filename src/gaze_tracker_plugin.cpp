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
	this->node1 = NULL;
	this->node2 = NULL;
	this->node3 = NULL;
	this->node4 = NULL;
	this->node5 = NULL;
	this->entity1 = NULL;
	this->entity2 = NULL;
	this->entity3 = NULL;
	this->entity4 = NULL;
	this->entity5 = NULL;
//	this->sceneNodes[9];
//	this->entities[9];
//	this->gazeZone[9];
	//this->sceneNodes = new vector<Ogre::SceneNode *>;
//	for(int i = 0; i < 9; i++)
//	{
//		this->sceneNodes[i] = NULL;
//		this->entities[i] = NULL;
//	}
}

GazeTrackerPlugin::~GazeTrackerPlugin() {
	close(this->sd);
	this->connection.reset();//	this->gazeZone[9];
}

void GazeTrackerPlugin::Load(int /*_argc*/, char ** /*_argv*/) {
	cout << "Load" << endl;
	this->ConnectToServer();
	this->connection = event::Events::ConnectPreRender(
			boost::bind(&GazeTrackerPlugin::CreateSphere, this));
	this->clientThread = new boost::thread(
			boost::bind(&GazeTrackerPlugin::ListenThread, this));
}


void GazeTrackerPlugin::Update() {

	/// the gaze tracker works just if gazebo is in fullscreen, in windowed you are loosing pixels
	this->gaze1.x = this->gaze_coords[0];
	this->gaze1.y = this->gaze_coords[1];

	SetGazeCircleCoords(this->gaze1);
//	int angle = 0;
//	int radius = 30;

//	for (int i = 1; i < 9; i++) {
//		this->gazeZone[i].x = this->gazeZone[0].x + radius * cos(angle);
//	this->gazeZone[i].y = this->gazeZone[0].y + radius * sin(angle);
//		angle = angle + 45;
//	}
	this->mUserCam = gui::get_active_camera();
	if((this->mUserCam->GetScene()->GetFirstContact(this->mUserCam, this->gaze1,this->refSpherePos)) == true)
	{
		this->node1->setPosition(this->refSpherePos.x, this->refSpherePos.y, this->refSpherePos.z);
		this->node1->setVisible(true);
		this->entity1->setVisible(true);
	}
	else
	{
		this->node1->setVisible(true);
		this->entity1->setVisible(true);
	}
	if((this->mUserCam->GetScene()->GetFirstContact(this->mUserCam, this->gaze2,this->refSpherePos)) == true)
	{
		this->node2->setPosition(this->refSpherePos.x, this->refSpherePos.y, this->refSpherePos.z);
		this->node2->setVisible(true);
		this->entity2->setVisible(true);
	}
	else
	{
		this->node2->setVisible(true);
		this->entity2->setVisible(true);
	}

	if((this->mUserCam->GetScene()->GetFirstContact(this->mUserCam, this->gaze3,this->refSpherePos)) == true)
	{
		this->node3->setPosition(this->refSpherePos.x, this->refSpherePos.y, this->refSpherePos.z);
		this->node3->setVisible(true);
		this->entity3->setVisible(true);
	}
	else
	{
		this->node3->setVisible(true);
		this->entity3->setVisible(true);
	}
	if((this->mUserCam->GetScene()->GetFirstContact(this->mUserCam, this->gaze4,this->refSpherePos)) == true)
	{
		this->node4->setPosition(this->refSpherePos.x, this->refSpherePos.y, this->refSpherePos.z);
		this->node4->setVisible(true);
		this->entity4->setVisible(true);
	}
	else
	{
		this->node4->setVisible(true);
		this->entity4->setVisible(true);
	}
	if((this->mUserCam->GetScene()->GetFirstContact(this->mUserCam, this->gaze5,this->refSpherePos)) == true)
	{
		this->node5->setPosition(this->refSpherePos.x, this->refSpherePos.y, this->refSpherePos.z);
		this->node5->setVisible(true);
		this->entity5->setVisible(true);
	}
	else
	{
		this->node5->setVisible(true);
		this->entity5->setVisible(true);
	}
	if((this->mUserCam->GetScene()->GetFirstContact(this->mUserCam, this->gaze6,this->refSpherePos)) == true)
	{
		this->node6->setPosition(this->refSpherePos.x, this->refSpherePos.y, this->refSpherePos.z);
		this->node6->setVisible(true);
		this->entity6->setVisible(true);
	}
	else
	{
		this->node6->setVisible(true);
		this->entity6->setVisible(true);
	}
	if((this->mUserCam->GetScene()->GetFirstContact(this->mUserCam, this->gaze7,this->refSpherePos)) == true)
	{
		this->node7->setPosition(this->refSpherePos.x, this->refSpherePos.y, this->refSpherePos.z);
		this->node7->setVisible(true);
		this->entity7->setVisible(true);
	}
	else
	{
		this->node7->setVisible(true);
		this->entity7->setVisible(true);
	}
	if((this->mUserCam->GetScene()->GetFirstContact(this->mUserCam, this->gaze8,this->refSpherePos)) == true)
	{
		this->node8->setPosition(this->refSpherePos.x, this->refSpherePos.y, this->refSpherePos.z);
		this->node8->setVisible(true);
		this->entity8->setVisible(true);
	}
	else
	{
		this->node8->setVisible(true);
		this->entity8->setVisible(true);
	}
	if((this->mUserCam->GetScene()->GetFirstContact(this->mUserCam, this->gaze9,this->refSpherePos)) == true)
	{
		this->node9->setPosition(this->refSpherePos.x, this->refSpherePos.y, this->refSpherePos.z);
		this->node9->setVisible(true);
		this->entity9->setVisible(true);
	}
	else
	{
		this->node9->setVisible(true);
		this->entity9->setVisible(true);
	}
//	for (int i = 0; i < 9; i++) {
//		if (this->mUserCam->GetScene()->GetFirstContact(this->mUserCam,
//				this->gazeZone[i], this->refSpherePos) == true) {
//			this->sceneNodes[i]->setPosition(this->refSpherePos.x,
//					this->refSpherePos.y, this->refSpherePos.z);
//			this->sceneNodes[i]->setVisible(true);
//			this->entities[i]->setVisible(true);
//		} else {
//			this->sceneNodes[i]->setVisible(false);
//			this->entities[i]->setVisible(false);
//		}
//	}
}

void GazeTrackerPlugin::SetGazeCircleCoords(math::Vector2i center)
{
	this->gaze2.x = center.x + 30;
	this->gaze2.y = center.y;

	this->gaze3.x = center.x;
	this->gaze3.y = center.y + 30;

	this->gaze4.x = center.x - 30;
	this->gaze4.y = center.y;

	this->gaze5.x = center.x;
	this->gaze5.y = center.y - 30;

	this->gaze6.x = center.x + 20;
	this->gaze6.y = center.y + 20;

	this->gaze7.x = center.x - 20;
	this->gaze7.y = center.y + 20;

	this->gaze8.x = center.x - 20;
	this->gaze8.y = center.y - 20;

	this->gaze9.x = center.x + 20;
	this->gaze9.y = center.y - 20;

}

void GazeTrackerPlugin::CreateSphere() {
	this->sceneManager = gui::get_active_camera()->GetScene()->GetManager();
	this->node1 = this->sceneManager->getRootSceneNode()->createChildSceneNode("node1");
	this->entity1 = this->sceneManager->createEntity("entity1", Ogre::SceneManager::PT_SPHERE);
	this->entity1->setMaterialName("Gazebo/Red");
	this->node1->setScale(0.0005,0.0005,0.0005);
	this->node1->setPosition(0,0,0);
	this->node1->attachObject(this->entity1);
	this->node1->setVisible(false);
	this->entity1->setVisible(false);

	this->sceneManager = gui::get_active_camera()->GetScene()->GetManager();
	this->node2 = this->sceneManager->getRootSceneNode()->createChildSceneNode("node2");
	this->entity2 = this->sceneManager->createEntity("entity2", Ogre::SceneManager::PT_SPHERE);
	this->entity2->setMaterialName("Gazebo/Red");
	this->node2->setScale(0.0005,0.0005,0.0005);
	this->node2->setPosition(0,0,0);
	this->node2->attachObject(this->entity2);
	this->node2->setVisible(false);
	this->entity2->setVisible(false);

	this->sceneManager = gui::get_active_camera()->GetScene()->GetManager();
	this->node3 = this->sceneManager->getRootSceneNode()->createChildSceneNode("node3");
	this->entity3 = this->sceneManager->createEntity("entity3", Ogre::SceneManager::PT_SPHERE);
	this->entity3->setMaterialName("Gazebo/Red");
	this->node3->setScale(0.0005,0.0005,0.0005);
	this->node3->setPosition(0,0,0);
	this->node3->attachObject(this->entity3);
	this->node3->setVisible(false);
	this->entity3->setVisible(false);

	this->sceneManager = gui::get_active_camera()->GetScene()->GetManager();
	this->node4 = this->sceneManager->getRootSceneNode()->createChildSceneNode("node4");
	this->entity4 = this->sceneManager->createEntity("entity4", Ogre::SceneManager::PT_SPHERE);
	this->entity4->setMaterialName("Gazebo/Red");
	this->node4->setScale(0.0005,0.0005,0.0005);
	this->node4->setPosition(0,0,0);
	this->node4->attachObject(this->entity4);
	this->node4->setVisible(false);
	this->entity4->setVisible(false);

	this->sceneManager = gui::get_active_camera()->GetScene()->GetManager();
	this->node5 = this->sceneManager->getRootSceneNode()->createChildSceneNode("node5");
	this->entity5 = this->sceneManager->createEntity("entity5", Ogre::SceneManager::PT_SPHERE);
	this->entity5->setMaterialName("Gazebo/Red");
	this->node5->setScale(0.0005,0.0005,0.0005);
	this->node5->setPosition(0,0,0);
	this->node5->attachObject(this->entity5);
	this->node5->setVisible(false);
	this->entity5->setVisible(false);

	this->node6 = this->sceneManager->getRootSceneNode()->createChildSceneNode("node6");
	this->entity6 = this->sceneManager->createEntity("entity6", Ogre::SceneManager::PT_SPHERE);
	this->entity6->setMaterialName("Gazebo/Red");
	this->node6->setScale(0.0005,0.0005,0.0005);
	this->node6->setPosition(0,0,0);
	this->node6->attachObject(this->entity6);
	this->node6->setVisible(false);
	this->entity6->setVisible(false);

	this->node7 = this->sceneManager->getRootSceneNode()->createChildSceneNode("node7");
	this->entity7 = this->sceneManager->createEntity("entity7", Ogre::SceneManager::PT_SPHERE);
	this->entity7->setMaterialName("Gazebo/Red");
	this->node7->setScale(0.0005,0.0005,0.0005);
	this->node7->setPosition(0,0,0);
	this->node7->attachObject(this->entity7);
	this->node7->setVisible(false);
	this->entity7->setVisible(false);

	this->node8 = this->sceneManager->getRootSceneNode()->createChildSceneNode("node8");
	this->entity8 = this->sceneManager->createEntity("entity8", Ogre::SceneManager::PT_SPHERE);
	this->entity8->setMaterialName("Gazebo/Red");
	this->node8->setScale(0.0005,0.0005,0.0005);
	this->node8->setPosition(0,0,0);
	this->node8->attachObject(this->entity8);
	this->node8->setVisible(false);
	this->entity1->setVisible(false);

	this->node9 = this->sceneManager->getRootSceneNode()->createChildSceneNode("node9");
	this->entity9 = this->sceneManager->createEntity("entity9", Ogre::SceneManager::PT_SPHERE);
	this->entity9->setMaterialName("Gazebo/Red");
	this->node9->setScale(0.0005,0.0005,0.0005);
	this->node9->setPosition(0,0,0);
	this->node9->attachObject(this->entity9);
	this->node9->setVisible(false);
	this->entity9->setVisible(false);

	this->connection = event::Events::ConnectPreRender(
			boost::bind(&GazeTrackerPlugin::Update, this));
//	this->sceneNode =
//			this->sceneManager->getRootSceneNode()->createChildSceneNode(
//					"gazeSphere");
//	this->ogreEntity = this->sceneManager->createEntity("gazeSphere",
//			Ogre::SceneManager::PT_SPHERE);
//	this->ogreEntity->setMaterialName("Gazebo/Red");
//	this->sceneNode->setScale(0.001, 0.001, 0.001);
//	this->sceneNode->setPosition(0, 0, 0);
//	this->sceneNode->attachObject(this->ogreEntity);
//	this->sceneNode->setVisible(false);
//	this->sceneNode->setVisible(false);
//	this->connection = event::Events::ConnectPreRender(
//			boost::bind(&GazeTrackerPlugin::Update, this));
//	std::stringstream sceneNodeName;
//	std::stringstream entityName;
//		sceneNodeName << "gazeSphere" << i << endl;
//		entityName << "sphereEntity" << i << endl;
//		cout << sceneNodeName.str() << endl;
//		this->sceneNodes[i] =
//		this->sceneManager->getRootSceneNode()->createChildSceneNode(
//				sceneNodeName.str());
//		cout << "SceneNode created!" << endl;
//		this->entities[i] = this->sceneManager->createEntity(entityName.str(),
//				Ogre::SceneManager::PT_SPHERE);
//		cout << "Entity created!" << endl;
//		this->entities[i]->setMaterialName("Gazebo/Red");
//		this->sceneNodes[i]->setScale(0.001, 0.001, 0.001);
//		this->sceneNodes[i]->setPosition(0, 0, 0);
//		this->sceneNodes[i]->attachObject(this->entities[i]);
//		this->sceneNodes[i]->setVisible(false);
//		this->sceneNodes[i]->setVisible(false);
//	std::cout << "Spheres created!" << std::endl;

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
