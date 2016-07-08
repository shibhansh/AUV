// Copyright 2016 AUV-IITK
#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <sstream>  // for converting the command line parameter to integer

/*! \file
* \brief super short description
*
* Long decription
*/

/*! member description */
int main(int argc, char **argv)
{
  // Check if video source has been passed as a parameter
  if (argv[1] == NULL)
    return 1;

  ros::init(argc, argv, "image_publisher");
  ros::NodeHandle nh;
  image_transport::ImageTransport it(nh);
  image_transport::Publisher pub = it.advertise("camera/image", 1);

  // Convert the passed as command line parameter index for the video device to
  // an integer
  std::istringstream video_sourceCmd(argv[1]);
  int video_source;
  // Check if it is indeed a number
  if (!(video_sourceCmd >> video_source))
    return 1;

  cv::VideoCapture cap(video_source);
  // Check if video device can be opened with the given index
  if (!cap.isOpened())
    return 1;
  cv::Mat frame;
  sensor_msgs::ImagePtr msg;
  int loopRate = 10;
  ros::Rate loop_rate(loopRate);
  while (nh.ok())
  {
    cap >> frame;
    // Check if grabbed frame is actually full with some content
    if (!frame.empty())
    {
      msg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", frame).toImageMsg();
      pub.publish(msg);
      // loop_rate.sleep();
    }
    ros::spinOnce();
  }
}
