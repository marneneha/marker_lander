#include <marker_lander/marker_lander.h>

int main(int argc, char **argv)
{
    ros::init(argc, argv, "marker_lander_node");
    ros::NodeHandle nh("~");

	motor_client = nh.serviceClient<std_srvs::SetBool>(motor_service_name);
	arm_client = (nh.serviceClient<mavros_msgs::CommandBool>(arv_service_name));
	land_client = (nh.serviceClient<mavros_msgs::CommandTOL>(land_service_name));
	set_mode_client = (nh.serviceClient<mavros_msgs::SetMode>(mode_service_name));
	takeoff_client = (nh.serviceClient<std_srvs::Trigger>(takeoff_service_name));

    takeoff();

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
		for(int i=0;i<100;i++)
		{
		//std::cout << __FILE__ << ":" << __LINE__ << "i got in while after arming"  <<std::endl; 
			ros::Duration(.1).sleep();
			takeoff_client.call(srv_takeoff);
		}

		if(takeoff_client.call(srv_takeoff)){
			ROS_INFO("takeoff %d", srv_takeoff.response.success);
		}else{
			ROS_ERROR("Failed Takeoff");
		}
		}
	while(!takeoff_client.call(srv_takeoff)){}
	  std::cout << __FILE__ << ":" << __LINE__ << "i am at takeoff end "  <<std::endl; 
}