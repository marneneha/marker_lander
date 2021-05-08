#include <ros/ros.h>
#include <ros/package.h>
#include <mavros_msgs/CommandTOL.h>
#include <mavros_msgs/CommandBool.h>
#include <mavros_msgs/SetMode.h>
#include <std_srvs/Trigger.h>
#include <std_srvs/SetBool.h>

	std_srvs::SetBool					  motor_request;
	mavros_msgs::CommandBool 			  arm_request;
	mavros_msgs::SetMode 				  srv_setMode;
	std_srvs::Trigger   				  srv_takeoff;

    ros::ServiceClient                    motor_client;
    ros::ServiceClient                    arm_client;
    ros::ServiceClient                    set_mode_client;
    ros::ServiceClient                    takeoff_client;
    ros::ServiceClient                    land_client;

    ros::ServiceClient                    goto_client;

    std::string motor_service_name = std::string("/")+"uav1"+ "/control_manager/motors";
    std::string arm_service_name = std::string("/")+"uav1"+ "/mavros/cmd/arming";
	std::string mode_service_name = std::string("/")+"uav1"+ "/mavros/set_mode";
	std::string takeoff_service_name = std::string("/")+"uav1"+ "/uav_manager/takeoff";
	std::string land_service_name = std::string("/")+"uav1"+ "/mavros/cmd/land";



    void takeoff();
