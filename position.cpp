#include "cv.h" 
#include "highgui.h"
#include "opencv2/imgproc/imgproc.hpp"
#include <cstdio> 
#include <iostream> 
#include <cstring> 
#include <utility> 
#include "morphOpen.h"
#include "mean.h"

using namespace cv;
using namespace std;

pair<Point,Point> positions(IplImage* src1,IplImage* src2,int index) {

	char fname[100];

	pair<Point,Point> means;

	IplImage* green_img1 = cvCreateImage( cvGetSize(src1), src1->depth, 1 ); //green layer for image 1
	IplImage* green_img2 = cvCreateImage( cvGetSize(src1), src1->depth, 1 ); //green layer for image 2
	IplImage* r = cvCreateImage( cvGetSize(src1), src1->depth, 1 ); //red layer -- no use
	IplImage* b = cvCreateImage( cvGetSize(src1), src1->depth, 1 ); //blue layer -- no use
	IplImage* abs_diff = cvCreateImage( cvGetSize(src1), src1->depth, 1 ); // absolute difference
	IplImage* abs_diff_thresh = cvCreateImage( cvGetSize(src1), src1->depth, 1 ); //thresholding of abs_diff_processed with the max value found
	IplImage* abs_diff_mopen = cvCreateImage( cvGetSize(src1), src1->depth, 1 ); //morphological opening of the thresholded image

	cvSplit(src1,r,green_img1,b,NULL);
	cvSplit(src2,r,green_img2,b,NULL);

	cvAbsDiff(green_img1,green_img2,abs_diff);
	
//	sprintf(fname,"diff/frame%d.jpg",index);
//	cvSaveImage(fname,abs_diff);
	
	cvThreshold( abs_diff, abs_diff_thresh, 30, 255, CV_THRESH_BINARY );
	abs_diff_mopen = mOpen(abs_diff_thresh);

//	sprintf(fname,"mean/frame%d.jpg",index);
//	cvSaveImage(fname,abs_diff_mopen);

	means = mean(abs_diff_mopen);
	
	return means;
}
