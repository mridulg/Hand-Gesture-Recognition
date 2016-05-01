//
//  temp.cpp
//  testSegmentation
//
//  Created by Mridul Gupta on 4/1/16.
//  Copyright © 2016 Mridul Gupta. All rights reserved.
//

//
//  References : http://paulbourke.net/geometry/circlesphere/
//               http://s-ln.in/2013/04/18/hand-tracking-and-gesture-detection-opencv/
//               http://docs.opencv.org/2.4/doc/tutorials/imgproc/shapedescriptors/find_contours/find_contours.html
//               http://docs.opencv.org/2.4/doc/tutorials/imgproc/shapedescriptors/hull/hull.html
//
//

#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
using namespace cv;

void displayImage(Mat , string);
void ASL_Detection(Mat );
double distPoints(Point , Point);
int main(int argc, const char * argv[])
{
	Mat orgImg;
	orgImg = imread("/Users/mridul/Desktop/3.jpg", CV_LOAD_IMAGE_ANYCOLOR);
	if(orgImage.empty())
	{
		cout<<"Issues with loading image. Please try again"<<endl;
		return 0;
	}
	displayImage(orgImage,"Original Image");
	return 0;
}
void displayImage(Mat img, string text)
{
	namedWindow(text, WINDOW_AUTOSIZE );
	imshow(text,img);
	waitKey(0);
	destroyWindow(text);
}
