#include "gaze_tracker_publisher.hh"

using namespace gazebo;
using namespace std;

GazeTrackerPublisher::GazeTrackerPublisher() {
	this->gaze_coords[2];
	this->sd = sizeof(int);
	this->rc = sizeof(int);
	this->length = sizeof(int);
	this->buffer[BufferLength];
	this->server[255];
	this->totalcnt = 0;
}

GazeTrackerPublisher::~GazeTrackerPublisher()
{
	close(this->sd);
	this->connection.reset();
}

void GazeTrackerPublisher::Load(int /*_argc*/, char ** /*_argv*/) {
	this->ConnectToServer();
	this->connection = event::Events::ConnectPreRender(
			boost::bind(&GazeTrackerPublisher::Update, this));
	this->clientThread = new boost::thread(
			boost::bind(&GazeTrackerPublisher::ListenThread, this));
}

void GazeTrackerPublisher::Init()
{
	math::Quaternion q;

	this->node = transport::NodePtr(new transport::Node());
	this->node->Init("empty_world");
	this->pub = this->node->Advertise<gazebo::msgs::Pose>("~/gaze_cylinder_pose");
}
void GazeTrackerPublisher::Update() {
	this->coords.x = this->gaze_coords[0];
	this->coords.y = this->gaze_coords[1];
	this->mUserCam = gui::get_active_camera();
	this->mUserCam->GetCameraToViewportRay(this->coords.x,this->coords.y,this->origin,this->direction);
//	this->cylinderCenter.x = (this->origin.x + this->direction.x)/2;
//	this->cylinderCenter.y = (this->origin.y + this->direction.y)/2;
//	this->cylinderCenter.z = (this->origin.z + this->direction.z)/2;
	this->mUserCam->GetWorldPosition();
	if (this->mUserCam->GetScene()->GetFirstContact(this->mUserCam,
			this->coords, this->contactPoint)) {
		math::Quaternion quat = GazeTrackerPublisher::QuatFromTwoVectors(this->origin,this->contactPoint);
		math::Quaternion camQuat =  this->mUserCam->GetWorldRotation();
		camQuat.x = camQuat.x + 0.9;
		camQuat.y = camQuat.y + 1.5;
		camQuat.z = camQuat.z + 1;
		this->cylinderCenter.x = (this->origin.x + this->contactPoint.x)/2;
		this->cylinderCenter.y = (this->origin.y + this->contactPoint.y)/2;
		this->cylinderCenter.z = (this->origin.z + this->contactPoint.z)/2;
		math::Pose cylPose = math::Pose(this->cylinderCenter, camQuat);
		gazebo::msgs::Set(&msg, cylPose);
		this->pub->Publish(this->msg);
	}
	else
	{
	}
}

math::Quaternion GazeTrackerPublisher::QuatFromTwoVectors(math::Vector3 u, math::Vector3 v)
{
	math::Vector3 a = u.Cross(v);
	math::Quaternion q;
	q.x = a.x;
	q.y = a.y;
	q.z = a.z;
	double vLength = v.GetLength();
	double uLength = u.GetLength();
	q.w = sqrt((uLength*uLength)*(vLength*vLength))+ u.Dot(v);
	q.Normalize();
	return q;
//	float norm_u_norm_v = sqrt((u.x*u.x + u.y*u.y + u.z*u.z)*(v.x*v.x + v.y*v.y + v.z*v.z));
//	float real_part = norm_u_norm_v + (u.x*v.x + u.y*v.y + u.z*v.z);
//	math::Vector3 w;
//	if(real_part < 1.e-6f * norm_u_norm_v)
//	{
//		real_part = 0.0f;
//		w = abs(u.x) > abs(u.z) ? math::Vector3(-u.y,u.x,0.f) : math::Vector3(0.f, -u.z, u.y);
//	}
//	else
//	{
//		w = math::Vector3((u.y*v.z - u.z*v.y), (u.z*v.x - u.x*v.z),(u.x*v.y - u.y*v.x));
//	}
//	return math::Quaternion(real_part,w.x,w.y,w.z);
}

void GazeTrackerPublisher::ListenThread() {
	boost::posix_time::millisec workTime(1);
	while (1) {
		string server_buffer = GazeTrackerPublisher::ListenToServer();
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

void GazeTrackerPublisher::ConnectToServer() {
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
			== (unsigned long) INADDR_NONE) {
		this->hostp = gethostbyname(server);
		if (this->hostp == (struct hostent *) NULL) {
			printf("HOST NOT FOUND --> ");
			printf("h_errno = %d\n", h_errno);
			printf("---This is a client program---\n");
			printf("Command usage: <server name or IP>\n");
			close(sd);
			exit(-1);
		}
		memcpy(&serveraddr.sin_addr, this->hostp->h_addr,
				sizeof(this->serveraddr.sin_addr));
	}
	if ((this->rc = connect(sd, (struct sockaddr *) &serveraddr,
			sizeof(this->serveraddr))) < 0) {
		perror("Client-connect() error");
		close(this->sd);
		exit(-1);
	} else
		printf("Connection established...\n");
}

string GazeTrackerPublisher::ListenToServer() {
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

GZ_REGISTER_SYSTEM_PLUGIN(GazeTrackerPublisher)
