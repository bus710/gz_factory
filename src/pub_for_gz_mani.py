#!/usr/bin/python

# Default libraries
import os, sys
import random

# ROS libraries
# Please add 'from std_msgs.msg import VARIABLE_TYPE' when you add new types of variables
import rospy
from std_msgs.msg import Float32MultiArray
from std_msgs.msg import MultiArrayLayout
from std_msgs.msg import MultiArrayDimension
from geometry_msgs.msg  import Twist
from sensor_msgs.msg import Joy

# this function makes a new publisher node 
# and send data to certain node for gz_mani 
def pub():
	""" 
	input: none
	output: none
	"""

	# Register a new node and initialization
	pub = rospy.Publisher("joint_angle_gz_mani", 
		Float32MultiArray, queue_size=1)
	rospy.init_node('pub')

	# Initialize for multi dimension array
	# Each cell of list means the angle of gz_mani's joints 
	dim = [MultiArrayDimension(label = 'Dimension1', 
			size = 1, 
			stride = 7)]
	data_offset = 1
	layout = MultiArrayLayout(dim, data_offset)
	time = 0
	data_flag = 2
	data = [0, 0.1, 0, 0, 0, 0, 0]
	data_1 = [0, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1]
	data_2 = [0, 0, 1, 1, 1, -1, 1]

	# The super loop
	while not rospy.is_shutdown():
		str = "ros time: %s"%rospy.get_time()
		rospy.loginfo(str)
		rospy.loginfo(data)

		topic = Float32MultiArray(layout, data)
		pub.publish(topic)

		rospy.sleep(0.1)
	print 'done'


if __name__ == '__main__':
	try:
		pub()
	except rospy.ROSInterruptException: 
		pass
	print




