#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include <iostream>
#include <time.h>

using namespace cv;
using namespace std;

void Threshold_Demo(int, void*);

RNG rng(12345);

Mat originalImg, grayImg, grayThreshold;

int  threshold_value = 250;
int const max_value = 255;
char* trackbar_value = "Value";
char* window_name = "window";

int result = 0;
int polyEdgeNum = 0;

int main(int argc, const char** argv) {

	//load original image
	//originalImg = imread("blackShapes.PNG", CV_LOAD_IMAGE_UNCHANGED);
	originalImg = imread("circleRaw.PNG", CV_LOAD_IMAGE_UNCHANGED);

	if (originalImg.empty()) {
		cout << "failed" << endl;
		return -1;
	}

	
	namedWindow("window", CV_WINDOW_AUTOSIZE);

	//convert to grayscale
	cvtColor(originalImg, grayImg, COLOR_RGB2GRAY);

	//threshold
	//createTrackbar(trackbar_value, window_name, &threshold_value, max_value, Threshold_Demo);
	threshold(grayImg, grayThreshold, threshold_value, max_value, 1);
	medianBlur(grayThreshold, grayThreshold, 3);
	//contour analysis
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	vector<Point> polyApprox;

	
	findContours(grayThreshold, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	//draw edges
	Mat drawing = Mat::zeros(grayThreshold.size(), CV_8UC3);
	for (int i = 0; i < contours.size(); i++) {


		approxPolyDP(Mat(contours[i]), polyApprox, arcLength(Mat(contours[i]), true)*0.02, true);
		polyEdgeNum = polyApprox.size();
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point());

	} 
	
	imshow("raw", originalImg);
	//imshow("window", grayImg);
	//imshow("threshold", grayThreshold);
	imshow("contours", drawing);
	waitKey(0);


	return 0;

}

void Threshold_Demo(int, void*)
{
	/* 0: Binary
	1: Binary Inverted
	2: Threshold Truncated
	3: Threshold to Zero
	4: Threshold to Zero Inverted
	*/

	threshold(grayImg, grayThreshold, threshold_value, max_value, 1);

	imshow(window_name, grayThreshold);
}