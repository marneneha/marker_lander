#include <marker_lander/marker_lander.h>

int main(int argc, char **argv)
{
    ros::init(argc, argv, "marker_lander_node");
    ros::NodeHandle nh("~");

	image_transport::ImageTransport       it_(nh);
	
	motor_client = (nh.serviceClient<std_srvs::SetBool>(motor_service_name));
	arm_client = (nh.serviceClient<mavros_msgs::CommandBool>(arm_service_name));
	set_mode_client = (nh.serviceClient<mavros_msgs::SetMode>(mode_service_name));
	takeoff_client = (nh.serviceClient<std_srvs::Trigger>(takeoff_service_name));
	land_client = (nh.serviceClient<mavros_msgs::CommandTOL>(land_service_name));
	//raw_image = it_.subscribe("/uav1/bluefox_optflow/image_raw",10,raw_image_cb);
	local_pose_sub = nh.subscribe<nav_msgs::Odometry>(local_pose_sub_name,1,local_pose_cb);
	pose_pub = nh.advertise<geometry_msgs::PoseStamped>(pose_pub_name,10);

    takeoff();
	
	
	
	
	ros::spin();
    return 0;

}

void takeoff()
{
	std::cout << __FILE__ << ":" << __LINE__ << "i am at takeoff start "  <<std::endl; 
	motor_request.request.data = 1;
	motor_client.call(motor_request);
	while(!motor_request.response.success)
	{
		ros::Duration(.1).sleep();
		motor_client.call(motor_request);
	}
	std::cout << __FILE__ << ":" << __LINE__ << "motor on"  <<std::endl; 

	
	//set arm
	ros::Duration(5).sleep();
	arm_request.request.value = true;
	arm_client.call(arm_request);
	while(!arm_request.response.success)
	{
		ros::Duration(.1).sleep();
		arm_client.call(arm_request);
	}
	std::cout << __FILE__ << ":" << __LINE__ << "arming done "  <<std::endl; 
	if(arm_request.response.success)
	{
		ROS_INFO("Arming Successful");	
	}

	
	//set mode
	ros::Duration(5).sleep();
	srv_setMode.request.base_mode = 0;
	srv_setMode.request.custom_mode = "offboard";
	set_mode_client.call(srv_setMode);

	if(set_mode_client.call(srv_setMode)){
		ROS_INFO("setmode send ok");
	}else{
	      ROS_ERROR("Failed SetMode");
	}


	//takeoff
	if(arm_request.response.success){
		ros::Duration(5).sleep();
		takeoff_client.call(srv_takeoff);
		std::cout << __FILE__ << ":" << __LINE__ << "i got in if after arming"<<"service responce"<<srv_takeoff.response.success<<std::endl; 
		while (srv_takeoff.response.success)
		{
			std::cout << __FILE__ << ":" << __LINE__ << "i got in while after arming"<<std::endl; 
			ros::Duration(.1).sleep();
			takeoff_client.call(srv_takeoff);
		}
		std::cout << __FILE__ << ":" << __LINE__ << "i got in if after arming"<<"service responce"<<srv_takeoff.response.success<<std::endl; 
		if(!srv_takeoff.response.success){
			ROS_INFO("takeoff %d", srv_takeoff.response.success);
		}else{
			ROS_ERROR("Failed Takeoff");
		}
		}
	while(!takeoff_client.call(srv_takeoff)){}
	  std::cout << __FILE__ << ":" << __LINE__ << "i am at takeoff end "<<std::endl; 
	std::cout<<"m here";
	float x = curr_pose.pose.pose.position.x;
	float y = curr_pose.pose.pose.position.y;
	float z = curr_pose.pose.pose.position.z;
	float yaw = curr_pose.pose.pose.orientation.z;
	std::cout<<"b4 while";
	while (1)
	{
		if(curr_pose.pose.pose.position.z>1.4)
		{
			z = curr_pose.pose.pose.position.z;
			for(int i= 0; i<100; i++)
			{
				std::cout<<"inside for "<<i<<"iteration"<<"current pose is"<<curr_pose.pose.pose.position<<"\n";
				z = z + 0.2;
				set_goal(x, y, z, w, roll, pitch, yaw);
			}
		}
	}
}

void set_goal(float x, float y, float z, float w, float roll , float pitch, float yaw )
{
	std::cout<<"inside set_goal function"<<"\n";
	geometry_msgs::PoseStamped next_point;
	next_point.header.seq = 0;
	next_point.header.stamp.sec = 0;
	next_point.header.stamp.nsec = 0;
	next_point.header.frame_id = "neha";
	next_point.pose.position.x = x;
	next_point.pose.position.y = y;
	next_point.pose.position.z = z;
	next_point.pose.orientation.w = w;
	next_point.pose.orientation.x = roll;
	next_point.pose.orientation.y = pitch;
	next_point.pose.orientation.z = yaw;

	pose_pub.publish(next_point);
}
void raw_image_cb(const sensor_msgs::ImageConstPtr& msg)
{
	cv_bridge::CvImagePtr cv_ptr;
	cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
   
	cv::imshow("bf_feed",cv_ptr->image);
	cv::waitKey(0); 
}

void local_pose_cb(const nav_msgs::Odometry::ConstPtr& msg)
{
	curr_pose = *msg;
  	//enu_2_local(curr_pose);
	  //std::cout<<"inside local pose callback pose is"<<curr_pose;
}
