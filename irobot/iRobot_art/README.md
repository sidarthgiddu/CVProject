# iRobotArt

This project makes Raspberry Pi Robots paint a picture using a central, vision-based localization system. In this case, the central vision-based
system is a laptop.

The robots themselves need not be aware of their locations and orientations, only the laptop. Visual C code, running on the laptop, takes the videostream input from 
a camera, identifies the robots, then calculates their positions and orientations. Given this information, it sends a right, left, straight, or stop 
command to each robot. Each robot has a python script running on it which interprets this command and tells the robot to move accordingly. 

Currently, this system only works for one robot.
Feel free to download this code and expand it however you like! 

## Setup

First, load the Python script, localizationRobot.py, onto a Raspberry Pi Robot. 
Next, download/install Visual Studio on your laptop if you do not have it already. Open iRobot_art.sln in Visual Studio.

Make sure the Raspberry Pi Robot and your laptop are connected to the same wifi. Next, obtain the IP address of the router. Set a static IP for both
the robot and your laptop. The first three numbers should be the same, the fourth can be any number between 0 and 255. Make sure the RPi robot and your
laptop do not have the same static IP. 

In localizationRobot.py, set HOST to the IP address of the RPi robot. 
In the Visual C code, in ClientNetwork.cpp, change the first parameter in getaddrinfo, to the IP address of the RPi robot. 

The port number can be whatever you like, just make sure it is the same in DEFAULT_PORT in ClientNetwork.h and PORT in localizationRobot.py . 

In main.cpp, there is a line VideoCapture capture (1). If you are using the webcam on your laptop, change the parameter to 0. If you are using an 
external camera, keep the parameter as 1. 

In main.cpp, there is a line, Mat img = imread... Change the first parameter to the file path of the image you want the robot(s) to "paint"

## Running the code

First, run localizationRobot.py by using the terminal on Raspberry Pi

Next, build/run the Visual Studio solution. Ease the robot(s) into the window of the camera, and watch the magic unfold! 




