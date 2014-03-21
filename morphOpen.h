#ifndef __MORPH_OPEN
#define __MORPH_OPEN

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>

using namespace cv; 

Mat Morphology_Operations( int, void*, Mat);
IplImage* mOpen(IplImage*);


#endif
