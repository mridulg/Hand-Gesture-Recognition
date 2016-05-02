
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
using namespace cv;

void displayImage(Mat , string);
void ASL_Detection(Mat );
double distPoints(Point , Point);

pair < Point, double > ThreePointsCircle(Point, Point, Point);

Mat GetSkin(Mat );
Mat spatialFilterGaussian(Mat );

double mask3(Mat , Mat , int , int);
Mat averageFilter3x3(Mat );
double mask5(Mat , Mat , int , int);
Mat averageFilter5x5(Mat );

int main(int argc, const char * argv[]) {
    
    Mat orgImg;
    // orgImg = imread("/Users/lifecodemohit/Documents/IIITD_Sem5/ImageAnalysis/IA_Project/Dataset/D/hand2_1_bot_seg_2_cropped.png", CV_LOAD_IMAGE_ANYCOLOR);
    orgImg = imread("/Users/mridul/Pictures/Screenshots/hand.png", CV_LOAD_IMAGE_ANYCOLOR);
    // orgImg = imread("/Users/lifecodemohit/Documents/IIITD_Sem5/ImageAnalysis/IA_Project/Dataset/D/hand2_3_top_seg_1_cropped.png", CV_LOAD_IMAGE_ANYCOLOR);
    // orgImg = imread("/Users/lifecodemohit/Documents/IIITD_Sem5/ImageAnalysis/IA_Project/Dataset/D/hand2_5_bot_seg_2_cropped.png", CV_LOAD_IMAGE_ANYCOLOR);
    
    displayImage(orgImg, "Original Image.");
    
    if(orgImg.empty()) {
        cout << "Image not loaded properly." << endl;
        return  0;
    }
    
    if (orgImg.type() != 0)
        orgImg = GetSkin(orgImg);
    
    // displayImage(orgImg, "Skin Segmentation Image.");
    
    
    for (int i = 0; i < orgImg.cols; i++) {
        orgImg.at<uchar>(orgImg.rows - 1, i) = 0;
        orgImg.at<uchar>(orgImg.rows - 2, i) = 0;
        orgImg.at<uchar>(orgImg.rows - 3, i) = 0;
        orgImg.at<uchar>(orgImg.rows - 4, i) = 0;
        orgImg.at<uchar>(orgImg.rows - 5, i) = 0;
    }
    
    erode(orgImg,orgImg,Mat());
    dilate(orgImg,orgImg,Mat());
    
    // orgImg = averageFilter3x3(orgImg);
    
    displayImage(orgImg, "Blurred Image.");
    
    ASL_Detection(orgImg);
    
    return 0;
    
}

void displayImage(Mat img, string text) {
    
    namedWindow(text, WINDOW_AUTOSIZE );
    imshow(text , img);
    waitKey(0);
    //    destroyWindow(text);
    
}

void ASL_Detection(Mat frame) {
    
    Mat canny_output;
    vector < vector < Point > > contours;
    vector<Vec4i> hierarchy;
    vector < pair < Point, double > > avg_center_of_palm;
    
    vector < Point > all_palm_point;
    vector < Point > st_point;
    vector < Point > en_point;
    vector < Point > fa_point;
    
    /// Find contours
    Canny(frame, canny_output, 100, 200, 3);
    findContours(canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
    
    /// Find the convex hull object for each contour
    vector < vector < int > > hullI(contours.size());
    vector < vector < Point > > hull(contours.size());
    vector < vector < Point > > defect_points(contours.size());
    
    Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3);
    
    int loc_max_defects = 0;
    
    for(int i = 0; i < (int)contours.size(); i++) {
        
        vector < Vec4i > defects;
        convexHull( Mat(contours[i]), hullI[i], false);
        convexHull( Mat(contours[i]), hull[i], false);
        
        //        RotatedRect rect = minAreaRect(Mat(contours[i]));
        //
        //        Point2f rect_points[4];
        //        rect.points(rect_points);
        //
        //        for(int j = 0; j < 4; j++)
        //            line(drawing, rect_points[j], rect_points[(j + 1) % 4], Scalar(255, 0, 0), 1, 8);
        
        convexityDefects(contours[i], hullI[i], defects);
        cout << "Size of Defects : " << defects.size() << endl;
        Point palm_center_all;
        
        vector < Point > palm_point;
        if ((int)defects.size() >= 3) {
            bool flag = false;
            if ((int)defects.size() > loc_max_defects) {
                loc_max_defects = (int)defects.size();
                flag = true;
                all_palm_point.clear();
                st_point.clear();
                en_point.clear();
                fa_point.clear();
            }
            for (int j = 0; j < (int)defects.size(); j++) {
                int index = defects[j][2];
                defect_points[i].push_back(contours[i][index]);
                circle(drawing, contours[i][index], 5, Scalar(243, 2, 243), -1);
                drawContours(drawing, contours, i, Scalar(243, 2, 58), 2, 8, vector<Vec4i>(), 0, Point());
                drawContours(drawing, hull, i, Scalar(9, 9, 227), 2, 8, vector<Vec4i>(), 0, Point());
                
                int start_index = defects[j][0];
                int end_index = defects[j][1];
                int far_index = defects[j][2];
                
                Point start_point(contours[i][start_index]);
                Point end_point(contours[i][end_index]);
                Point far_point(contours[i][far_index]);
                
                palm_center_all += start_point + end_point + far_point;
                palm_point.push_back(start_point);
                palm_point.push_back(end_point);
                palm_point.push_back(far_point);
                
                if (flag) {
                    
                    all_palm_point.push_back(start_point);
                    all_palm_point.push_back(end_point);
                    all_palm_point.push_back(far_point);
                    st_point.push_back(start_point);
                    en_point.push_back(end_point);
                    fa_point.push_back(far_point);
                    
                }
                
            }
            
            palm_center_all.x = (palm_center_all.x)/((int)defects.size()*3);
            palm_center_all.y = (palm_center_all.y)/((int)defects.size()*3);
            
            // cout << "Palm center all : (" << palm_center_all.x << ", " << palm_center_all.y << ")" << endl;
            
            vector < pair < double, int > > distance_vector;
            for (int j = 0; j < (int)palm_point.size(); j++) {
                distance_vector.push_back(make_pair(distPoints(palm_center_all, palm_point[j]), j));
            }
            
            sort(distance_vector.begin(), distance_vector.end());
            pair < Point, double > find_circle;
            
            for (int j = 0; j < (int)distance_vector.size() - 2; j++) {
                
                Point pt1 = palm_point[distance_vector[j+0].second];
                Point pt2 = palm_point[distance_vector[j+1].second];
                Point pt3 = palm_point[distance_vector[j+2].second];
                
                find_circle = ThreePointsCircle(pt1, pt2, pt3);
                
                // cout << pt1.x << " " << pt1.y << endl;
                // cout << pt2.x << " " << pt2.y << endl;
                // cout << pt3.x << " " << pt3.y << endl;
                // cout << endl;
                
                if(find_circle.second != 0)
                    break;
            }
            
            // cout << "Yo " << find_circle.second  << endl;
            
            avg_center_of_palm.push_back(find_circle);
            
            Point center_of_palm = find_circle.first;
            //            double radius = find_circle.second;
            //            if (radius > 0 && radius < 300) {
            
            //                circle(drawing, center_of_palm, 5, Scalar(144,144,255), 3);
            
            //                // cout << radius << endl;
            
            //                circle(drawing, center_of_palm, radius, Scalar(144,144,255), 3);
            
            //            }
            
        }
        
        
        
    }
    
    
    
    Point all_palm_center_all;
    
    for (int j = 0; j < (int)all_palm_point.size(); j++) {
        
        all_palm_center_all.x += all_palm_point[j].x;
        all_palm_center_all.y += all_palm_point[j].y;
    }
    
    all_palm_center_all.x = all_palm_center_all.x/(int)(all_palm_point.size());
    all_palm_center_all.y = all_palm_center_all.y/(int)(all_palm_point.size());
    
    cout << "Palm center average : (" << all_palm_center_all.x << ", " << all_palm_center_all.y << ")" << endl;
    
    vector < pair < double, int > > all_distance_vector;
    
    for (int j = 0; j < (int)all_palm_point.size(); j++) {
        all_distance_vector.push_back(make_pair(distPoints(all_palm_center_all, all_palm_point[j]), j));
    }
    
    sort(all_distance_vector.begin(), all_distance_vector.end());
    
    pair < Point, double > all_find_circle;
    vector < pair < Point, double > > all_circle;
    
    // cout << all_distance_vector.size() << endl;
    
    //    for (int j = 0; j < (int)all_distance_vector.size(); j++) {
    
    //        for (int k = j+1; k < (int)all_distance_vector.size(); k++) {
    
    //            for (int l = k+1; l < (int)all_distance_vector.size(); l++) {
    
    //                Point pt1 = all_palm_point[all_distance_vector[j].second];
    
    //                Point pt2 = all_palm_point[all_distance_vector[k].second];
    
    //                Point pt3 = all_palm_point[all_distance_vector[l].second];
    
    //                all_find_circle = ThreePointsCircle(pt1, pt2, pt3);
    
    //                all_circle.push_back(all_find_circle);
    
    //            }
    
    //        }
    
    //    }
    
    
    
    for (int j = 0; j < (int)all_distance_vector.size() - 2; j++) {
        
        Point pt1 = all_palm_point[all_distance_vector[j + 0].second];
        Point pt2 = all_palm_point[all_distance_vector[j + 1].second];
        Point pt3 = all_palm_point[all_distance_vector[j + 2].second];
        
        all_find_circle = ThreePointsCircle(pt1, pt2, pt3);
        //        all_circle.push_back(all_find_circle);
        
        if(all_find_circle.second != 0)
            break;
    }
    
    //    cout << all_circle.size() << " " << frame.rows << " " << frame.cols << endl;
    
    Point center_of_palm = all_find_circle.first;
    double radius = all_find_circle.second;
    
    // sort(all_circle.begin(), all_circle.end());
    //    int MAXC = 10000000;
    
    //    int point_max = -1;
    
    //
    
    //    for (int j = 0; j < (int)all_circle.size(); j++) {
    
    //        int count = 0;
    
    //        for (int k = 0; k < frame.rows; k++) {
    
    //            for (int l = 0; l < frame.cols; l++) {
    
    //                if ((frame.at<float>(k, l) > 200) && (distPoints(all_circle[j].first, Point(l, k))) <= all_circle[j].second) {
    
    //                    count++;
    
    //                }
    
    //            }
    
    //        }
    
    //        if (count > MAXC) {
    
    //            MAXC = count;
    
    //            point_max = j;
    
    //        }
    
    //        // cout << j << " " << all_circle.size() << endl;
    
    //    }
    
    //
    
    //
    
    //    int mid_all_circle = point_max;
    
    
    
    //    Point all_center_of_palm = all_circle[mid_all_circle].first;
    
    //    double all_radius = all_circle[mid_all_circle].second;
    
    //
    
    //     // circle(drawing, center_of_palm, 5, Scalar(144,144,255), 3);
    
    //     circle(drawing, all_center_of_palm, all_radius, Scalar(144,144,255), 3);
    
    
    
    cout << "Radius : " << radius << endl;
    
    circle(drawing, center_of_palm, 5, Scalar(34, 245, 10), 3);
    circle(drawing, center_of_palm, radius, Scalar(34, 245, 10), 3);
    
    int finger_count = 0;
    for (int j = 0; j < (int)st_point.size(); j++) {
        
        double xlength = sqrt(distPoints(st_point[j], center_of_palm));
        double ylength = sqrt(distPoints(fa_point[j], center_of_palm));
        double hlength = sqrt(distPoints(fa_point[j], st_point[j]));
        double zlength = sqrt(distPoints(en_point[j], fa_point[j]));
        
        // Reference for selecting center of palm:  http://s-ln.in/2013/04/18/hand-tracking-and-gesture-detection-opencv/
        
        if((hlength <= 3*radius) && (ylength >= 0.4*radius) && (hlength >= 10) && (zlength >= 10) && max(hlength,zlength)/min(hlength,zlength) >= 0.8)
            if(min(xlength, ylength)/max(xlength,ylength) <= 0.8) {
                
                if(((xlength >= 0.1*radius) && (xlength <= 1.3*radius) && (xlength < ylength))||((ylength >= 0.1*radius) && (ylength <= 1.3*radius) && (xlength > ylength)))
                    line(drawing, en_point[j], fa_point[j], Scalar(0,255,0), 1);
                
                finger_count++;
            }
    }
    
    cout << "Finger count : " << min(5, finger_count + 1) << endl;
    
    displayImage(drawing, "Contour + Hull Image.");
    
}



double distPoints(Point x,Point y) {
    return ((x.x - y.x) * (x.x - y.x)) + ((x.y - y.y) * (x.y - y.y));
}



// Reference : http://paulbourke.net/geometry/circlesphere/

pair < Point, double > ThreePointsCircle(Point pt1, Point pt2, Point pt3) {
    
    int ma_num = pt2.y - pt1.y;
    int ma_den = pt2.x - pt1.x;
    
    int mb_num = pt3.y - pt2.y;
    int mb_den = pt3.x - pt2.x;
    
    double eps = 0.00000001;
    
    if (abs((mb_num * ma_den) - (mb_den * ma_num)) < eps)
        return make_pair(Point(-1, -1), 0);
    
    double x = ((ma_num*mb_num*(pt1.y - pt3.y)) + (mb_num*ma_den*(pt1.x + pt2.x)) - (ma_num*mb_den*(pt2.x + pt3.x)))/(2*(mb_num*ma_den - ma_num*mb_den));
    double y = ((pt1.y + pt2.y)/2) -((ma_den*(x - (pt1.x + pt2.x)/2))/ma_num);
    double rad = hypot(pt1.x - x, pt1.y - y);
    
    return make_pair(Point(x, y), rad);
}

Mat GetSkin(Mat img) {
    
    Mat outImg = img.clone();
    Mat imgYCRCB, imgHSV;
    
    bool rgb;
    Vec3b black = Vec3b::all(0);
    
    //    bool b,c ;
    Vec3b white = Vec3b::all(255);
    
    img.convertTo(imgHSV, CV_32FC3);
    cvtColor(imgHSV,imgHSV, CV_BGR2HSV);
    normalize(imgHSV,imgHSV, 0.0, 255.0, NORM_MINMAX, CV_32FC3);
    cvtColor(img, imgYCRCB, CV_BGR2YCrCb);
    
    
    for(int i = 0; i < img.rows; i++) {
        
        for(int j = 0; j < img.cols; j++) {
            
            Vec3f hsv = imgHSV.ptr<Vec3f>(i)[j];
            Vec3b bgr = img.ptr<Vec3b>(i)[j];
            Vec3b ycrcb = imgYCRCB.ptr<Vec3b>(i)[j];
            
            int B = bgr.val[0];
            int G = bgr.val[1];
            int R = bgr.val[2];
            
            if ((R > 95) && (G > 40) && (B > 20) && ((max(R, max(G,B)) - min(R, min(G,B))) > 15) && (abs(R - G) > 15) && (R > G) && (R > B))
                rgb = true;
            else
                rgb=false;
            
            if(!rgb)
                outImg.ptr<Vec3b>(i)[j] = black;
            else
                outImg.ptr<Vec3b>(i)[j] = white;
        }
    }
    return outImg;
}


Mat spatialFilterGaussian(Mat img) {
    
    Mat G = Mat::zeros(img.rows + 6, img.cols + 6, CV_8UC1);
    Mat F = Mat::zeros(img.rows, img.cols, CV_8UC1);
    
    for (int i = 0; i < img.rows; i++) {
        for (int j = 0; j < img.cols; j++) {
            G.at<uchar>(i+3, j+3) = img.at<uchar>(i, j);
        }
    }
    
    int filter[][7] = {{1, 1, 2, 2, 2, 1, 1}, {1, 2, 2, 4, 2, 2, 1}, {2, 2, 4, 8, 4, 2, 2}, {2, 4, 8, 16, 8, 4, 2}, {2, 2, 4, 8, 4, 2, 2}, {1, 2, 2, 4, 2, 2, 1}, {1, 1, 2, 2, 2, 1, 1}};
    
    for (int i = 3; i < img.rows + 3; i++) {
        for (int j = 3; j < img.cols + 3; j++) {
            double res = 0.0, sum = 0.0;
            for (int i1 = 0; i1 < 7; i1++) {
                for (int j1 = 0; j1 < 7; j1++) {
                    res += (G.at<uchar>(i - 3 + i1, j - 3 + j1)*filter[i1][j1]);
                    sum += filter[i1][j1];
                }
            }
            res = res/sum;
            F.at<uchar>(i - 3, j - 3) = (255 < res ? 255 : res);
        }
    }
    
    return F;
}

double mask3(Mat filter, Mat img, int x, int y) {
    
    double res = 0.0;
    for (int i = 0; i < filter.rows; i++) {
        for (int j = 0; j < filter.cols; j++) {
            res = res + ((int)filter.at<uchar>(i, j)*(int)img.at<uchar>(y - 1 + i, x - 1 + j)) / 9.0;
        }
    }
    return res;
}

double mask5(Mat filter, Mat img, int x, int y) {
    
    double res = 0.0;
    for (int i = 0; i < filter.rows; i++) {
        for (int j = 0; j < filter.cols; j++) {
            res = res + ((int)filter.at<uchar>(i, j)*(int)img.at<uchar>(y - 2 + i, x - 2 + j)) / 25.0;
        }
    }
    return res;
}

Mat averageFilter3x3(Mat img) {
    
    Mat tmpImg = Mat(img.rows + 2, img.cols + 2, CV_8UC1);  // Zero padding
    Mat finalImg = Mat(img.rows, img.cols, CV_8UC1);
    
    for (int y = 0; y < tmpImg.rows; y++)
        for (int x = 0; x < tmpImg.cols; x++)
            tmpImg.at<uchar>(y, x) = 0;
    
    for (int y = 1; y <= img.rows; y++)
        for (int x = 1; x <= img.cols; x++) {
            tmpImg.at<uchar>(y, x) = img.at<uchar>(y-1, x-1);
        }
    
    Mat avFilter3X3 = Mat::ones(3, 3, CV_8UC1);
    
    for (int y = 1; y < tmpImg.rows - 1; y++) {
        for (int x = 1; x < tmpImg.cols - 1; x++) {
            finalImg.at<uchar>(y - 1, x - 1) = mask3(avFilter3X3, tmpImg, x, y);
        }
    }
    
    return finalImg;
}

Mat averageFilter5x5(Mat img) {
    
    Mat tmpImg = Mat(img.rows + 4, img.cols + 4, CV_8UC1);  // Zero padding
    Mat finalImg = Mat(img.rows, img.cols, CV_8UC1);
    
    for (int y = 0; y < tmpImg.rows; y++)
        for (int x = 0; x < tmpImg.cols; x++)
            tmpImg.at<uchar>(y, x) = 0;
    
    for (int y = 2; y <= img.rows + 1; y++)
        for (int x = 2; x <= img.cols + 1; x++) {
            tmpImg.at<uchar>(y, x) = img.at<uchar>(y-2, x-2);
        }
    
    Mat avFilter5X5 = Mat::ones(5, 5, CV_8UC1);
    
    for (int y = 2; y < tmpImg.rows - 2; y++) {
        for (int x = 2; x < tmpImg.cols - 2; x++) {
            finalImg.at<uchar>(y - 2, x - 2) = mask5(avFilter5X5, tmpImg, x, y);
        }
    }
    
    return finalImg;
}