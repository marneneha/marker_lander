#include <ros/ros.h>
#include <ros/package.h>
#include <mavros_msgs/CommandTOL.h>
#include <mavros_msgs/CommandBool.h>
#include <mavros_msgs/SetMode.h>
#include <std_srvs/Trigger.h>
#include <std_srvs/SetBool.h>
#include <sensor_msgs/Image.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include <nav_msgs/Odometry.h>
#include <mrs_msgs/Vec4.h>
#include <mrs_msgs/MpcTrackerDiagnostics.h>
#include <apriltag_ros/AprilTagDetectionArray.h>
#include <apriltag_ros/AprilTagDetection.h>
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
    mrs_msgs::Vec4                        srv_goto;
    mrs_msgs::MpcTrackerDiagnostics       curr_pose;


    ros::ServiceClient                    motor_client;
    ros::ServiceClient                    arm_client;
    ros::ServiceClient                    set_mode_client;
    ros::ServiceClient                    takeoff_client;
    ros::ServiceClient                    goto_client;
    ros::ServiceClient                    land_client;

    ros::Publisher                        pose_pub;
    ros::Subscriber                       local_pose_sub;
    ros::Subscriber                       tag_detect_sub;

    ros::Timer                            pose_update_tim;
    image_transport::Subscriber           raw_image;

    bool                                  call_local_pose_cb= false;
    bool                                  toggle = false;
    bool                                  tag_detect = false;
    float                                 goal_x;
    float                                 goal_y;
    float                                 goal_z;


    std::string motor_service_name = std::string("/")+"uav1"+ "/control_manager/motors";
    std::string arm_service_name = std::string("/")+"uav1"+ "/mavros/cmd/arming";
	std::string mode_service_name = std::string("/")+"uav1"+ "/mavros/set_mode";
	std::string takeoff_service_name = std::string("/")+"uav1"+ "/uav_manager/takeoff";
    std::string goto_service_name = std::string("/")+"uav1"+ "/control_manager/goto";
	std::string land_service_name = std::string("/")+"uav1"+ "/mavros/cmd/land";
    std::string local_pose_sub_name = std::string("/")+"uav1"+"/control_manager/mpc_tracker/diagnostics";
    std::string tag_detect_sub_name = "/tag_detections";

    void raw_image_cb(const sensor_msgs::ImageConstPtr& msg);
    void local_pose_cb(const mrs_msgs::MpcTrackerDiagnostics::ConstPtr& msg);
    void tag_detect_cb(const apriltag_ros::AprilTagDetectionArrayConstPtr& msg);
    void set_goal(float x, float y, float z, float yaw );
    void takeoff();
    void pose_update(const ros::TimerEvent&);
