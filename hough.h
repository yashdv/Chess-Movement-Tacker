#ifndef __HOUGH
#define __HOUGH

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <algorithm>
#include <vector>
#include <utility>
#include <cmath>
#include <cstdlib>

using namespace cv;
using namespace std;

#define INF 1000000000

const int WIDTH = 1280;
const int HEIGHT = 800;

typedef struct T
{
	int ind,num,den;

	T();
	T(int,int,int);
}T;

void set_point(Vec4i &,int &,int &,int &,int &);
void get_num_den1(int,int &,int &);
void get_num_den2(int,int &,int &);
void make_two_groups(vector<T> &, vector<T> &);
float find_max_gap(vector<T> &);
void remove_extra_lines(vector<T> &,vector<T> &,float &);
void draw_chess_lines(vector<T> &);
void getLines(IplImage *,vector<Vec4i> &,vector<Vec4i> &);

#endif
