#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/common/common.hh>
#include <boost/bind.hpp>
#include <ros/ros.h>
#include <std_msgs/Float32.h>
#include <std_msgs/Float32MultiArray.h>

namespace gazebo
{
	class gz_mani : public ModelPlugin
	{
		//! Variable declaration for gz_mani
		private: physics::ModelPtr model;
		private: physics::JointController * jc0;
		private: physics::JointController * jc1;
		private: physics::JointController * jc2;
		private: physics::JointController * jc3;
		private: physics::JointController * jc4;
		private: physics::JointController * jc5;
		private: physics::JointController * jc6;
		private: physics::JointPtr j0;
		private: physics::JointPtr j1;
		private: physics::JointPtr j2;
		private: physics::JointPtr j3;
		private: physics::JointPtr j4;
		private: physics::JointPtr j5;
		private: physics::JointPtr j6;

		//! Node and connector declaration
		private: event::ConnectionPtr updateConnection;
		private: ros::NodeHandle n;
		private: ros::Subscriber sub;

		//! Variables for current status
		private: float angle[7];
		private: float angle_buf[7];
		private: int time;
		private: double wdt;

		//! Constructor and destroyer
		public: gz_mani(){}
		public: ~gz_mani(){
			ROS_INFO("Done");	
			this->n.shutdown();
		}

		//! Gazebo runs this method when the model is spawned
		public: void Load(physics::ModelPtr _parent, sdf::ElementPtr)
		{
			//! Telling to ROS for gz_mani
			//! Register the topic "joint_angle_gz_mani"
			ROS_INFO("Load");	
			
			int argc = 0;
			char** argv = NULL;

			ros::init(argc, argv, "gz_mani");
			sub = n.subscribe("joint_angle_gz_mani", 1, &gz_mani::cb, this);

			//! Get instances for each joint
			model = _parent;
			jc0 = new physics::JointController(model);
			jc1 = new physics::JointController(model);
			jc2 = new physics::JointController(model);
			jc3 = new physics::JointController(model);
			jc4 = new physics::JointController(model);
			jc5 = new physics::JointController(model);
			jc6 = new physics::JointController(model);

			j0 = model->GetJoint("joint0");
			j1 = model->GetJoint("joint1");
			j2 = model->GetJoint("joint2");
			j3 = model->GetJoint("joint3");
			j4 = model->GetJoint("joint4");
			j5 = model->GetJoint("joint5");
			j6 = model->GetJoint("joint6");

			//! Connect the update handler with an event
			updateConnection = event::Events::ConnectWorldUpdateBegin(
										boost::bind(&gz_mani::OnUpdate,
										this, _1));
			//! variable initialization
			time = 0;
			angle[0] = angle_buf[0] = 0;
			angle[1] = angle_buf[1] = 0;
			angle[2] = angle_buf[2] = 0;
			angle[3] = angle_buf[3] = 0;
			angle[4] = angle_buf[4] = 0;
			angle[5] = angle_buf[5] = 0;
			angle[6] = angle_buf[6] = 0;
			wdt = 0;
		}

		//! Gazebo runs this callback when it recieves topic from ROS
		public: void cb(const std_msgs::Float32MultiArrayPtr &msg)
		{
			//ROS_INFO("run cb()");
			//angle2 = msg->data/100;
			angle_buf[0] = msg->data[0];
			angle_buf[1] = msg->data[1];
			angle_buf[2] = msg->data[2];
			angle_buf[3] = msg->data[3];
			angle_buf[4] = msg->data[4];
			angle_buf[5] = msg->data[5];
			angle_buf[6] = msg->data[6];
		}

		//! Gazebo periodically runs this callback to update gz_mani's status 
		public: void OnUpdate(const common::UpdateInfo &)
		{
			time += 1;
			wdt += 1;
			if(time >= 100){
				time = 0;
				angle[0] += angle_buf[0];
				angle[1] += angle_buf[1];
				angle[2] += angle_buf[2];
				angle[3] += angle_buf[3];
				angle[4] += angle_buf[4];
				angle[5] += angle_buf[5];
				angle[6] += angle_buf[6];

				ROS_INFO("%3f", angle[1]);
			}

			jc0->SetJointPosition(j0, 0);
			jc1->SetJointPosition(j1, angle[1]);
			jc2->SetJointPosition(j2, angle[2]);
			jc3->SetJointPosition(j3, angle[3]);
			jc4->SetJointPosition(j4, angle[4]);
			jc5->SetJointPosition(j5, angle[5]);
			jc6->SetJointPosition(j6, angle[6]);
		}
	};
	//! be careful for the name. that should be mathed with world file
	GZ_REGISTER_MODEL_PLUGIN(gz_mani)
}








