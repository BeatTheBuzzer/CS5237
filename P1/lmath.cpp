#include "lmath.h"


int signDet(LongInt x1, LongInt y1, LongInt w1,
	LongInt x2, LongInt y2, LongInt w2,
	LongInt x3, LongInt y3, LongInt w3){
		LongInt det = x1*y2*w3+x2*y3*w1+x3*y1*w2-x1*y3*w2-x2*y1*w3-x3*y2*w1;
		if(det>0) return 1;
		else if(det<0) return -1;
		else return 0;
}

int signDet(LongInt x[3][3]){
	LongInt det = x[0][0]*x[1][1]*x[2][2]+x[1][0]*x[2][1]*x[0][2]+x[2][0]*x[0][1]*x[1][2]-
		x[0][0]*x[2][1]*x[1][2]-x[1][0]*x[0][1]*x[2][2]-x[2][0]*x[1][1]*x[0][2];
	if(det>0) return 1;
	else if(det<0) return -1;
	else return 0;

}