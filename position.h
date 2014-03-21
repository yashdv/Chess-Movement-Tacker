#ifndef __POS
#define __POS

#include "cv.h" 
#include "highgui.h"
#include "opencv2/imgproc/imgproc.hpp"
#include <cstdio> 
#include <iostream> 
#include <utility> 
#include "morphOpen.h"
#include "mean.h"

using namespace cv;
using namespace std;

pair<Point,Point> positions(IplImage* src1,IplImage* src2,int index); 

#endif
