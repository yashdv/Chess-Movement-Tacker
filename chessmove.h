#ifndef __CHESS_MOVE
#define __CHESS_MOVE

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

using namespace cv;
using namespace std;

const int refx = -1000;
const int refy = -1000;

bool same_side(Vec4i &, Point &);
int get_line(vector<Vec4i> &, Point &);
pair< pair<int,int>, pair<int,int> > get_move(vector<Vec4i> &,vector<Vec4i> &,Point &,Point &);

#endif
