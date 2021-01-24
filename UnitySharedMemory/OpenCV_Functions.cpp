#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include "OpenCV_Functions.h"

using namespace std;
using namespace cv;


extern "C"
{
	__declspec(dllexport) void ProcessImage(Color32 **rawImage, int width, int height, Matrix4x4 &cameraPosition, Vector2 &ufoPosition, Vector2 &laserPosition)
	{

		// create an opencv object sharing the same data space
		Mat image(height, width, CV_8UC4, *rawImage);
		Mat image_rgb;
		cv::cvtColor(image, image_rgb, cv::COLOR_RGBA2BGR);

		FindUfoAndLaser(image_rgb, ufoPosition, laserPosition);

		circle(image, Point(laserPosition.x, laserPosition.y), 3, Scalar(255, 0, 0), -1);

		/*
		// start processing the image
		// ************************************************

		Mat edges;
		Canny(image, edges, 50, 200);
		dilate(edges, edges, (5, 5));
		cvtColor(edges, edges, COLOR_GRAY2RGBA);
		normalize(edges, edges, 0, 1, NORM_MINMAX);
		multiply(image, edges, image);

		ufoPosition.x = 0.5f;
		ufoPosition.y = -2.0f;

		cameraPosition.m0 = 7.2;

		// end processing the image
		// ************************************************
		*/

	}
}



void FindUfoAndLaser(Mat &playArea, Vector2 &ufoPosition, Vector2 &laserPosition)
{
	Mat playAreaTemp;
	cvtColor(playArea, playAreaTemp, CV_BGR2HSV);
	std::vector<std::vector<cv::Point> > contours;

	// for yellow
	Mat mask_yellow;
	inRange(playAreaTemp, Scalar(10, 50, 50), Scalar(50, 255, 255), mask_yellow);

	cv::findContours(mask_yellow, contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);

	if (contours.size() != 0)
	{
		double maxArea = 0;
		int maxAreaContourId = 0;
		for (int j = 0; j < contours.size(); j++) {
			double newArea = cv::contourArea(contours.at(j));
			if (newArea > maxArea) {
				maxArea = newArea;
				maxAreaContourId = j;
			}
		}

		Moments mu = moments(contours[maxAreaContourId], false);
		ufoPosition.x = (mu.m10 / mu.m00);
		ufoPosition.y = (mu.m01 / mu.m00);
	}

	// for red
	Mat1b mask1, mask2;
	inRange(playAreaTemp, Scalar(0, 70, 50), Scalar(10, 255, 255), mask1);
	inRange(playAreaTemp, Scalar(170, 70, 50), Scalar(180, 255, 255), mask2);

	Mat1b mask_red = mask1 | mask2;

	cv::findContours(mask_red, contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);

	if (contours.size() != 0)
	{
		double maxArea = 0;
		int maxAreaContourId = 0;
		for (int j = 0; j < contours.size(); j++) {
			double newArea = cv::contourArea(contours.at(j));
			if (newArea > maxArea) {
				maxArea = newArea;
				maxAreaContourId = j;
			}
		}

		Moments mu = moments(contours[maxAreaContourId], false);
		laserPosition.x = (mu.m10 / mu.m00);
		laserPosition.y = (mu.m01 / mu.m00);
	}
}


