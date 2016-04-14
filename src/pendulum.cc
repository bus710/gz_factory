#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/common/common.hh>
#include <boost/bind.hpp>
#include <ros/ros.h>
#include <std_msgs/Float32.h>

namespace gazebo
{
	class Pendulum : public ModelPlugin
	{
		//! variable declaration
		private: physics::ModelPtr model;
		private: physics::JointController * jc0;
		private: physics::JointPtr j0;
		private: physics::JointPtr j1;
		private: event::ConnectionPtr updateConnection;
		private: ros::NodeHandle n;
		private: ros::Subscriber sub;

		private: double angle;
		private: double angle2;
		private: int time;

		public: Pendulum(){}
		public: ~Pendulum(){ 
			this->n.shutdown();
		}

		//! instance initialization
		public: void Load(physics::ModelPtr _parent, sdf::ElementPtr)
		{
			//! set ROS connection
			int argc = 0;
			char** argv = NULL;

			sub = n.subscribe("joint_angle_pendulum", 1, &Pendulum::cb, this);

			//! set model 
			model = _parent;
			jc0 = new physics::JointController(model);
			j0 = model->GetJoint("upper_joint");
			j1 = model->GetJoint("lower_joint");

			//! set OnUpdate
			updateConnection = event::Events::ConnectWorldUpdateBegin(
											boost::bind(&Pendulum::OnUpdate, 
											this, _1));
			time = 0;
			angle = 0;
		}

		//! callback for ROS topic 
		public: void cb(const std_msgs::Float32Ptr &msg)
		{
			angle2 = msg->data/100;
		}

		//! callback for Gazebo update
		public: void OnUpdate(const common::UpdateInfo &)
		{
			time += 1;
			if(time >= 100){
				time = 0;
				angle += angle2;
			}
			
			// set position for the joints
			jc0->SetJointPosition(j0, angle);
			jc0->SetJointPosition(j1, angle);
		}
	};
	GZ_REGISTER_MODEL_PLUGIN(Pendulum)
}
