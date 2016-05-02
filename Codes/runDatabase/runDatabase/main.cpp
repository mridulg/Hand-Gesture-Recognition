//
//  main.cpp
//  ImageAnalysis_Assignment3
//
//  Created by Mridul Gupta on 02/04/16.
//  Copyright © 2015 Mridul Gupta. All rights reserved.
//

/*
 
 
 Question 3 and 4 :
 Report of comparison attached.
 
 */


#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <string>
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/objdetect/objdetect.hpp"

#define PI 3.14159265
#define min(i, j) (i <= j ? i : j)
#define INF 10000

using namespace std;
using namespace cv;

void displayImage(Mat , string);


Mat convertVec3bToBinary(Mat );
Mat medianFilter(Mat );
Mat erosion(Mat , int **arr, int);
Mat dilation(Mat , int **arr, int);
void genStructingElement1(int , int **arr);
Mat diff(Mat , Mat );
int countBead(Mat , int **arr, int);


Mat keyPoints(Mat );
vector < vector < float > > HOG(Mat );
float cosine_dis(vector < float > , vector < float > );


Mat lbp(Mat );
int lbpValue(int [][5]);
void faceDivide(Mat , vector < int > [64]);
float chiSquareDiff(vector < int > [64], vector < int > [64]);


Mat GetSkin(Mat );

int main(int argc, const char * argv[]) {
    
    
    //    Histogram of Oriented Gradients (HOGs)
    // --------------------------------------------------------------------------------------------------------
    //
    //    Mat orgImg1;
    //    orgImg1 = imread("/Users/lifecodemohit/Documents/IIITD_Sem5/ImageAnalysis/Slides/FaceImages/1_5.jpg", 0);
    //
    //    if(orgImg1.empty()) {
    //        cout << "Image not loaded properly." << endl;
    //        return  0;
    //    }
    //
    //    displayImage(orgImg1, "Original Image 1");
    //    cout << orgImg1.rows << " " << orgImg1.cols << endl;
    //    Mat keyImg1 = keyPoints(orgImg1);
    //    vector < vector < float > > des1 = HOG(keyImg1);
    //
    //    Mat orgImg2;
    //    orgImg2 = imread("/Users/lifecodemohit/Documents/IIITD_Sem5/ImageAnalysis/Slides/FaceImages/7_3.jpg", 0);
    //
    //    if(orgImg2.empty()) {
    //        cout << "Image not loaded properly." << endl;
    //        return  0;
    //    }
    //
    //    displayImage(orgImg2, "Original Image 2");
    //    cout << orgImg2.rows << " " << orgImg2.cols << endl;
    //    Mat keyImg2 = keyPoints(orgImg2);
    //    vector < vector < float > > des2 = HOG(keyImg2);
    //
    //    float answer = 0;
    //    for (int i = 0; i < 16; i++) {
    //        answer += cosine_dis(des1[i], des2[i]);
    //    }
    //
    //    cout << answer/16.0 << endl;
    //
    // --------------------------------------------------------------------------------------------------------
    //
    // HOG Loop computation
    // --------------------------------------------------------------------------------------------------------
    
    cout << "HOG Loop computation : " << endl;
    int accuracy = 0;
    int ctr=0;
    for (int i1 = 0; i1 < 10; i1++) {
        for (int j1 = 5; j1 <= 10; j1++) {
            ctr+=1;
            float MIN = 100000.0;
            int imagePos1 = -1;
            int imagePos2 = -1;
            for (int l1 = 0; l1 < 10; l1++) {
                for (int k1 = 1; k1 <= 4; k1++) {
                    float result = 0.0;
                    Mat orgImg1;
                    
                    char first1[] = "/Users/mridul/HGR/Datasets/myDataset/";
                    stringstream f1;
                    f1 << first1;
                    
                    stringstream m1, m2, m3;
                    m1 << i1;
                    m2 << "_";
                    m3 << j1;
                    
                    char end1[] = ".png";
                    stringstream e1;
                    e1 << end1;
                    
                    f1 << m1.str();
                    f1 << m2.str();
                    f1 << m3.str();
                    f1 << e1.str();
                    
                    orgImg1 = imread(f1.str(), 0);
                    cout <<"Source: "<< f1.str() << endl;
                    
                    if(orgImg1.empty()) {
                        cout << "Image not loaded properly." << endl;
                        return  0;
                    }
                    
                    //                     displayImage(orgImg1, "Original Image 1");
                    Mat keyImg1 = keyPoints(orgImg1);
                    vector < vector < float > > des1 = HOG(keyImg1);
                    
                    Mat orgImg2;
                    char first2[] = "/Users/mridul/HGR/Datasets/myDataset/";
                    stringstream f2;
                    f2 << first2;
                    
                    stringstream m11, m12, m13;
                    m11 << l1;
                    m12 << "_";
                    m13 << k1;
                    
                    char end2[] = ".png";
                    stringstream e2;
                    e2 << end2;
                    
                    f2 << m11.str();
                    f2 << m12.str();
                    f2 << m13.str();
                    f2 << e2.str();
                    
                    cout <<"Destination: "<< f2.str() << endl;
                    
                    orgImg2 = imread(f2.str(), 0);
                    
                    
                    
                    if(orgImg2.empty()) {
                        cout << "Imagen not loaded properly." << endl;
                        return  0;
                    }
                    
                    // displayImage(orgImg2, "Original Image 2");
                    
                    Mat keyImg2 = keyPoints(orgImg2);
                    vector < vector < float > > des2 = HOG(keyImg2);
                    
                    float answer = 0;
                    for (int i = 0; i < 16; i++) {
                        answer += cosine_dis(des1[i], des2[i]);
                    }
                    cout<<"Answer is "<<answer<<endl;
                    result = (answer/16.0);
                    cout<<"prelim result for "<<f1.str()<<" with "<<f2.str()<<" is "<<result<<endl;
                    if (result < MIN) {
                        MIN = result;
                        imagePos1 = l1;
                        imagePos2 = k1;
                    }
                }
            }
            cout << endl;
            cout << "Image " << i1 << "_" << j1 << ".jpg minimum cosine measure : " << MIN << "." << endl;
            cout << "Best match of image " << i1 << "_" << j1 << ".jpg is image " << imagePos1 << "_" << imagePos2 << ".png" << endl;
            if (imagePos1 == i1)
                accuracy++;
        }
    }
    
    cout << endl;
    cout << "Accuracy : " << (accuracy*100.0)/ctr << "% "<< endl;
    // --------------------------------------------------------------------------------------------------------
    
    // -------------------------------------------------------------------------------------------------------
    
    //  Local Binary Pattern
    // --------------------------------------------------------------------------------------------------------
    
    //    Mat orgImg1;
    //    orgImg1 = imread("/Users/lifecodemohit/Documents/IIITD_Sem5/ImageAnalysis/Slides/FaceImages/1_10.jpg", 0);
    //
    //    if(orgImg1.empty()) {
    //        cout << "Image not loaded properly." << endl;
    //        return  0;
    //    }
    //
    //    displayImage(orgImg1, "Original Image 1");
    //
    //    Mat orgImg2;
    //    orgImg2 = imread("/Users/lifecodemohit/Documents/IIITD_Sem5/ImageAnalysis/Slides/FaceImages/9_2.jpg", 0);
    //
    //    if(orgImg2.empty()) {
    //        cout << "Image not loaded properly." << endl;
    //        return  0;
    //    }
    //
    //    displayImage(orgImg2, "Original Image 2");
    //
    //    Mat lbpImg1 = lbp(orgImg1);
    //    Mat lbpImg2 = lbp(orgImg2);
    //
    //    vector < int > Reg1[64];
    //    vector < int > Reg2[64];
    //
    //    for (int i = 0;i < 64; i++) Reg1[i].clear();
    //    faceDivide(lbpImg1, Reg1);
    //    for (int i = 0;i < 64; i++) Reg2[i].clear();
    //    faceDivide(lbpImg2, Reg2);
    //
    //    float result = chiSquareDiff(Reg1, Reg2);
    //    cout << result << endl;
    
    // --------------------------------------------------------------------------------------------------------
    
    // LBP 3 Loop computation
    // --------------------------------------------------------------------------------------------------------
    
    //    cout << "Question 3 Loop computation : " << endl;
    //    int accuracy = 0;
    //    for (int i1 = 1; i1 <= 10; i1++) {
    //        for (int j1 = 5; j1 <= 10; j1++) {
    //            int MIN = INT_MAX;
    //            int imagePos1 = -1;
    //            int imagePos2 = -1;
    //            for (int l1 = 1; l1 <= 10; l1++) {
    //                for (int k1 = 1; k1 <= 4; k1++) {
    //                    float result = 0.0;
    //                    Mat orgImg1;
    //
    //                    char first1[] = "/Users/lifecodemohit/Documents/IIITD_Sem5/ImageAnalysis/Slides/FaceImages/";
    //                    stringstream f1;
    //                    f1 << first1;
    //
    //                    stringstream m1, m2, m3;
    //                    m1 << i1;
    //                    m2 << "_";
    //                    m3 << j1;
    //
    //                    char end1[] = ".jpg";
    //                    stringstream e1;
    //                    e1 << end1;
    //
    //                    f1 << m1.str();
    //                    f1 << m2.str();
    //                    f1 << m3.str();
    //                    f1 << e1.str();
    //
    //                    orgImg1 = imread(f1.str(), 0);
    //                    // cout << f1.str() << endl;
    //
    //                    if(orgImg1.empty()) {
    //                        cout << "Image not loaded properly." << endl;
    //                        return  0;
    //                    }
    //
    //                    // displayImage(orgImg1, "Original Image 1");
    //
    //                    Mat orgImg2;
    //                    char first2[] = "/Users/lifecodemohit/Documents/IIITD_Sem5/ImageAnalysis/Slides/FaceImages/";
    //                    stringstream f2;
    //                    f2 << first2;
    //
    //                    stringstream m11, m12, m13;
    //                    m11 << l1;
    //                    m12 << "_";
    //                    m13 << k1;
    //
    //                    char end2[] = ".jpg";
    //                    stringstream e2;
    //                    e2 << end2;
    //
    //                    f2 << m11.str();
    //                    f2 << m12.str();
    //                    f2 << m13.str();
    //                    f2 << e2.str();
    //
    //                    orgImg2 = imread(f2.str(), 0);
    //
    //                    // cout << f2.str() << endl;
    //
    //                    if(orgImg2.empty()) {
    //                        cout << "Image not loaded properly." << endl;
    //                        return  0;
    //                    }
    //
    //                    // displayImage(orgImg2, "Original Image 2");
    //
    //                    Mat lbpImg1 = lbp(orgImg1);
    //                    Mat lbpImg2 = lbp(orgImg2);
    //
    //                    vector < int > Reg1[64];
    //                    vector < int > Reg2[64];
    //
    //                    for (int i = 0;i < 64; i++) Reg1[i].clear();
    //                    faceDivide(lbpImg1, Reg1);
    //                    for (int i = 0;i < 64; i++) Reg2[i].clear();
    //                    faceDivide(lbpImg2, Reg2);
    //
    //                    result = chiSquareDiff(Reg1, Reg2);
    //                    if (result < MIN) {
    //                        MIN = result;
    //                        imagePos1 = l1;
    //                        imagePos2 = k1;
    //                    }
    //                }
    //            }
    //            cout << endl;
    //            cout << "Image " << i1 << "_" << j1 << ".jpg minimum chi square : " << MIN << "." << endl;
    //            cout << "Best match of image " << i1 << "_" << j1 << ".jpg is image " << imagePos1 << "_" << imagePos2 << ".jpg" << endl;
    //            if (imagePos1 == i1)
    //                accuracy++;
    //        }
    //    }
    //
    //    cout << endl;
    //    cout << "Accuracy : " << (accuracy*100.0)/60.0 << "% "<< endl;
    //
    // --------------------------------------------------------------------------------------------------------
    return 0;
}

void displayImage(Mat img, string text) {
    
    namedWindow(text, WINDOW_AUTOSIZE );
    imshow(text , img);
    waitKey(0);
    //    destroyWindow(text);
    
}

Mat convertVec3bToBinary(Mat img) {
    
    Mat outputImg = Mat::zeros(img.size(), CV_32F);
    
    for (int i = 6; i < img.rows - 6; i++) {
        for (int j = 6; j < img.cols - 6; j++) {
            outputImg.at<float>(i, j) = (((img.at<Vec3b>(i, j)[0] >= 0 && img.at<Vec3b>(i, j)[0] <= 170) && (img.at<Vec3b>(i, j)[1] >= 0 && img.at<Vec3b>(i, j)[1] <= 140) && (img.at<Vec3b>(i, j)[2] >= 0 && img.at<Vec3b>(i, j)[2] <= 140)) ? 0.0 : 255.0);
        }
    }
    
    // outputImg.convertTo(outputImg, CV_THRESH_BINARY);
    return outputImg;
}

Mat medianFilter(Mat img) {
    
    Mat outputImg = Mat::zeros(img.rows + 2, img.cols + 2, CV_32F);
    Mat foutputImg = Mat::zeros(img.size(), CV_32F);
    
    img.convertTo(img, CV_32F);
    
    for (int i = 0; i < img.rows; i++) {
        for (int j = 0; j < img.cols; j++) {
            outputImg.at<float>(i+1, j+1) = img.at<float>(i, j);
        }
    }
    
    for (int i = 0; i < img.rows; i++) {
        for (int j = 0; j < img.cols; j++) {
            vector < float > v1;
            for (int i1 = -1; i1 <= 1; i1++) {
                for (int j1 = -1; j1 <= 1; j1++) {
                    v1.push_back(outputImg.at<float>(i+1-i1, j+1-j1));
                }
            }
            sort(v1.begin(), v1.end());
            foutputImg.at<float>(i, j) = v1[5];
        }
    }
    
    // foutputImg.convertTo(foutputImg, CV_THRESH_BINARY);
    return foutputImg;
}


Mat erosion(Mat img, int **filter, int sz) {
    
    Mat outputImg = Mat::zeros(img.rows + sz - 1, img.cols + sz - 1, CV_32F);
    Mat foutputImg = Mat::zeros(img.size(), CV_32F);
    
    for (int i = 0; i < img.rows; i++) {
        for (int j = 0; j < img.cols; j++) {
            outputImg.at<float>(i+(sz/2), j+(sz/2)) = img.at<float>(i, j);
        }
    }
    
    for (int i = 0; i < img.rows; i++) {
        for (int j = 0; j < img.cols; j++) {
            float result = 255.0;
            for (int i1 = -(sz/2); i1 <= sz/2; i1++) {
                for (int j1 = -(sz/2); j1 <= sz/2; j1++) {
                    if (filter[i1+(sz/2)][j1+(sz/2)] == 255 && outputImg.at<float>(i+(sz/2)-i1, j+(sz/2)-j1) != (float)filter[i1+(sz/2)][j1+(sz/2)])
                        result = 0.0;
                }
            }
            foutputImg.at<float>(i, j) = result;
        }
    }
    
    return foutputImg;
}

Mat dilation(Mat img, int **filter, int sz) {
    
    Mat outputImg = Mat::zeros(img.rows + sz - 1, img.cols + sz - 1, CV_32F);
    Mat foutputImg = Mat::zeros(img.size(), CV_32F);
    
    for (int i = 0; i < img.rows; i++) {
        for (int j = 0; j < img.cols; j++) {
            outputImg.at<float>(i+(sz/2), j+(sz/2)) = img.at<float>(i, j);
        }
    }
    
    for (int i = 0; i < img.rows; i++) {
        for (int j = 0; j < img.cols; j++) {
            float result = 0.0;
            for (int i1 = -(sz/2); i1 <= sz/2; i1++) {
                for (int j1 = -(sz/2); j1 <= sz/2; j1++) {
                    if (filter[i1+(sz/2)][j1+(sz/2)] == 255 && outputImg.at<float>(i+(sz/2)-i1, j+(sz/2)-j1) == (float)filter[i1+(sz/2)][j1+(sz/2)])
                        result = 255.0;
                }
            }
            foutputImg.at<float>(i, j) = result;
        }
    }
    
    return foutputImg;
}

void genStructingElement1(int n, int **arr) {
    int k = n/2;
    for (int  i = 0; i <= n/2; i++) {
        for (int j = 0; j <= n/2; j++) {
            if (j < k) {
                arr[i][j] = arr[i][n-1-j] = 0;
                arr[n-1-i][j] = arr[n-1-i][n-1-j] = 0;
            }
            else {
                arr[i][j] = arr[i][n-1-j] = 255;
                arr[n-1-i][j] = arr[n-1-i][n-1-j] = 255;
            }
        }
        k--;
    }
}

Mat diff(Mat img1, Mat img2) {
    Mat outputImg = Mat::zeros(img1.rows, img1.cols, CV_32F);
    for (int i = 0; i < img1.rows; i++) {
        for (int j = 0; j < img1.cols; j++) {
            outputImg.at<float>(i, j) = img1.at<float>(i, j) - img2.at<float>(i, j);
        }
    }
    return outputImg;
}


int countBead(Mat img, int **filter, int sz) {
    Mat outputImg = Mat::zeros(img.rows + sz - 1, img.cols + sz - 1, CV_32F);
    
    for (int i = 0; i < img.rows; i++) {
        for (int j = 0; j < img.cols; j++) {
            outputImg.at<float>(i+(sz/2), j+(sz/2)) = img.at<float>(i, j);
        }
    }
    
    int cnt = 0;
    for (int i = 0; i < img.rows; i++) {
        for (int j = 0; j < img.cols; j++) {
            int count = 0;
            for (int i1 = -(sz/2); i1 <= sz/2; i1++) {
                for (int j1 = -(sz/2); j1 <= sz/2; j1++) {
                    if ((filter[i1+(sz/2)][j1+(sz/2)] == 255) && (outputImg.at<float>(i+(sz/2)-i1, j+(sz/2)-j1) != (float)filter[i1+(sz/2)][j1+(sz/2)]))
                        count++;
                }
            }
            
            if (count < 2*sz) {
                cnt++;
                for (int i1 = -(sz/2); i1 <= sz/2; i1++) {
                    for (int j1 = -(sz/2); j1 <= sz/2; j1++) {
                        outputImg.at<float>(i+(sz/2)-i1, j+(sz/2)-j1) = 0;
                    }
                }
            }
        }
    }
    
    // displayImage(outputImg, "Difference 1");
    return cnt;
}



Mat keyPoints(Mat img) {
    
    Mat outputImg = Mat::zeros(48, 48, CV_32F);
    Mat foutputImg = Mat::zeros(8, 8, CV_32F);
    
    img.convertTo(img, CV_32F);
    
    
    for (int i = 8; i < img.rows - 8; i++) {
        for (int j = 8; j < img.cols - 8; j++) {
            outputImg.at<float>(i - 8, j - 8) = img.at<float>(i, j);
            
        }
    }
    
    //    outputImg.convertTo(outputImg, CV_8U);
    //    displayImage(outputImg, "Image");
    
    for (int i = 0, i1 = 0; i < 8; i++, i1 += 6) {
        for (int j = 0, j1 = 0; j < 8; j++, j1 += 6) {
            foutputImg.at<float>(i, j) = outputImg.at<float>(i1, j1);
            //            cout << outputImg.at<float>(i1, j1) << " ";
        }
        //        cout << endl;
    }
    
    //        foutputImg.convertTo(foutputImg, CV_8U);
    //        displayImage(foutputImg, "Image");
    return foutputImg;
}

vector < vector < float > > HOG(Mat img) {
    
    Mat outputImg = Mat::zeros(img.rows + 2, img.cols + 2, CV_32F);
    
    for (int i = 0; i < img.rows; i++) {
        for (int j = 0; j < img.cols; j++) {
            outputImg.at<float>(i+1, j+1) = img.at<float>(i, j);
            //            cout << img.at<float>(i, j) << " ";
        }
        //        cout << endl;
    }
    
    float v[16][8];
    float ang[8][8], mag[8][8];
    
    for (int i = 1; i <= 8; i++) {
        for (int j = 1; j <= 8; j++) {
            mag[i-1][j-1] = sqrt(pow(outputImg.at<float>(i+1, j) - outputImg.at<float>(i-1, j), 2) + pow(outputImg.at<float>(i, j+1) - outputImg.at<float>(i, j-1), 2));
            ang[i-1][j-1] = atan2((outputImg.at<float>(i, j+1) - outputImg.at<float>(i, j-1)), (outputImg.at<float>(i+1, j) - outputImg.at<float>(i-1, j)));
            if (ang[i-1][j-1] < 0)
                ang[i-1][j-1] += 2*PI;
            ang[i-1][j-1] = ang[i-1][j-1]*(180/PI);
            //            cout << ang[i-1][j-1] << " " ;
        }
        //        cout << endl;
    }
    
    for (int i = 0; i < 16; i++) {
        for (int  j= 0; j < 8; j++) {
            v[i][j] = 0;
        }
    }
    
    for (int i= 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            int oct = (int)(ang[i][j]/45.0);
            float magnitude = mag[i][j];
            if (i < 2 && j < 2) {
                v[0][oct] += magnitude;
            }
            else if (i < 2 && j < 4) {
                v[1][oct] += magnitude;
            }
            else if (i < 2 && j < 6) {
                v[2][oct] += magnitude;
            }
            else if (i < 2 && j < 8) {
                v[3][oct] += magnitude;
            }
            else if (i < 4 && j < 2) {
                v[4][oct] += magnitude;
            }
            else if (i < 4 && j < 4) {
                v[5][oct] += magnitude;
            }
            else if (i < 4 && j < 6) {
                v[6][oct] += magnitude;
            }
            else if (i < 4 && j < 8) {
                v[7][oct] += magnitude;
            }
            else if (i < 6 && j < 2) {
                v[8][oct] += magnitude;
            }
            else if (i < 6 && j < 4) {
                v[9][oct] += magnitude;
            }
            else if (i < 6 && j < 6) {
                v[10][oct] += magnitude;
            }
            else if (i < 6 && j < 8) {
                v[11][oct] += magnitude;
            }
            else if (i < 8 && j < 2) {
                v[12][oct] += magnitude;
            }
            else if (i < 8 && j < 4) {
                v[13][oct] += magnitude;
            }
            else if (i < 8 && j < 6) {
                v[14][oct] += magnitude;
            }
            else if (i < 8 && j < 8) {
                v[15][oct] += magnitude;
            }
        }
    }
    
    vector < vector < float > > bin;
    for (int i = 0; i < 16; i++) {
        vector < float > reg;
        for (int  j= 0; j < 8; j++) {
            reg.push_back(v[i][j]);
        }
        bin.push_back(reg);
    }
    
    return bin;
}

float cosine_dis(vector < float > d1, vector < float > d2) {
    
    float dotp = 0.0, dend1 = 0.0, dend2 = 0.0;
    
    for (int i = 0; i < (int)d1.size(); i++) {
        dotp += d1[i]*d2[i];
        dend1 += d1[i]*d1[i];
        dend2 += d2[i]*d2[i];
    }
    
    float result = 1.0 - (dotp/(sqrt(dend1)*sqrt(dend2)));
    return result;
}


Mat lbp(Mat img) {
    
    img.convertTo(img, CV_32F);
    Mat outputImg = Mat::zeros(img.rows + 4, img.cols + 4, CV_32F);
    Mat foutputImg = Mat::zeros(img.rows, img.cols, CV_32F);
    Mat disoutputImg = Mat::zeros(img.rows, img.cols, CV_32F);
    
    for (int i = 0; i < img.rows; i++) {
        for (int j = 0; j < img.cols; j++) {
            outputImg.at<float>(i+2, j+2) = img.at<float>(i, j);
        }
    }
    
    
    for (int i = 0; i < img.rows; i++) {
        for (int j = 0; j < img.cols; j++) {
            
            int arr[5][5];
            for (int i1 = -2; i1 <= 2; i1++) {
                for (int j1 = -2; j1 <= 2; j1++) {
                    if (outputImg.at<float>(i + 2 - i1, j + 2 - j1) >= img.at<float>(i, j)) {
                        arr[i1+2][j1+2] = 1;
                    }
                    else {
                        arr[i1+2][j1+2] = 0;
                    }
                }
            }
            foutputImg.at<float>(i, j) = (float)lbpValue(arr);
            disoutputImg.at<float>(i, j) = (foutputImg.at<float>(i, j)/pow(2, 16))*255.0;
            foutputImg.at<float>(i, j) = (foutputImg.at<float>(i, j)/pow(2, 16))*255.0; // change
            
        }
    }
    
    disoutputImg.convertTo(disoutputImg, CV_8U);
    // displayImage(disoutputImg, "LBP Image");
    
    return foutputImg;
}

int lbpValue(int arr[][5]) {
    
    vector < pair < int, int > > filter;
    
    filter.push_back(make_pair(2, 0));
    filter.push_back(make_pair(3, 0));
    filter.push_back(make_pair(3, 1));
    filter.push_back(make_pair(4, 1));
    
    filter.push_back(make_pair(4, 2));
    filter.push_back(make_pair(4, 3));
    filter.push_back(make_pair(3, 3));
    filter.push_back(make_pair(3, 4));
    
    filter.push_back(make_pair(2, 4));
    filter.push_back(make_pair(1, 4));
    filter.push_back(make_pair(1, 3));
    filter.push_back(make_pair(0, 3));
    
    filter.push_back(make_pair(0, 2));
    filter.push_back(make_pair(0, 1));
    filter.push_back(make_pair(1, 1));
    filter.push_back(make_pair(1, 0));
    
    int power = 1;
    int result = 0;
    for (int i = (int)filter.size() - 1; i >= 0; i--) {
        result = result + arr[filter[i].first][filter[i].second]*power;
        power *= 2;
    }
    
    return result;
}

void faceDivide(Mat img, vector < int > Reg[64]) {
    
    int reg = -1;
    for (int i = 0; i < 64; i++) {
        for (int j = 0; j < pow(2, 8); j++) {       // 2^16 change
            Reg[i].push_back(0);
        }
    }
    
    for (int i = 0; i < img.rows; i += 8) {
        for (int j = 0; j < img.cols; j += 8) {
            reg++;
            for (int i1 = 0; i1 < 8; i1++) {
                for (int j1 = 0; j1 < 8; j1++) {
                    Reg[reg][(int)img.at<float>(i + i1, j + j1)] += 1;
                }
            }
        }
    }
}

float chiSquareDiff(vector < int > Reg1[64], vector < int > Reg2[64]) {
    
    float result = 0.0;
    for (int i = 0; i < 64; i++) {
        for (int j = 0; j < pow(2, 8); j++) {       // 2^16 change
            if (Reg1[i][j] + Reg2[i][j] != 0)
                result = result + (pow(Reg1[i][j] - Reg2[i][j], 2) / (float)(Reg1[i][j] + Reg2[i][j]));
        }
    }
    return result;
}


Mat GetSkin(Mat img) {
    
    Mat outImg = img.clone();
    Mat imgYCRCB, imgHSV;
    
    bool rgb;
    Vec3b black = Vec3b::all(0);
    
    //    bool b,c ;
    //    Vec3b white = Vec3b::all(255);
    
    img.convertTo(imgHSV, CV_32FC3);
    cvtColor(imgHSV,imgHSV, CV_BGR2HSV);
    normalize(imgHSV,imgHSV, 0.0, 255.0, NORM_MINMAX, CV_32FC3);
    cvtColor(img, imgYCRCB, CV_BGR2YCrCb);
    
    
    for(int i = 0; i < img.rows; i++) {
        
        for(int j = 0; j < img.cols; j++) {
            
            Vec3f hsv = imgHSV.ptr<Vec3f>(i)[j];
            Vec3b bgr = img.ptr<Vec3b>(i)[j];
            Vec3b ycrcb = imgYCRCB.ptr<Vec3b>(i)[j];
            //
            //            int Y = ycrcb.val[0];
            //            int Cr = ycrcb.val[1];
            //            int Cb = ycrcb.val[2];
            //            if ((135 < Cr <180) && (85 < Cb <135) && (Y > 80))
            //                b = true;
            //            else
            //                b = false;
            //            
            //            float H = hsv.val[0];
            //            float S = hsv.val[1];
            //            float V = hsv.val[2];
            //            if ((0 < H < 50) && (0.23 < S < 0.68))
            //                c = true;
            //            else
            //                c = false;
            //            
            int B = bgr.val[0];
            int G = bgr.val[1];
            int R = bgr.val[2];
            
            if ((R > 95) && (G > 40) && (B > 20) && ((max(R, max(G,B)) - min(R, min(G,B))) > 15) && (abs(R - G) > 15) && (R > G) && (R > B))
                rgb = true;
            else
                rgb=false;
            
            if(!rgb)
                outImg.ptr<Vec3b>(i)[j] = black;
            //            else
            //                img3.ptr<Vec3b>(i)[j] = white;
        }
    }
    return outImg;
}
