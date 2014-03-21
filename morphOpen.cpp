#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>

using namespace cv;

/// Global variables

int morph_elem = 0;
int morph_size = 1;
int morph_operator = 0;
int const max_operator = 4;
int const max_elem = 2;
int const max_kernel_size = 21;

/** Function Headers */
Mat Morphology_Operations( int, void*, Mat);

/** @function main */
IplImage* mOpen(IplImage* img)
{
	Mat dst,final;
	Mat src(img);

	/// Default start
	dst = Morphology_Operations( 0, 0 ,src);
	dst.convertTo(final,CV_8U);
	IplImage temp = final;
	IplImage* opened;
	opened = cvCreateImage(cvSize(dst.cols,dst.rows), IPL_DEPTH_16S, 1);
	opened = cvCloneImage(&temp);

	return opened;
}

/**
 * @function Morphology_Operations
 */
Mat Morphology_Operations( int, void* ,Mat src)
{
	Mat dst;
	// Since MORPH_X : 2,3,4,5 and 6
	int operation = morph_operator + 2;

	Mat element = getStructuringElement( morph_elem, Size( 2*morph_size + 1, 2*morph_size+1 ), Point( morph_size, morph_size ) );

	/// Apply the specified morphology operation
	morphologyEx( src, dst, operation, element );
	
	return dst;
}
