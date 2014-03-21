#ifndef __UPDATEMOVE
#define __UPDATEMOVE


#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <utility>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <set>

using namespace cv;
using namespace std;

void make_board(vector< vector<int> > &);

void update_board(vector< vector<int> > &, pair<int,int> &, pair<int,int> &, int &, int &);

#endif
