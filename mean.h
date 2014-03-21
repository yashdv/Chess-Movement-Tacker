#ifndef __MEAN
#define __MEAN

#include "cv.h" 
#include "highgui.h"
#include "opencv2/imgproc/imgproc.hpp"
#include <cstdio> 
#include <iostream> 
#include <utility> 

using namespace cv; 
using namespace std;

int dist(Point m,int i,int j); 
pair<Point,Point> mean(IplImage* src); 

#endif
