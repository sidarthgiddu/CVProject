/*
* File:				Multi-Robot Localization on Heterogenous Platform (SW version)
* Author:			Moonyoung Lee (ml634@cornell.edu)
* Adapted from:
*					main.cpp by
* Author:			Mohit Sravya
* Target Hardward:  Visual Studio Laptop to RPi/ Arduino (Galileo/Edison)
*
* (1) Server Client Setup
* (2) Visual Tracking Algorithm
* (3) Send Command to Robot
*
*/
//========================================================
// Initialization
//========================================================

// Include directories 
#include "stdafx.h"
#include "ServerGame.h"
#include "ClientGame.h"

#include <iostream>
#include <math.h>
#include <time.h>
#include <numeric>

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;

// Functions declarations
void iRobotCommandFormatVelocity(int, char *);
void sendCommands(char *);
void serverLoop(void *);
void morphFilter(Mat);
void updateGoalPosition(Point2f, Mat);
int getAngleDifference(Point2f, Point2f, Point2f);

char * serialDataOverNetwork = new char[2];

ServerGame * server;
ClientGame * client;

#define PI 3.14159265
#define numberOfRobot 1					//adjust to # of robot on field

int robotCount = 0;						//used to iterate newly found robot
int detectedSize = 0;
int detectedRobots = -1;					//used to count how many robots on detected field 
int extendLineLength = 100;				//line used to extend from robot
int previousAngleDifference = 0;
int currentAngleDifference = 0;
double previousAngle = 0;

bool turn = false; //turn is true when robot is turning, else it is false
bool dir = true; //dir is true when robot is moving right, else it is moving left

vector<Moments> mu(numberOfRobot);			//holds Moments
Point2f newPosition;						//holds Center of Mass (X,Y) for unknown robot
Point2f goalPosition;	//holds desired end pt


int x, y, MA, ma, angle;
int intializedRobot = 0;				//initialize Robot with ID and starting location

//angle between header and goal
int angleDifference;

#define averageFilterSize 5
double angleArray[averageFilterSize] = {};					//holds 5pt averaging filter values
double angleFiltered;						//averaged angle
int angleArrayCounter = 0;					//to index through Array
#define angleTolerance 10				//degree to go straight instead of turning
#define distanceComplete 75			//distance in pixel size to determine if goal reached
#define distanceFromTop 125			//distance in pixel size to determine is robot should stop
#define ROBOT_SIZE 3000					//calibrated robot pixel_size > adjust with setup

Mat ImgOriginal;	//instance of Img Mat
Mat ImgGray;		//instance of HSV Mat
Mat ImgThreshold;	//instance of Threshold Mat
Mat ImgHSV;
Mat robotPath(480, 640, CV_8UC3, Scalar(0));
Mat img = imread("C:\\Users\\Sidarth Giddu\\Desktop\\simplewallpaper.png", CV_LOAD_IMAGE_COLOR);
Mat displayWindow(480, 640, CV_8UC3, Scalar(0));

//HSV values to threshold black objects
int lowH = 0;
int highH = 179;
int lowS = 0;
int highS = 255;
int lowV = 0;
int highV = 91;

bool startState = false; //flag to start tracking

bool doneState = false; //flag to complete tracking
double remainingDist = 0;

//========================================================
// class of Robot to hold unique info
//========================================================
class Robot {
public:
	int robotID = 0;
	Point2f previousPos;
	Point2f position;
	double angle;							    //robot orientation							
	Point2f extendLinePos;						//extends in direction of orientation
};

Robot robotList[numberOfRobot]; //enter number of robot object to store in array


//========================================================
// Main Function
//========================================================
int main() {

	// To set new IP server, go to ClientNetwork.cpp line 37 
	cout << "Initializing Client: " << endl;
	client = new ClientGame();
	VideoCapture capture(1);
	cout << "Captured a frame" << endl;
	// no instance of camera
	if (!capture.isOpened()) {
		cout << "Video Capture Error" << endl;
		system("pause");
		return -1;
	}

	//enter main loop default stage
	//iRobotCommandFormatVelocity(4, serialDataOverNetwork); sendCommands(serialDataOverNetwork);


	/* ================== MAIN WHILE LOOP =====================
	/ - Image Process [Input > GrayScale > Denoise > Canny Edge > Find Contour]
	/ - Tracking Algorithm [Match Robot ID > Find CoM > Find Orientation > Determine Command]
	/ - Send Command
	=========================================================== */
	while (true) {

		// read video input
		bool vidSuccess = capture.read(ImgOriginal);
		if (!vidSuccess) { cout << "No video input" << endl; break; }

		/* ================== Object Detection Setup ===================== */

		//Threshold with black color object
		cvtColor(ImgOriginal, ImgHSV, COLOR_BGR2HSV);
		inRange(ImgHSV, Scalar(lowH, lowS, lowV), Scalar(highH, highS, highV), ImgThreshold);
		
		//Denoise w/ morphology filter
		morphFilter(ImgThreshold);

		/* ================== Robot Tracking Algorithm  ===================== */

		// mask image for copying contour since modifies input
		Mat Img_mask(ImgThreshold.size(), CV_8UC3, Scalar(0));
		ImgThreshold.copyTo(Img_mask);
		//find contours of robots (arbitrary sequence of detecting shapes)
		RNG rng(12345);
		vector<vector<Point>> contours;
		vector<Vec4i> hierarchy;
		vector<Point> polyApprox;
		findContours(Img_mask, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

		//loop through detected contour  newly execute PER FRAME
		for (int i = 0; i < contours.size(); i++) {

			//get area of detected contour
			detectedSize = contourArea(contours[i]);

			//perform on only robot-sized objects
			if (!doneState && (detectedSize > ROBOT_SIZE)) {

				detectedRobots += 1;
				//======================================================== 

				//get new position of unknown robot
				mu[0] = moments(contours[i]);
				newPosition = Point2f((mu[0].m10 / mu[0].m00), (mu[0].m01 / mu[0].m00));

				//========================================================

				//case for 1 robot
				robotList[detectedRobots].position = newPosition;

				cout << "Robot Position: " << newPosition.x << " , " << newPosition.y << endl;
				cout << "Goal Position: " << goalPosition.x << " , " << goalPosition.y << endl;
				cout << "Dir: " << dir << endl;


				//calculate goalPosition
				updateGoalPosition(newPosition, Img_mask);

				//draw circle on robotPath
				circle(robotPath, robotList[detectedRobots].position, 5, Scalar(0, 0, 255), -1);
				int imgx = (int)(newPosition.x*(img.cols - 1) / 639.0 + 0.5);
				int imgy = (int)(newPosition.y*(img.rows - 1) / 479.0 + 0.5);
				cout << "Img x: " << imgx << endl;
				cout << "Img y: " << imgy << endl;
				Scalar color = img.at<Vec3b>(Point(imgx, imgy));
				circle(displayWindow, robotList[detectedRobots].position, 30,color, -1);

				//draw circle on CoM
				circle(ImgOriginal, robotList[detectedRobots].position, 5, Scalar(0, 255, 0), 2);
				//draw circle on end PT
				circle(ImgOriginal, goalPosition, 5, Scalar(0, 0, 255), 2);
				//draw line from 2 pts
				line(ImgOriginal, robotList[detectedRobots].position, goalPosition, Scalar(0, 0, 255), 1);

				//draw box around object
				RotatedRect box = fitEllipse(contours[i]);
				Point2f vtx[4];
				box.points(vtx);
				for (int j = 0; j < 4; j++) { line(ImgOriginal, vtx[j], vtx[(j + 1) % 4], Scalar(0, 255, 0), 1); }

				//get orientation of robot
				robotList[detectedRobots].angle = box.angle;
				cout << "angle of rob1: " << robotList[0].angle << endl;

				if (!dir && turn && abs(previousAngle - robotList[detectedRobots].angle) > 100){
					turn = false;
				}

				else if (dir && turn && abs(robotList[detectedRobots].angle - previousAngle) > 100){
					turn = false;
				}
				cout << "Turn: " << turn << endl;
				previousAngle = robotList[detectedRobots].angle;

				//draw extendLine for header
				if (turn == dir){
					robotList[detectedRobots].extendLinePos = Point2f(robotList[detectedRobots].position.x - extendLineLength*sin((previousAngle)*PI / 180), robotList[detectedRobots].position.y + extendLineLength*cos((previousAngle)*PI / 180));
				}
				else{
					robotList[detectedRobots].extendLinePos = Point2f(robotList[detectedRobots].position.x + extendLineLength*sin((previousAngle)*PI / 180), robotList[detectedRobots].position.y - extendLineLength*cos((previousAngle)*PI / 180));
				}
				
				//draw line from 2 pts
				line(ImgOriginal, robotList[detectedRobots].position, robotList[detectedRobots].extendLinePos, Scalar(0, 255, 0), 1);

				// get angle between 2 lines (robot header & goal line)
				angleDifference = getAngleDifference(robotList[detectedRobots].extendLinePos, robotList[detectedRobots].position, goalPosition);
				putText(ImgOriginal, to_string(angleDifference), robotList[detectedRobots].position, FONT_HERSHEY_COMPLEX, 0.5, Scalar(255, 255, 255), 2);
				cout << "Angle Difference: " << angleDifference << endl;

				// determine when goal is reached
				remainingDist = (cv::norm(goalPosition - robotList[detectedRobots].position));
				cout << "distance: " << remainingDist << endl;

				//determine DONE when robot near Goal
				if (remainingDist < distanceComplete) {
					if (dir && goalPosition.y > distanceFromTop){
						dir = false;
						turn = true;
						continue;
					}
					else if (!dir && goalPosition.y > distanceFromTop){
						dir = true;
						turn = true;
						continue;
					}
					else{
						cout << "Stop" << endl;
						iRobotCommandFormatVelocity(3, serialDataOverNetwork);
						sendCommands(serialDataOverNetwork);
						cout << "DONE" << endl;
						doneState = true;
						break;
					}
				}

				//hard-coded for the first robot only
				if (detectedRobots == 0) {
					// ***determine commands***
					if ((angleDifference > angleTolerance) && (remainingDist > distanceComplete)) {
						if (turn && dir){
							cout << "Turn Right" << endl;
							iRobotCommandFormatVelocity(4, serialDataOverNetwork);
							sendCommands(serialDataOverNetwork);
						}
						else{
							cout << "Right" << endl;
							iRobotCommandFormatVelocity(0, serialDataOverNetwork);
							sendCommands(serialDataOverNetwork);
						}
					}
					else if ((angleDifference < -angleTolerance) && (remainingDist > distanceComplete)) {
						if (turn && !dir){
							cout << "Turn Left" << endl;
							iRobotCommandFormatVelocity(5, serialDataOverNetwork);
							sendCommands(serialDataOverNetwork);
						}
						else{
							cout << "Left" << endl;
							iRobotCommandFormatVelocity(2, serialDataOverNetwork);
							sendCommands(serialDataOverNetwork);
						}
					}
					else if ((angleDifference > -angleTolerance) && (angleDifference < angleTolerance) && (remainingDist > distanceComplete)) {
						cout << "Straight" << endl;
						iRobotCommandFormatVelocity(1, serialDataOverNetwork);
						sendCommands(serialDataOverNetwork);
					}
				}

			} //end of detect size
		} //end of contour loop
		detectedRobots = -1;

		//display window
		imshow("raw", ImgOriginal);
		imshow("thres", ImgThreshold);
		imshow("robotPath", robotPath);
		imshow("Display Window", displayWindow);


		if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
		{
			cout << "esc key is pressed by user" << endl;
			break;
		}

	}
	return 0;
}

//========================================================
// update Goal X,Y position
//========================================================

void updateGoalPosition(Point2f robotPos, Mat img) {
	if ((dir && turn) || (dir && !startState)){
		goalPosition = Point2f(img.cols, robotPos.y);
		startState = true;
	}
	else if ((!dir && turn) || (!dir && !startState)){
		goalPosition = Point2f(0, robotPos.y);
		startState = true;
	}
	else{

	}
}

//========================================================
// get angle from three points from robot to goal
//========================================================
int getAngleDifference(Point2f robotHeadingPos, Point2f robotPos, Point2f goalPos) {

	float dy31, dy21, dx31, dx21;
	float theta1, theta2;
	previousAngleDifference = currentAngleDifference;
	dy21 = robotHeadingPos.y - robotPos.y;
	dy31 = goalPos.y - robotPos.y;
	dx21 = robotHeadingPos.x - robotPos.x;
	dx31 = goalPos.x - robotPos.x;
	theta1 = atan(dy21 / dx21) * 180 / PI;
	theta2 = atan(dy31 / dx31) * 180 / PI;
	currentAngleDifference = int((theta2 - theta1) + 0.5);  //typecast to int w/ rounding
	if (turn){
		if (dir){
			currentAngleDifference = currentAngleDifference + 180;
		}
		else{
			currentAngleDifference = currentAngleDifference - 180;
		}
	}
	return currentAngleDifference; 
}


//========================================================
// Creates a byte of command
//========================================================
void iRobotCommandFormatVelocity(int rotationCommand, char * stringDataOverNetwork) {

	*(stringDataOverNetwork) = (char)(rotationCommand);
}

//========================================================
// Sends command into the network
//========================================================
void sendCommands(char * serialData) {
	client->sendPackets(serialData);
}

// infinite loop for server - Not needed for this project
void serverLoop(void * arg)
{
	while (true)
	{
		server->update();
	}
}

//========================================================
//Denoise with morphology filter
//========================================================
void morphFilter(Mat image) {

	erode(image, image, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
	dilate(image, image, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

	dilate(image, image, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
	erode(image, image, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

}