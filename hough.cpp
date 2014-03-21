/* 
   NOTE
   -> large image resolution may cause integer overflow during calculation
 */

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <algorithm>
#include <vector>
#include <utility>
#include <cmath>
#include <cstdlib>
#include "hough.h"

using namespace cv;
using namespace std;

vector<Vec4i> lines;

T::T()
{
	ind = num = den = 0;
}

T::T(int _ind,int _num,int _den)
{
	ind = _ind; 
	num = _num; 
	den = _den; 
}


int gcd(int a,int b)
{
	return (b == 0) ? a : gcd(b,a%b);
}

void set_point(Vec4i &l,int &x1,int &x2,int &y1,int &y2)
{
	x1 = l[0];
	y1 = l[1];
	x2 = l[2];
	y2 = l[3];
}

void get_num_den1(int a,int &num,int &den)
{
	int x1,x2,y1,y2;
	int w = WIDTH/2;

	set_point(lines[a],x1,x2,y1,y2);
	num = (y2 - y1)*w + y1*x2 - y2*x1;
	den = x2 - x1;

	int gcdd = gcd(num,den);
	num /= gcdd;
	den /= gcdd;
	
	//circle(cdst,Point(w,(int)(num/(float)den)),10,Scalar(0,0,255),-1,8,0);
}

void get_num_den2(int a,int &num,int &den)
{
	int x1,x2,y1,y2;
	int h = HEIGHT/2;

	set_point(lines[a],x1,x2,y1,y2);
	num = (x2 - x1)*h - y1*x2 + y2*x1;
	den = y2 - y1;
	
	int gcdd = gcd(num,den);
	num /= gcdd;
	den /= gcdd;
	//circle(cdst,Point((int)(num/(float)den),h),10,Scalar(0,0,255),-1,8,0);
}


bool cmp1(T a,T b)
{
	return 1LL*a.num*b.den < 1LL*b.num*a.den;
}

bool cmp2(T a,T b)
{
	return 1LL*a.num*b.den < 1LL*b.num*a.den;
}

int signum(int x)
{
	if(x == 0)
		return 0;
	return ((x > 0) ? 1 : -1);
}

float points2angle(Vec4i l)
{
	float ans;
	int x1,x2,y1,y2;

	set_point(l,x1,x2,y1,y2);
	if(x1 == x2)
		ans = signum(y2-y1)*INF;
	else
		ans = (y2-y1) / (x2-(float)x1);
	float ret = rintf(atan(ans)*(180/CV_PI));
	return ret;
}

void make_two_groups(vector<T> &g1, vector<T> &g2)
{
	size_t i,lim = lines.size();
	int num,den;

	for(i=1; i<lim; i++)
	{
		if(abs(points2angle(lines[i])) < 25)
		{
			get_num_den1(i,num,den);
			g1.push_back(T(i,num,den));
		}
		else
		{
			get_num_den2(i,num,den);
			g2.push_back(T(i,num,den));
		}
	}
	
	/*
	for(size_t i=0; i<g1.size(); i++)
		cout << points2angle(lines[g1[i].ind]) << endl;
	cout << endl;
	for(size_t i=0; i<g2.size(); i++)
		cout << points2angle(lines[g2[i].ind]) << endl;
	*/
}

float find_max_gap(vector<T> &g)
{
	float gap = 0;
	size_t lim = g.size();

	for(size_t i=1; i<lim; i++)
		gap = max(gap, g[i].num/(float)g[i].den - g[i-1].num/(float)g[i-1].den);
	
	return gap;
}

void remove_extra_lines(vector<T> &g,vector<T> &gr,float &gap)
{
	size_t prev = 0,lim = g.size();
	for(size_t i=0; i<lim; i++)
	{
		while( i<lim-1 && ((g[i+1].num/(float)g[i+1].den-g[i].num/(float)g[i].den) < gap) )
			i++;
		gr.push_back(g[(prev+i)/2]);
		prev = i+1;
	}
}

void draw_chess_lines(vector<T> &g, Mat &cdst)
{
	for(size_t i=0; i<g.size(); i++)
	{

		Vec4i l = lines[g[i].ind];
		line(cdst,Point(l[0],l[1]),Point(l[2],l[3]),Scalar(0,0,255),1,CV_AA);
		imshow("detected lines", cdst);
		waitKey();
	}
}

void getLines(IplImage *img, vector<Vec4i> &horiz, vector<Vec4i> &vert)
{
	Mat src(img);
	Mat dst, cdst, bdst;

	GaussianBlur(src,bdst,Size(5,5),0,0);
	Canny(bdst,dst,100,200,3);
	cvtColor(dst,cdst,CV_GRAY2BGR);
	HoughLinesP(dst, lines, 1, CV_PI/180, 80, 100, 800);

	vector<T> group1, group2, gr1, gr2;

	make_two_groups(group1,group2);
	
	sort(group1.begin(), group1.end(), cmp1);
	sort(group2.begin(), group2.end(), cmp2);
	
	float gap = find_max_gap(group1);
	gap *= 0.7;
	remove_extra_lines(group1,gr1,gap);
	
	gap = find_max_gap(group2);
	gap *= 0.7;
	remove_extra_lines(group2,gr2,gap);

	if(gr1.size()!=9 || gr2.size()!=9)
	{
		cout << "incorrect number of chess board lines" << endl;
		cout << gr1.size() << endl;
		cout << gr2.size() << endl;
		//exit(EXIT_FAILURE);
	}
	
//	draw_chess_lines(group1,cdst);
//	draw_chess_lines(group2,cdst);
	//draw_chess_lines(gr1,cdst);
	//draw_chess_lines(gr2,cdst);

	int lim = gr1.size();
	for(int i=0; i<lim; i++)
		horiz.push_back(lines[gr1[i].ind]);
	
	lim = gr2.size();
	for(int i=0; i<lim; i++)
		vert.push_back(lines[gr2[i].ind]);
}
