#include "cv.h" 
#include "highgui.h"
#include "opencv2/imgproc/imgproc.hpp"
#include <cstdio> 
#include <iostream> 
#include <cstring> 
#include <vector> 
#include <utility> 
#include "morphOpen.h"
#include "mean.h"

using namespace cv;
using namespace std;

IplImage* r = cvCreateImage(cvSize(820,820),IPL_DEPTH_8U,1);
IplImage* g = cvCreateImage(cvSize(820,820),IPL_DEPTH_8U,1);
IplImage* b = cvCreateImage(cvSize(820,820),IPL_DEPTH_8U,1);

void drawpiece(IplImage *board,CvPoint c,IplImage* h,int flag){
	CvScalar s,t;
	cvSplit(board,b,g,r,NULL);
	for(int i=c.x-38;i<c.x+38;i++){
		for(int j=c.y-18;j<c.y+18;j++){
			s = cvGet2D(h,i+38-c.x,j+18-c.y);
			if(flag == 1){
				cvSet2D(r,i,j,s);
			}	
			if(flag == 2){
				cvSet2D(g,i,j,s);
			}
		}
	}
	cvMerge(b,g,r,NULL,board);
}

void removepiece(IplImage *board,CvPoint c){
	CvScalar s;
	s.val[0] = 0;
	cvSplit(board,b,g,r,NULL);
	for(int i=c.x-38;i<c.x+38;i++){
		for(int j=c.y-18;j<c.y+18;j++){
			cvSet2D(b,i,j,s);
			cvSet2D(g,i,j,s);
			cvSet2D(r,i,j,s);
		}
	}
	cvMerge(b,g,r,NULL,board);
}

int main(){
	FILE *f;
	IplImage* board = cvCreateImage(cvSize(820,820),IPL_DEPTH_8U,3);
	IplImage *red[10],*green[10];
	char name[100];
	for(int i=0;i<9;i++){
		sprintf(name,"pieces/%d.png",i);
		red[i] = cvLoadImage(name,CV_LOAD_IMAGE_GRAYSCALE);
		green[i] = cvLoadImage(name,CV_LOAD_IMAGE_GRAYSCALE);
	}
	CvPoint start,end;
	vector< vector<char> > play(8,vector<char>(8));
	vector< vector<int> > piece(8,vector<int>(8,-1));
	start.x = 10;
	start.y = 10;
	end.x = 10;
	end.y = 810;
	for(int i=0;i<9;i++){
		cvLine(board,start,end,CV_RGB(255,255,255),2);
		start.x += 100;
		end.x += 100;
	}
	start.x = 10;
	start.y = 10;
	end.x = 810;
	end.y = 10;
	for(int i=0;i<9;i++){
		cvLine(board,start,end,CV_RGB(255,255,255),2);
		start.y += 100;
		end.y += 100;
	}

	vector< vector<CvPoint> > game(8,vector<CvPoint>(8));
	for(int i=0;i<8;i++){
		for(int j=0;j<8;j++){
			game[i][j].x = 60+i*100;
			game[i][j].y = 60+j*100;
			if(i == 0){
				drawpiece(board,game[i][j],red[j],1);
				play[i][j] = 'r';
				piece[i][j] = j;
			}
			else if(i == 1){
				drawpiece(board,game[i][j],red[8],1);
				play[i][j] = 'r';
				piece[i][j] = 8;
			}
			else if(i == 7){
				drawpiece(board,game[i][j],green[j],2);
				play[i][j] = 'g';
				piece[i][j] = j;
			}
			else if(i == 6){
				drawpiece(board,game[i][j],green[8],2);
				play[i][j] = 'g';
				piece[i][j] = 8;
			}
		}
	}

	int ox,oy,fx,fy,ret;
	cvNamedWindow("Live_Game!!",1);
	CvPoint temp;
	int index = 1;
	char fname[100];
	while(1){
		sprintf(fname,"moves/move%d.txt",index);
		f = fopen(fname,"r");
		if(f != NULL){
			if((ret = fscanf(f,"%d%d%d%d",&ox,&oy,&fx,&fy)) != EOF){
				if(ox == -1)
					break;
				removepiece(board,game[ox][oy]);
				removepiece(board,game[fx][fy]);
				if(play[ox][oy] == 'r'){
					drawpiece(board,game[fx][fy],red[piece[ox][oy]],1);
					play[fx][fy] = 'r';
					piece[fx][fy] = piece[ox][oy];
				}
				else if(play[ox][oy] == 'g'){
					drawpiece(board,game[fx][fy],green[piece[ox][oy]],2);
					play[fx][fy] = 'g';
					piece[fx][fy] = piece[ox][oy];
				}
				piece[ox][oy] = -1;
				index++;
			}
			fclose(f);
		}
		cvShowImage("Live_Game!!",board);
		cvWaitKey(10);
	}
	cvDestroyWindow("Live_Game!!");
	return 0;
}
