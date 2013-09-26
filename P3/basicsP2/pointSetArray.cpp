#include "pointSetArray.h"


int PointSetArray::getPoint (int pIndex, LongInt& x1,LongInt& y1){
	pIndex--;
	if (pIndex>=myPoints.size())
		return 0;
	x1=myPoints[pIndex].x;
	y1=myPoints[pIndex].y;
	return 1;
}

int PointSetArray::noPt(){
	return myPoints.size();
}

void PointSetArray::eraseAllPoints(){
	myPoints.clear();
}
