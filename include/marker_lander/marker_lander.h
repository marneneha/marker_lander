#include <ros/ros.h>
#include <ros/package.h>
#include <mavros_msgs/CommandTOL.h>
#include <mavros_msgs/CommandBool.h>
#include <mavros_msgs/SetMode.h>
#include <std_srvs/Trigger.h>
#include <std_srvs/SetBool.h>

	mavros_msgs::CommandBool 			  arm_request;
	mavros_msgs::SetMode 				  srv_setMode;
    std_srvs::SetBool					  motor_request;
	std_srvs::Trigger					  srv_takeoff;
    ros::ServiceClient                    arm_client;
    ros::ServiceClient                    motor_client;
    ros::ServiceClient                    land_client;
    ros::ServiceClient                    set_mode_client;
    ros::ServiceClient                    goto_client;
    ros::ServiceClient                    takeoff_client;


    //see uav here need to replace
    std::string motor_service_name = std::string("/")+"uav1"+ "/control_manager/motors";
    std::string arv_service_name = std::string("/")+"uav1"+ "/mavros/cmd/arming";
	std::string land_service_name = std::string("/")+"uav1"+ "/mavros/cmd/land";
	std::string mode_service_name = std::string("/")+"uav1"+ "/mavros/set_mode";
	std::string takeoff_service_name = std::string("/")+"uav1"+ "/mavros/takeoff";



    void takeoff();
