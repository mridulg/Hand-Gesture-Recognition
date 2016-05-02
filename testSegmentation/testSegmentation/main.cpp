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
	int i=0;
	orgImg = imread("/Users/mridul/Desktop/3.jpg", CV_LOAD_IMAGE_ANYCOLOR);
	if(orgImage.empty())
	{
		cout<<"Issues with loading image. Please try again"<<endl;
		return 0;
	}
	displayImage(orgImage,"Original Image");
	if (orgImg.type())
		orgImg = GetSkin(orgImg);
	displayImage(orgImage,"Skin Segmented Image");
	while(i<orgImg.cols)
	{
		orgImg.at<uchar>(orgImg.rows - 1, i) = 0;
		orgImg.at<uchar>(orgImg.rows - 2, i) = 0;
		orgImg.at<uchar>(orgImg.rows - 3, i) = 0;
		orgImg.at<uchar>(orgImg.rows - 4, i) = 0;
		orgImg.at<uchar>(orgImg.rows - 5, i) = 0;
		i++;
	}
	erode(orgImg,orgImg,Mat());
	dilate(orgImg,orgImg,Mat());
	return 0;
}

void displayImage(Mat img, string text)
{
	namedWindow(text, WINDOW_AUTOSIZE );
	imshow(text,img);
	waitKey(0);
	destroyWindow(text);
}

Mat getSkin(Mat img)
{
	Mat outImg=img.clone();
	Mat imgYCRCB, imgHSV;
	bool rbg=false;
	Vec3b black = Vec3b::all(0);
	//    bool b,c ;
	Vec3b white = Vec3b::all(255);
	img.convertTo(imgHSV, CV_32FC3);
    cvtColor(imgHSV,imgHSV, CV_BGR2HSV);
    normalize(imgHSV,imgHSV, 0.0, 255.0, NORM_MINMAX, CV_32FC3);
    cvtColor(img, imgYCRCB, CV_BGR2YCrCb);
    int i=0,j=0; //for-loop initializers
    while(i<img.rows)
    {
    	while(j<img.cols)
    	{
    		Vec3f hsv = imgHSV.ptr<Vec3f>(i)[j];
    		Vec3b bgr = img.ptr<Vec3b>(i)[j];
    		Vec3b ycrcb = imgYCRCB.ptr<Vec3b>(i)[j];
    		
    		int B = bgr.val[0];
    		int G = bgr.val[1];
    		int R = bgr.val[2];
    		j++;
    		if ((R>95)&&(G>40)&&(B>20)&&((max(R,max(G,B))-min(R,min(G,B)))>15)&&(abs(R-G)>15)&&(R>G)&&(R>B))
    		rgb = true;
    		if(rgb)
    			outImg.ptr<Vec3b>(i)[j]=white;
    		else outImg.ptr<Vec3b>(i)[j]=black;
    	}
    	i++;
    }
    return outImg;
}
