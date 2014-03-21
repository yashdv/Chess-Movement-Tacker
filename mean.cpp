#include "cv.h" 
#include "highgui.h"
#include "opencv2/imgproc/imgproc.hpp"
#include <cstdio> 
#include <iostream> 
#include <utility> 

using namespace cv;
using namespace std;

int dist(Point m,int i,int j){
	int dst = (m.x-i)*(m.x-i) + (m.y-j)*(m.y-j);
	return dst;
}

pair<Point,Point> mean(IplImage* src) {
	pair<Point,Point> ret;
	CvScalar pix;
	Point mean,mean1,mean2;	
	int i,j,xcord = 0,ycord = 0,cnt = 0;
	for (i = 0; i < src->height; i+=1){
		for (j = 0; j < src->width; j+=1){
			pix = cvGet2D(src,i,j);
			if(pix.val[0] == 255){
				xcord = xcord + i;
				ycord = ycord + j;
				cnt++;
			}
		}   
	}
	mean.x = xcord/cnt;
	mean.y = ycord/cnt;
	mean1.x = mean.x+4;
	mean1.y = mean.y+4;
	mean2.x = mean.x+8;
	mean2.y = mean.y+8;

	int m1x,m1y,m2x,m2y;
	int dist1,dist2,cnt1,cnt2;
	int itr_cnt = 0;
	while(1){
		itr_cnt++;
		m1x = 0;
		m1y = 0;
		m2x = 0;
		m2y = 0;
		dist1 = 0;
		dist2 = 0;
		cnt1 = 0;
		cnt2 = 0;
		for (i = 0; i < src->height; i+=1){
			for (j = 0; j < src->width; j+=1){
				pix = cvGet2D(src,i,j);
				if(pix.val[0] == 255){
					dist1 = dist(mean1,i,j);
					dist2 = dist(mean2,i,j);
					if(dist1 < dist2){
						m1x += i;
						m1y += j;
						cnt1++;
					}
					else{
						m2x += i;
						m2y += j;
						cnt2++;
					}
				}
			}   
		}
		m1x = m1x/cnt1;
		m1y = m1y/cnt1;
		m2x = m2x/cnt2;
		m2y = m2y/cnt2;
		if(m1x == mean1.x && m1y == mean1.y && m2x == mean2.x && m2y == mean2.y){
			break;
		}
		else{
			mean1.x = m1x;
			mean1.y = m1y;
			mean2.x = m2x;
			mean2.y = m2y;
		}
		if(itr_cnt > 10)
			break;
	}
	ret = make_pair(mean1,mean2);
	return ret;
}
