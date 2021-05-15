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
	goto_client = (nh.serviceClient<mrs_msgs::Vec4>(goto_service_name));
	raw_image = it_.subscribe("/uav1/bluefox_optflow/image_raw",10,raw_image_cb);
	local_pose_sub = nh.subscribe<mrs_msgs::MpcTrackerDiagnostics>(local_pose_sub_name,1,local_pose_cb);
	pose_update_tim = nh.createTimer(ros::Duration(0.5), pose_update);
    takeoff();
	
	ROS_INFO("m here");
	ros::spin();
    return 0;

}

void pose_update(const ros::TimerEvent&){
	float x = curr_pose.setpoint.position.x;
	float y = curr_pose.setpoint.position.y;
	float z = curr_pose.setpoint.position.z;
	float yaw = 0;
	ROS_INFO("b4 while curr_pose is %f %f %f",curr_pose.setpoint.position.x, curr_pose.setpoint.position.y, curr_pose.setpoint.position.z);
	if(call_local_pose_cb)
	{
		ROS_INFO("inside if");
		for(int i= 0; i<100; i++)
		{
			ROS_INFO("inside for %d iteration current pose is %f %f %f",i ,curr_pose.setpoint.position.x, curr_pose.setpoint.position.y, curr_pose.setpoint.position.z);
			z = z + 0.2;
			ros::Duration(5).sleep();
			set_goal(x, y, z, yaw);
		}
	}
}
void takeoff()
{
	ROS_INFO("i am at takeoff start ");
	motor_request.request.data = 1;
	motor_client.call(motor_request);
	while(!motor_request.response.success)
	{
		ros::Duration(.1).sleep();
		motor_client.call(motor_request);
	}
	ROS_INFO("motor on"); 

	
	//set arm
	ros::Duration(5).sleep();
	arm_request.request.value = true;
	arm_client.call(arm_request);
	while(!arm_request.response.success)
	{
		ros::Duration(.1).sleep();
		arm_client.call(arm_request);
	}
	ROS_INFO( "arming done ");
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
		ROS_INFO("i got in if after arming service responce %d " ,srv_takeoff.response.success); 
		while (srv_takeoff.response.success)
		{
			ROS_INFO("i got in while after arming");
			ros::Duration(.1).sleep();
			takeoff_client.call(srv_takeoff);
		}
		ROS_INFO("i got in if after arming service responce %d " ,srv_takeoff.response.success); 
		if(!srv_takeoff.response.success){
			ROS_INFO("takeoff %d", srv_takeoff.response.success);
		}else{
			ROS_ERROR("Failed Takeoff");
		}
		}
	while(!takeoff_client.call(srv_takeoff)){}
	ros::Duration(10).sleep();
	  ROS_INFO("i am at takeoff end "); 
}

void set_goal(float x, float y, float z, float yaw )
{
	std::cout<<"inside set_goal function"<<"\n";
	srv_goto.request.goal.at(0) = x;
	srv_goto.request.goal.at(1) = y;
	srv_goto.request.goal.at(2) = z;
	srv_goto.request.goal.at(3) = yaw;
	goto_client.call(srv_goto);
	while(!goto_client.call(srv_goto)){}
}
void raw_image_cb(const sensor_msgs::ImageConstPtr& msg)
{
	cv_bridge::CvImagePtr cv_ptr;
	cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
   
	cv::imshow("bf_feed",cv_ptr->image);
	cv::waitKey(0); 
}
void local_pose_cb(const mrs_msgs::MpcTrackerDiagnostics::ConstPtr& msg)
{
	curr_pose = *msg;
	call_local_pose_cb = true;
	ROS_INFO("inside local pose callback pose is %f %f %f",curr_pose.setpoint.position.x, curr_pose.setpoint.position.y, curr_pose.setpoint.position.z);

}
