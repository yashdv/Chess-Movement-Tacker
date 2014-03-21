#include "position.h"
#include "hough.h"
#include "chessmove.h"
#include "updatemove.h"
#include <cv.h>
#include <highgui.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include<cstdio>
#include<utility>

using namespace std;
using namespace cv;

inline static void allocateOnDemand( IplImage **img, CvSize size, int depth, int channels )
{
	if (*img != NULL)
		return;

	*img = cvCreateImage(size, depth, channels);
	if (*img == NULL)
	{
		fprintf(stderr, "Error: Couldn't allocate image.  Out of memory?\n");
		exit(-1);
	}
}
int main(int argc,char *argv[])
{
	int index = 10;
	int flag = 0;
	int diff = 2;
	int m = 0;
	int cnt = 0;
	int chance = 1;
	vector<Vec4i> horiz,vert;
	vector< vector<int> > board(8, vector<int>(8));
	make_board(board);

	CvCapture *capture = cvCaptureFromAVI(argv[1]);
//	CvCapture *capture = cvCaptureFromCAM(1);
//	CvCapture* capture = cvCreateCameraCapture( CV_CAP_ANY );
	if( !capture ) return 1;    
	
	int fps = ( int )cvGetCaptureProperty( capture, CV_CAP_PROP_FPS );
//	int fps = 24;
//	cvNamedWindow( "video", CV_WINDOW_AUTOSIZE);
	cvNamedWindow( "video", 0);
	int key = '#';
	int debug = 0;

	int frame_cnt = 0;
	IplImage *img_next1;
	IplImage *img_pres1;
	IplImage *key_prev;
	IplImage *key_curr;
	IplImage *temp_frame;
	IplImage *img_pres = NULL, *img_next = NULL, *sub1 = NULL, *sub2 = NULL, *add = NULL, *temp = NULL, *temp_thresh = NULL;

	int skip = 2;
	int hand = 0;
		
	pair<Point,Point> means;
	pair< pair<int,int>, pair<int,int> > cells;
//	IplImage *abc,*abc2;
//	while(key != 'c'){
//		abc = cvQueryFrame( capture );
//		abc2 = cvCloneImage(abc);
//		cvShowImage( "video", abc2);
//		key = cvWaitKey( 1000 / fps );
//	}

//	cout << "--------start-------" << endl;

	while(key != 'q' )
	{
		frame_cnt++;
		char pres[100],next[100];

		if(frame_cnt == 1)	
		{
			IplImage *curr = cvQueryFrame( capture );
			img_pres1 = cvCloneImage(curr);
		}
		
		cvShowImage( "video", img_pres1 );
		key = cvWaitKey( 1000 / fps );

		try
		{
			for(int i=0;i<diff;i++){
				img_next1 = cvQueryFrame( capture );
				if(!img_next1)
					throw 0;
				cvShowImage( "video", img_next1);
				key = cvWaitKey( 1000 / fps );
			}
		}
		catch(int e)
		{
			break;
		}


		allocateOnDemand(&img_pres,cvGetSize(img_pres1),IPL_DEPTH_8U,1);
		allocateOnDemand(&img_next,cvGetSize(img_next1),IPL_DEPTH_8U,1);
		cvCvtColor(img_pres1,img_pres,CV_RGB2GRAY);
		cvCvtColor(img_next1,img_next,CV_RGB2GRAY);
		
		int height,width,step,channels;
		uchar *data;

		allocateOnDemand(&sub1,cvGetSize(img_pres),IPL_DEPTH_8U,1);
		allocateOnDemand(&sub2,cvGetSize(img_pres),IPL_DEPTH_8U,1);
		allocateOnDemand(&add,cvGetSize(img_pres),IPL_DEPTH_8U,1);
		
//		cvSub(img_pres,img_next,sub1,NULL);
//		cvSub(img_next,img_pres,sub2,NULL);
//		cvAdd(sub1,sub2,add,NULL);

		cvAbsDiff(img_pres,img_next,add);
		
		allocateOnDemand(&temp,cvGetSize(add),add->depth,1);
		allocateOnDemand(&temp_thresh,cvGetSize(add),add->depth,1);

	//	int thresh_val = 70;
	//	int thresh_val = 100;
		int thresh_val = 80;

		cvThreshold(add,temp_thresh,thresh_val,255,CV_THRESH_BINARY);
		// get the image data
		height    = add->height;
		width     = add->width;
		step      = add->widthStep;
		channels  = add->nChannels;
		data      = (uchar *)add->imageData;

		char fname[100];
		int count = 0;
		for(int i=0;i<height;i++)
			for(int j=0;j<width;j++)
				for(int k=0;k<channels;k++)
					if(data[i*step+j*channels+k] > thresh_val)
						count++;

		if(count > 1)
		{
			if(flag == 0){		
				key_prev = cvCloneImage(img_pres1);
				cnt++;
				if(cnt == 1)
					getLines(key_prev,horiz,vert);
			}
			flag = 1;
		}
		else
		{
			if(flag == 1){
				key_curr = cvCloneImage(img_next1);
				means = positions(key_prev,key_curr,cnt);
				cells = get_move(horiz,vert,means.first,means.second);
				update_board(board,cells.first,cells.second,chance,cnt);
			}
			flag = 0;
		}

		if(flag == 1)
		{
			diff = 6;
			index += diff;
		}
		if(flag == 0)
		{
			diff = 2;
			index += diff;
		}
		cvCopy(img_next1, img_pres1);
	}
	char fname[100];
	sprintf(fname,"moves/move%d.txt",cnt+1);
	FILE *fp = fopen(fname,"w");
	fprintf(fp,"-1 -1 -1 -1");
	fclose(fp);
	system("sleep 2s");
	system("rm moves/*");
	return 0;
}
