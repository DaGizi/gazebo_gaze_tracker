#include "cylinder_controller.hh"

using namespace gazebo;
using namespace std;

void CylinderController::Load(physics::WorldPtr _parent, sdf::ElementPtr /*_sdf*/)
{
	this->newPose.Set(0,0,0,0,0,0);
	this->gazeCylinder = _parent->GetModel("gaze_cylinder");
}

void CylinderController::Init()
{
	this->SubscribeToGazeTopic();
	this->updateConnection = event::Events::ConnectWorldUpdateBegin(boost::bind(&CylinderController::OnUpdate,this));
}

void CylinderController::SubscribeToGazeTopic()
{
	this->gazeNode = transport::NodePtr(new transport::Node());
	this->gazeNode->Init("empty");
	this->poseSub = this->gazeNode->Subscribe("/gazebo/empty_world/gaze_cylinder_pose", &CylinderController::gazeCallback, this);
	cout << "Subscribing to topic: ~/gaze_cylinder_pose" << endl;
}

void CylinderController::SubscribetoContactTopic()
{
	this->contactNode = transport::NodePtr(new transport::Node());
	this->contactNode->Init("empty");
//	this->contactSub = this->contactNode->Subscribe("topic_name", &CylinderController::contactCallback,this);
	cout << "Subscribing to topic :~/" << endl;
}


void CylinderController::OnUpdate()
{
	this->newPose.Set(this->cylinderPosition,this->cylinderOrientation);
	this->gazeCylinder->SetWorldPose(this->newPose);
}

void CylinderController::gazeCallback(const boost::shared_ptr<const gazebo::msgs::Pose> &_msg)
{
	//this->newPose.Set(_msg->position(), _msg->orientation());
	this->cylinderPosition.x = _msg->position().x();
	this->cylinderPosition.y = _msg->position().y();
	this->cylinderPosition.z = _msg->position().z();
	this->cylinderOrientation.w = _msg->orientation().w();
	this->cylinderOrientation.x = _msg->orientation().x();
	this->cylinderOrientation.y = _msg->orientation().y();
	this->cylinderOrientation.z = _msg->orientation().z();
//	this->newPose.Set(_msg->x(),_msg->y(),_msg->z(),0,0,0);
//	this->gazeCylinder->SetLinkWorldPose(this->newPose,"gaze_cylinder_link");
}

void CylinderController::contactCallback(gazebo::msgs::ContactSensorPtr &_msg)
{
	cout << _msg->DebugString();
}
GZ_REGISTER_WORLD_PLUGIN(CylinderController)
