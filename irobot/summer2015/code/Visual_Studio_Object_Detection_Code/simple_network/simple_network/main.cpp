// main.cpp : Defines the entry point for the console application.
//

// may need #include "stdafx.h" in visual studio
#include "stdafx.h"
#include "ServerGame.h"
#include "ClientGame.h"
// used for multi-threading
#include <process.h>
#include <iostream>
#include <ctime>
#include <iostream>
#include <math.h>
#include <time.h>

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;

Mat imgThresholdedMedian;
RNG rng(12345);

int MIN_SPEED_FRONT = 0;
int MIN_SPEED_BACK = 0;
int MIN_TURN_LEFT = 550;  // More radius; means less turn
int MIN_TURN_RIGHT = -550;  // more radius: means less turning

int SPEED_INCREMENT_STEPS = 4;
int TURN_INCREMENT_STEPS = 20;

int MAX_SPEED_FRONT = 500;
int MAX_SPEED_BACK = -500;
int MAX_TURN_LEFT = 201;  // with 0 radius, turning will be maximum
int MAX_TURN_RIGHT = -201;  // with 0 radius, turning will be maximum

int MAX_RECT_ANGLE = -89;
int MIN_RECT_ANGLE = -1;

// to convert the range of radius
int betaMin = 0;
int betaMax = 0;
int lMin = 0;
int lMax = 0;


// To convert the range of velocity
int alphaMin = 0;
int alphaMax = 0;
int mMin = 0;
int mMax = 0;

double prevTime = 0;
int prevArea = 0;

// Calibration time of 1 second to allocate MAX range of object. Blue color
const unsigned int CALIBRATION_TIME_MAX_RANGE = 5;
// Calibration time of 2 seconds to allocate MIN range of the object. Red color
const unsigned int CALIBRATION_TIME_MIN_RANGE = 10;

// Wait for 3 seconds before start with the video: Pink color
const unsigned int CALIBRATION_COMPLETE_WAIT_TIME = 13;
bool stopCommandFlag = 0;
//----------------------Revised------------------------//
bool readyToStart = 0;


// Functions declarations
void thresh_callback(int, void*);
void iRobotCommandFormatVelocity(int, int, char *);
void corner_detection();
int rangeConversion(int, int, int, int, int);
void sendCommands(char *);

char * serialDataOverNetwork = new char[8];


void serverLoop(void *);
void clientLoop(void);

ServerGame * server;
ClientGame * client;

// -----------------------  Functions to calculate accurate timing  ------------------------------------ //
// Resolution in microseconds
LARGE_INTEGER
getFILETIMEoffset()
{
	SYSTEMTIME s;
	FILETIME f;
	LARGE_INTEGER t;

	s.wYear = 1970;
	s.wMonth = 1;
	s.wDay = 1;
	s.wHour = 0;
	s.wMinute = 0;
	s.wSecond = 0;
	s.wMilliseconds = 0;
	SystemTimeToFileTime(&s, &f);
	t.QuadPart = f.dwHighDateTime;
	t.QuadPart <<= 32;
	t.QuadPart |= f.dwLowDateTime;
	return (t);
}

int clock_gettime(int X, struct timeval *tv)
{
	LARGE_INTEGER           t;
	FILETIME            f;
	double                  microseconds;
	static LARGE_INTEGER    offset;
	static double           frequencyToMicroseconds;
	static int              initialized = 0;
	static BOOL             usePerformanceCounter = 0;

	if (!initialized) {
		LARGE_INTEGER performanceFrequency;
		initialized = 1;
		usePerformanceCounter = QueryPerformanceFrequency(&performanceFrequency);
		if (usePerformanceCounter) {
			QueryPerformanceCounter(&offset); 
			frequencyToMicroseconds = (double)performanceFrequency.QuadPart / 1000000.;
		}
		else {
			offset = getFILETIMEoffset();
			frequencyToMicroseconds = 10.;
		}
	}
	if (usePerformanceCounter) QueryPerformanceCounter(&t);
	else {
		GetSystemTimeAsFileTime(&f);
		t.QuadPart = f.dwHighDateTime;
		t.QuadPart <<= 32;
		t.QuadPart |= f.dwLowDateTime;
	}

	t.QuadPart -= offset.QuadPart;
	microseconds = (double)t.QuadPart / frequencyToMicroseconds;
	t.QuadPart = microseconds;
	tv->tv_sec = t.QuadPart / 1000000;
	tv->tv_usec = t.QuadPart % 1000000;
	return (0);
}


// -------------------------------- Main Function ------------------------------------- //
// Image capture
// YCbCr threshold for red color
// median filtering with 5x5 window
// Object detection. Find the rectangle enclosed by the object
// Find the angle of the detected rectangle
// Find the relative distance of the object from camera (with respect to maximum and minimum distance range)
// Form the command of radius and velocity from the angle and distance
// send the command via ethernet
int main() {

	// initialize the client 
	// Caviat: If there is no server hosted on Arduino 192.168.137.10, then ClientGame will wait and exit.
	// To know the ip configurations, go to ClientNetwork.cpp and go to line 37 with following text
	// --> //resolve server address and port 
	// --> iResult = getaddrinfo("192.168.137.10", DEFAULT_PORT, &hints, &result); // Galileo server
	// Change the ip and default port to the ip and port configured in Arduino/ Galileo which runs this server.
	client = new ClientGame();

	// initialize the server - Not needed for this project
	// Because server is hosted by arduino
	//	server = new ServerGame();


	// create thread with arbitrary argument for the run function
	// Not needed for this project
	//	_beginthread(serverLoop, 0, (void*)12);

	timeval tv_start, tv_end;

	Sleep(1000);

	VideoCapture cap(0); //capture the video from web cam
	Sleep(2000);

	if (!cap.isOpened())  // if not success, exit program
	{
		cout << "Cannot open the web cam" << endl;
		getchar();
		return -1;
	}

	namedWindow("Control", CV_WINDOW_AUTOSIZE); //create a window called "Control"

	int iLowY = 0;
	int iHighY = 255;

	int iLowCb = 169;
	int iHighCb = 255;

	int iLowCr = 0;
	int iHighCr = 255;

	// initialization of the velocity range. It will always be below value throughout our program.
	alphaMax = MAX_SPEED_FRONT;
	alphaMin = MAX_SPEED_BACK;

	// YCbCr thresholding for red colored object
	cvCreateTrackbar("LowY", "Control", &iLowY, 160);
	cvCreateTrackbar("HighY", "Control", &iHighY, 160);

	cvCreateTrackbar("LowCb", "Control", &iLowCb, 255);
	cvCreateTrackbar("HighCb", "Control", &iHighCb, 255);

	cvCreateTrackbar("LowCr", "Control", &iLowCr, 255);
	cvCreateTrackbar("HighCr", "Control", &iHighCr, 255);

	// Infinite loop

	while (true) {
		Mat imgOriginal;
		clock_gettime(1, &tv_start);

		// Image capture
		bool bSuccess = cap.read(imgOriginal); // read a new frame from video

		if (!bSuccess) //if not success, break loop
		{
			cout << "Cannot read a frame from video stream" << endl;
			break;
		}

		Mat imgYCbCr;
		Mat imgThresholded;

		//working set
		cvtColor(imgOriginal, imgYCbCr, COLOR_BGR2YCrCb); //Convert the captured frame from BGR to YCbCr

		inRange(imgYCbCr, Scalar(iLowY, iLowCb, iLowCr), Scalar(iHighY, iHighCb, iHighCr), imgThresholded); //Threshold the image : For YCbCr

		// Median blur with 5 window size
		medianBlur(imgThresholded, imgThresholdedMedian, 5);	// From 5 to 11: Otherwise median filter will be slow

		// Display different version of the images
		
		namedWindow("thresholded", CV_WINDOW_AUTOSIZE);
		imshow("thresholded", imgThresholded); //show the original image

		namedWindow("Original", CV_WINDOW_AUTOSIZE);
		imshow("Original", imgYCbCr); //show the original image

		namedWindow("median_object");
		imshow("median_object", imgThresholdedMedian); //show the thresholded image
		//-------------------Revised-------------------------------------//
		while (!readyToStart){
			if (waitKey(30) == 13){
				readyToStart = 1;
				cout << "Enter key pressed by user, ready to start" << endl;
			}
		}
		//-------------------Revised end-------------------------------------//

		// Call this function to detect the edges of the red object
		thresh_callback(0, 0);

		// Clock the end time
		clock_gettime(1, &tv_end);

		// Find the time of operation
		printf("Tick Count = %d\n", tv_end.tv_usec - tv_start.tv_usec);

		if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
		{
			cout << "esc key is pressed by user" << endl;
			break;
		}
	}
	return 0;
}


// Reference links is below mentioned
// https://github.com/Itseez/opencv/blob/master/samples/cpp/tutorial_code/ShapeDescriptors/generalContours_demo2.cpp
// Code that finds the minimum area cross section of the image sends command to the arduino.
void thresh_callback(int, void*)
{
	Mat threshold_output;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	/// Detect edges using Threshold
	findContours(imgThresholdedMedian, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	/// Find the rotated rectangles and ellipses for each contour
	vector<RotatedRect> minRect(contours.size());
	RotatedRect maxAreaminRect;
	int maxArea = 100;


	for (size_t i = 0; i < contours.size(); i++)
	{
		minRect[i] = minAreaRect(Mat(contours[i]));

		if ((minRect[i].size.width * minRect[i].size.height) > maxArea)
		{
			maxArea = (minRect[i].size.width * minRect[i].size.height);
			maxAreaminRect = minRect[i];
		}
	}

	if (maxArea <= 100)
	{
		if (!stopCommandFlag) {
			iRobotCommandFormatVelocity(0, 0, serialDataOverNetwork);

			sendCommands(serialDataOverNetwork);

			stopCommandFlag = 1;
		}

		prevTime = clock();
		return;
	}

	Mat drawing = Mat::zeros(imgThresholdedMedian.size(), CV_8UC3);

	Scalar color = Scalar(0, 255, 0);//	rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
	int borderThickness = 1;

	// Startup calibration time
	if (stopCommandFlag) {
		double currentTime = clock();

		int elapsedTime = (int)((currentTime - prevTime) / CLOCKS_PER_SEC);
		if (elapsedTime <= CALIBRATION_TIME_MAX_RANGE){
			mMax = maxArea;
			color = Scalar(255, 0, 0);
			borderThickness = 3;
		}
		else if (elapsedTime <= CALIBRATION_TIME_MIN_RANGE) {
			//----------------------Revised------------------------
			if (maxArea > mMax){
				mMin = mMax;
				mMax = maxArea;
			}
			else{
				mMin = maxArea;
			}
			//----------------------Revised------------------------
			color = Scalar(0, 0, 255);
			borderThickness = 3;
		}
		else if (elapsedTime <= CALIBRATION_COMPLETE_WAIT_TIME) {
			color = Scalar(255, 0, 255);
			borderThickness = 2;
		}
		else {
			stopCommandFlag = 0;
		}
	}


	// rotated rectangle
	Point2f rect_points[4]; maxAreaminRect.points(rect_points);
	for (int j = 0; j < 4; j++)
		line(drawing, rect_points[j], rect_points[(j + 1) % 4], color, borderThickness, 8);

	/// Show in a window
	namedWindow("Contours_1", WINDOW_AUTOSIZE);
	imshow("median_object", drawing);

	if (stopCommandFlag){
		return;
	}


	int l = (int)maxAreaminRect.angle;

	int currentWidth = maxAreaminRect.size.width;
	int currentHeight = maxAreaminRect.size.height;
	int currentArea = maxArea;

	// Right direction turns Inclination angle between -1 to -89 (Don't consider 0 and -90 it's confusing
	if (currentHeight < currentWidth)
	{
		betaMax = MIN_TURN_RIGHT;
		betaMin = MAX_TURN_RIGHT;
		lMin = MAX_RECT_ANGLE; // Max 
		lMax = MIN_RECT_ANGLE;
	}

	// Left direction 
	//else if (currentHeight >= currentWidth)
	else
	{
		betaMax = MAX_TURN_LEFT;
		betaMin = MIN_TURN_LEFT;
		lMin = MAX_RECT_ANGLE;
		lMax = MIN_RECT_ANGLE;
	}

	// alpha and m => for velocity
	// beta and l => for radius
	int alpha = rangeConversion(alphaMin, alphaMax, currentArea, mMin, mMax);
	cout << "alphaMin = " << alphaMin << "\t alphaMax = " << alphaMax << "\t currentArea = " << currentArea << "\t mMin = " << mMin << "\t mMax = " << mMax << endl;
	int beta = rangeConversion(betaMin, betaMax, l, lMin, lMax); // betaMin + ((betaMax - betaMin) * (l - lMin) / (lMax - lMin));
	cout << "betaMin = " << betaMin << "\t betaMax = " << betaMax << "\t l = " << l << "\t lMin = " << lMin << "\t lMax = " << lMax << endl;
	int velocitySpeed = alpha;
	int rotationAngle = beta;
	cout << "velocity=" << velocitySpeed << "\t angle" << rotationAngle << endl;


	iRobotCommandFormatVelocity(velocitySpeed, rotationAngle, serialDataOverNetwork);

	sendCommands(serialDataOverNetwork);
}


// Function which converts value of l within the range of (lMin, lMax) to the value beta within new range i.e. (betaMin, betaMax)
// Referenced link: Part of
// http://ieeexplore.ieee.org/xpl/login.jsp?tp=&arnumber=6780120&url=http%3A%2F%2Fieeexplore.ieee.org%2Fxpls%2Fabs_all.jsp%3Farnumber%3D6780120
int rangeConversion(int betaMin, int betaMax, int l, int lMin, int lMax) {

	l = (l < lMin) ? lMin : l;
	l = (l > lMax) ? lMax : l;
	//if (lMax != lMin){
	//cout << "l=" << l << endl;
	//cout << "lMax = " << lMax << "\t lMin = " << lMin << endl;
		return  (betaMin + ((betaMax - betaMin) * (l - lMin) / (lMax - lMin+0.1))); //}
	//else {
	//	return betaMin;
	//}
}

// Creates a byte of command which is compatible with iRobot
void iRobotCommandFormatVelocity(int velocitySpeed, int rotationAngle, char * stringDataOverNetwork) {

	*(stringDataOverNetwork + 0) = (char)((velocitySpeed & 0xFF00) >> 8);
	*(stringDataOverNetwork + 1) = (char)(velocitySpeed & 0x00FF);

	*(stringDataOverNetwork + 2) = (char)((rotationAngle & 0xFF00) >> 8);
	*(stringDataOverNetwork + 3) = (char)(rotationAngle & 0x00FF);
	*(stringDataOverNetwork + 4) = NULL;
}

// Sends command into the network (in our case, arduino)
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
