#include "lmath.h"


int signDet(LongInt x1, LongInt y1, LongInt w1,
			LongInt x2, LongInt y2, LongInt w2,
			LongInt x3, LongInt y3, LongInt w3){
	LongInt a,b,c;
	a=x1*(y2*w3-y3*w2);
	b=y1*(x2*w3-x3*w2);
	c=w1*(x2*y3-x3*y2);
	return (a-b+c).sign();
}

int signDet(LongInt x[3][3]){
	LongInt a,b,c;
	a=x[0][0]*(x[1][1]*x[2][2]-x[2][1]*x[1][2]);
	b=x[0][1]*(x[1][0]*x[2][2]-x[2][0]*x[1][2]);
	c=x[0][2]*(x[1][0]*x[2][1]-x[2][0]*x[1][1]);
	return (a-b+c).sign();
}

LongInt Det(LongInt x1, LongInt y1, LongInt w1,
			LongInt x2, LongInt y2, LongInt w2,
			LongInt x3, LongInt y3, LongInt w3){
	LongInt a,b,c;
	a=x1*(y2*w3-y3*w2);
	b=y1*(x2*w3-x3*w2);
	c=w1*(x2*y3-x3*y2);
	return a-b+c;
}

LongInt Det(LongInt x[3][3]){
	LongInt a,b,c,t1,t2,t3;
	a=x[0][0]*(x[1][1]*x[2][2]-x[2][1]*x[1][2]);
	b=x[0][1]*(x[1][0]*x[2][2]-x[2][0]*x[1][2]);
	c=x[0][2]*(x[1][0]*x[2][1]-x[2][0]*x[1][1]);
	return a-b+c;
}