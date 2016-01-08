#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include <iostream>
#include <time.h>

using namespace cv;
using namespace std;

Mat capturedDrawing; //input parameter to pass for contourAnalysis()

int contourAnalysis(Mat);

bool newImageRequest = false;
int numberOfEdge = 0;

int main(int argc, const char** argv) {

	int startFlag = 0;
	int timeCounter = 0;

	VideoCapture cap(0);
	cap.set(CV_CAP_PROP_FRAME_WIDTH, 320);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, 240);
	if (!cap.isOpened()) {
		cout << "ERROR" << endl;
		system("pause"); // wait for key press
		return -1;
	}
	double fps = cap.get(CV_CAP_PROP_FPS);
	cout << "FPS is:" << fps << endl;
	double dWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH); //get the width of frames of the video
	double dHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT); //get the height of frames of the video

	cout << "Frame size: " << dWidth << "x" << dHeight << endl;

	namedWindow("myVideo", CV_WINDOW_AUTOSIZE);
	int iLowH = 170;
	int iHighH = 179;

	int iLowS = 150;
	int iHighS = 255;

	int iLowV = 60;
	int iHighV = 255;


	cvCreateTrackbar("LowH", "myVideo", &iLowH, 179);
	cvCreateTrackbar("HighH", "myVideo", &iHighH, 179);

	cvCreateTrackbar("LowS", "myVideo", &iLowS, 255);
	cvCreateTrackbar("High", "myVideo", &iHighS, 255);

	cvCreateTrackbar("LowV", "myVideo", &iLowV, 255);
	cvCreateTrackbar("highV", "myVideo", &iHighV, 255);

	//Capture a temporary image from the camera
	Mat imgTmp;
	cap.read(imgTmp);

	//Create a black image with the size as the camera output
	Mat imgLines = Mat::zeros(imgTmp.size(), CV_8UC3);;


	int lastPosX = -1;
	int lastPosY = -1;
	bool firstFrame = true;
	bool drawing = false;
	bool doneDrawing = false;

	Mat currentFrame, previousFrame, differenceFrame;

	double dM01 = 0;
	double dM10 = 0;
	double dArea = 0;

	double diffM01 = 0;
	double diffM10 = 0;
	double diffArea = 0;



	while (1) {

		Mat ImgOriginal;

		bool bSuccess = cap.read(ImgOriginal);

		if (!bSuccess) {
			cout << "failed to open video camera" << endl;
			break;
		}
		Mat imgHSV;
		cvtColor(ImgOriginal, imgHSV, COLOR_BGR2HSV);
		Mat imgThresholded;
		//Mat imgThresholdedMedian;


		inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image


		//morphological opening (remove small objects from the foreground)
		erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

		//morphological closing (fill small holes in the foreground)
		dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));


		currentFrame = imgThresholded.clone();

		if (firstFrame) {
			previousFrame = imgThresholded.clone();
			differenceFrame = imgThresholded.clone();
			firstFrame = false;
		}


		//Calculate the moments of the thresholded image
		Moments oMoments = moments(imgThresholded);

		dM01 = oMoments.m01;
		dM10 = oMoments.m10;
		dArea = oMoments.m00;


		// if the area <= 10000, I consider that the there are no object in the image and it's because of the noise, the area is not zero 
		if (dArea > 40000)
		{
			timeCounter = timeCounter + 1;
			//calculate the position of the ball
			int posX = dM10 / dArea;
			int posY = dM01 / dArea;

			lastPosX = posX;
			lastPosY = posY;

			if (timeCounter % 10 == 0) {

				absdiff(currentFrame, previousFrame, differenceFrame);
				currentFrame.copyTo(previousFrame);

				Moments diffMoments = moments(differenceFrame);

				diffM01 = diffMoments.m01;
				diffM10 = diffMoments.m10;
				diffArea = diffMoments.m00;

			}
			//drawing in process 
			if (diffArea > 30000 && timeCounter > 50 && doneDrawing == false) {
				drawing = true;
				//cout << "diffArea: " << diffArea << endl;
				line(imgLines, Point(lastPosX, lastPosY), Point(posX, posY), Scalar(255, 255, 255), 20);
				lastPosX = posX;
				lastPosY = posY;
				startFlag = 1;

			}

			//drawing done bc object captured but no motion
			if (diffArea < 30000 && timeCounter > 50 && doneDrawing == false) {
				doneDrawing = true;
				capturedDrawing = imgLines.clone();
				cout << ">>>>DRAWING IS DONE<<<<" << endl;

			}
		}

		else { //object not captured

			//ran out of bounds while drawing
			if (diffArea > 30000 && timeCounter > 50 && doneDrawing == false) {

				cout << ">>>>Drawing out of bounds<<<<" << endl;

				//refresh drawing page
				imgLines = Mat::zeros(imgTmp.size(), CV_8UC3);;

			}

			startFlag = 0;
			timeCounter = 0;



		}

		if (doneDrawing && newImageRequest == false) {
			numberOfEdge = contourAnalysis(capturedDrawing);
			newImageRequest = true;
			cout << "# of edges: " << numberOfEdge << endl;

		}




		imshow("objectThresholdMedian", imgThresholded);
		imshow("handDrawn", imgLines);
		imshow("original", ImgOriginal);
		imshow("diff", differenceFrame);



		if (startFlag == 1 && (timeCounter % 10 == 0)) {

			//cout << "Time count: " << timeCounter << endl;
		}

		if (waitKey(30) == 27) //wait for 'esc' key press for 30 ms. If 'esc' key is pressed, break loop
		{
			cout << "esc key is pressed by user" << endl;
			break;
		}



	}
	return 0;

}

int contourAnalysis(Mat capturedMat)
{
	cout << "in contourAnalysis" << endl;
	
	Mat capturedRaw, median, grayImg, grayThreshold, erodeOut;
	RNG rng(12345);
	capturedRaw = capturedMat.clone();
	medianBlur(capturedRaw, median, 5);


	cvtColor(median, grayImg, COLOR_RGB2GRAY);
	threshold(grayImg, grayThreshold, 250, 255, 1);

	// Create a structuring element
	int erosion_size = 6;
	Mat element2 = getStructuringElement(cv::MORPH_CROSS,
		cv::Size(2 * erosion_size + 1, 2 * erosion_size + 1),
		cv::Point(erosion_size, erosion_size));

	erode(grayThreshold, erodeOut, element2);

	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	vector<Point> polyApprox;
	int numberOfEdgeSum = 0;

	findContours(erodeOut, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	//draw edges
	Mat drawing = Mat::zeros(erodeOut.size(), CV_8UC3);
	for (int i = contours.size()-1; i < contours.size(); i++) {


		approxPolyDP(Mat(contours[i]), polyApprox, arcLength(Mat(contours[i]), true)*0.02, true);
		numberOfEdgeSum =  polyApprox.size();
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point());

		cout << "Counted edge in contour:" << numberOfEdgeSum << endl;

	}
	numberOfEdge = numberOfEdgeSum;

	//imshow("erode", erodeOut);
	imshow("function", drawing);
	return numberOfEdge;
}
