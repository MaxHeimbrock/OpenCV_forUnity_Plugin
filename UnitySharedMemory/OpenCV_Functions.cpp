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
	__declspec(dllexport) void ProcessImage(Color32 **rawImage, int width, int height)
	{

		// create an opencv object sharing the same data space
		Mat image(height, width, CV_8UC4, *rawImage);

		// start with flip (in both directions) if your image looks inverted
		flip(image, image, -1);

		// start processing the image
		// ************************************************

		Mat edges;
		Canny(image, edges, 50, 200);
		dilate(edges, edges, (5, 5));
		cvtColor(edges, edges, COLOR_GRAY2RGBA);
		normalize(edges, edges, 0, 1, NORM_MINMAX);
		multiply(image, edges, image);

		// end processing the image
		// ************************************************

		// flip again (just vertically) to get the right orientation
		flip(image, image, 0);
		flip(image, image, 1);
	}
}