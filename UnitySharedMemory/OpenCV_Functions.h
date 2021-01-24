#pragma once

#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

using namespace cv;

struct Color32
{
	uchar red;
	uchar green;
	uchar blue;
	uchar alpha;
};

struct Vector2
{
	float x, y;
};

struct Matrix4x4
{
	float m0, m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13, m14, m15;
};

extern "C" __declspec(dllexport) void ProcessImage(Color32 **rawImage, int width, int height, Matrix4x4 &cameraPosition, Vector2 &ufoPosition, Vector2 &laserPosition);