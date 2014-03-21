#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <utility>
#include <cmath>
#include <cstdlib>
#include <set>
#include "chessmove.h"

using namespace cv;
using namespace std;

int signumx(int x)
{
	if(x == 0)
		return 0;
	return ((x > 0) ? 1 : -1);
}

void set_pointx(Vec4i &l,int &x1,int &x2,int &y1,int &y2)
{
	x1 = l[0];
	y1 = l[1];
	x2 = l[2];
	y2 = l[3];
}

bool same_side(Vec4i &l, Point &p)
{
	int x1,x2,y1,y2,num,ref_sign,p_sign;
	set_pointx(l,x1,x2,y1,y2);

	num = (y2-y1)*refx + (y1*x2 - y2*x1) - (x2-x1)*refy;
	ref_sign = signumx(num);
	num = (y2-y1)*p.y + (y1*x2 - y2*x1) - (x2-x1)*p.x;
	p_sign = signumx(num);
	
	if(num == 0)
	{
		cout << "Side of Line : point on line\n";
	}
	return (ref_sign == p_sign);
}

int get_line(vector<Vec4i> &a, Point &m)
{
	int lim = a.size();
	for(int i=0; i<lim; i++)
		if(same_side(a[i],m))
			return max(0,i-1);
	return 7;
}

pair< pair<int,int>, pair<int,int> > get_move(vector<Vec4i> &horiz, vector<Vec4i> &vert, Point &m1, Point &m2)
{
	pair< pair<int,int>, pair<int,int> > ret;
	ret.first.first = get_line(horiz,m1);
	ret.first.second = get_line(vert,m1);
	
	ret.second.first = get_line(horiz,m2);
	ret.second.second = get_line(vert,m2);

	return ret;
}
