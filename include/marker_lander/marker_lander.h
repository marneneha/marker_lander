#include <ros/ros.h>
#include <ros/package.h>
#include <mavros_msgs/CommandTOL.h>
#include <mavros_msgs/CommandBool.h>
#include <mavros_msgs/SetMode.h>
#include <std_srvs/Trigger.h>
#include <std_srvs/SetBool.h>
#include <sensor_msgs/Image.h>
//cv2 lib
#include <image_transport/image_transport.h>
#include <opencv2/opencv.hpp>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>


	std_srvs::SetBool					  motor_request;
	mavros_msgs::CommandBool 			  arm_request;
	mavros_msgs::SetMode 				  srv_setMode;
	std_srvs::Trigger   				  srv_takeoff;

    ros::ServiceClient                    motor_client;
    ros::ServiceClient                    arm_client;
    ros::ServiceClient                    set_mode_client;
    ros::ServiceClient                    takeoff_client;
    ros::ServiceClient                    land_client;
    
    image_transport::Subscriber           raw_image;

    ros::ServiceClient                    goto_client;

    std::string motor_service_name = std::string("/")+"uav1"+ "/control_manager/motors";
    std::string arm_service_name = std::string("/")+"uav1"+ "/mavros/cmd/arming";
	std::string mode_service_name = std::string("/")+"uav1"+ "/mavros/set_mode";
	std::string takeoff_service_name = std::string("/")+"uav1"+ "/uav_manager/takeoff";
	std::string land_service_name = std::string("/")+"uav1"+ "/mavros/cmd/land";

    void raw_image_cb(const sensor_msgs::ImageConstPtr& msg);


    void takeoff();
