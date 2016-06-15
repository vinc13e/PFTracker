
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>
#include "tracker.h"
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

int getColorValue(const Mat sArea){
    cv::Mat dest;
    //blue -- WP_20160614_10_23_48_Pro.mp4
    //Scalar hsv_l(90,  150, 150);
    //Scalar hsv_h(110,255,255);

    //red -- WP_20160614_14_39_48_Pro.mp4
    Scalar hsv_l(10,  100, 100);
    Scalar hsv_h(179,  255,255);
    inRange(sArea,hsv_l,hsv_h,dest);
    return 1000*((double)countNonZero(dest) / sArea.total());
}

