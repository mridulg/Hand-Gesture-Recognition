//
//  main.cpp
//  videoCapture
//
//  Created by Mridul Gupta on 5/3/16.
//  Copyright © 2016 Mridul Gupta. All rights reserved.
//
#include "opencv2/opencv.hpp"
#include <iostream>
using namespace cv;

int main(int, char**)
{
    VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;
    
    Mat edges;
    namedWindow("edges",1);
    for(;;)
    {
        Mat frame, rgb;
        cap >> frame; // get a new frame from camera
        cvtColor(frame, edges, COLOR_BGR2GRAY);
//        cvtColor(edges,rgb, CV_GRAY2RGB);
        GaussianBlur(edges,edges, Size(7,7), 1.5, 1.5);
        Canny(edges,edges, 0, 30, 3);
        imshow("edges", edges);
        if(waitKey(30) >= 0) break;
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}