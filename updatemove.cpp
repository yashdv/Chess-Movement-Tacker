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

void make_board(vector< vector<int> > &a)
{
	int lim2,lim1 = a.size();
	for(int i=0; i<lim1; i++)
	{
		lim2 = a[i].size();
		for(int j=0; j<lim2; j++)
			a[i][j] = 0;
	}

	for(int i=0; i<8; i++)
	{
		a[0][i] = a[1][i] = 1;
		a[6][i] = a[7][i] = 2;
	}
}

void update_board(vector< vector<int> > &a, pair<int,int> &m1, pair<int,int> &m2, int &chance, int &fno)
{
	int from_r,from_c,to_r,to_c;
	
	if(chance == 1)
	{
		if(a[m1.first][m1.second] == 1)
		{
			from_r = m1.first;
			from_c = m1.second;
			to_r = m2.first;
			to_c = m2.second;
		}
		else if(a[m2.first][m2.second] == 1)
		{
			from_r = m2.first;
			from_c = m2.second;
			to_r = m1.first;
			to_c = m1.second;
		}
	}
	else if(chance == 2)
	{
		if(a[m1.first][m1.second] == 2)
		{
			from_r = m1.first;
			from_c = m1.second;
			to_r = m2.first;
			to_c = m2.second;
		}
		else if(a[m2.first][m2.second] == 2)
		{
			from_r = m2.first;
			from_c = m2.second;
			to_r = m1.first;
			to_c = m1.second;
		}
	}
	a[to_r][to_c] = a[from_r][from_c];
	a[from_r][from_c] = 0;
	

	char fname[100];
	sprintf(fname,"moves/move%d.txt",fno);
	FILE *f = fopen(fname,"w");
	fprintf(f,"%d %d %d %d\n",from_r,from_c,to_r,to_c);
	fclose(f);
	chance = 3 - chance;
}
