#include <ros/ros.h>
#include <serial/serial.h>
#include <std_msgs/String.h>
#include <std_msgs/Empty.h>


serial::Serial ser;

void write_callback(const std_msgs::String::ConstPtr& msg)
{
    ROS_INFO("I heard: [%s]", msg->data.c_str());
    ROS_INFO_STREAM("Writing to serial port" << msg->data);
    ser.write(msg->data);
}
//send msg to serial port

int main (int argc, char** argv)
{
    ros::init(argc, argv, "serial_test");
    ros::NodeHandle nh;
    //ROS node initialization

    ros::Subscriber write_sub = nh.subscribe("joint_angle", 1000, write_callback);
    //serial port subscribe something from PC
    ros::Publisher read_pub = nh.advertise<std_msgs::String>("read", 1000);
    //Pub something from serialport and i can subscribe this toptic"read"from another listen_node

    try//set serial port
    {
      ser.setPort("/dev/ttyUSB0");
      ser.setBaudrate(9600);
      serial::Timeout to = serial::Timeout::simpleTimeout(1000);
      ser.setTimeout(to);
      ser.open();
    }
    catch (serial::IOException& e)
    {
      ROS_ERROR_STREAM("Unable to open port ");
      return -1;
    }

    if(ser.isOpen())//test if serial port is open
        {
	  ROS_INFO_STREAM("Serial Port initialized"); 
	}
    else
        {return -1;}

        ros::Rate loop_rate(5);
        while(ros::ok())
        {
          ros::spinOnce();
          if(ser.available())
                  {
                      ROS_INFO_STREAM("Reading from serial port");
                      std_msgs::String result;
                      result.data = ser.read(ser.available());
                      ROS_INFO_STREAM("Read: " << result.data);
                      read_pub.publish(result);
                  }

          loop_rate.sleep();
        }
}
