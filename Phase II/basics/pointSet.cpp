#include "pointSet.h"

int PointSet::addPoint(LongInt x1, LongInt y1){
	struct MyPoint thisPoint;
	thisPoint.x = x1;
	thisPoint.y = y1;
	thisPoint.z = 0;

	myPoints.push_back(thisPoint);
	return (myPoints.size());
}

int PointSet::inCircle(int p1Idx, int p2Idx, int p3Idx, int pIdx) {
	int sign;
	LongInt x1, y1, x2, y2, x3, y3, x, y;
	x = myPoints[pIdx].x; y = myPoints[pIdx].y;
	x1 = myPoints[p1Idx].x; y1 = myPoints[p1Idx].y;
	x2 = myPoints[p2Idx].x; y2 = myPoints[p2Idx].y;
	x3 = myPoints[p3Idx].x; y3 = myPoints[p3Idx].y;
	sign = signDet(
		x1 - x, y1 - y, (x1 - x) * (x1 - x) + (y1 - y) * (y1 - y), 
		x2 - x, y2 - y, (x2 - x) * (x2 - x) + (y2 - y) * (y2 - y), 
		x3 - x, y3 - y, (x3 - x) * (x3 - x) + (y3 - y) * (y3 - y)
	);
	sign *= signDet(
		x1, y1, 1, 
		x2, y2, 1, 
		x3, y3, 1
	);

	return sign;
}


int PointSet::inTri(int p1Idx, int p2Idx, int p3Idx, int pIdx) {
	int s1, s2, s3;
	struct MyPoint v1, v2;
	
	v1 = myPoints[p2Idx] - myPoints[p1Idx];
	v2 = myPoints[pIdx] - myPoints[p1Idx];
	s1 = signDet(1, 0, 0, 0, v1.x, v1.y, 0, v2.x, v2.y);

	v1 = myPoints[p3Idx] - myPoints[p2Idx];
	v2 = myPoints[pIdx] - myPoints[p2Idx];
	s2 = signDet(1, 0, 0, 0, v1.x, v1.y, 0, v2.x, v2.y);

	v1 = myPoints[p1Idx] - myPoints[p3Idx];
	v2 = myPoints[pIdx] - myPoints[p3Idx];
	s3 = signDet(1, 0, 0, 0, v1.x, v1.y, 0, v2.x, v2.y);

	if (!s1 || !s2 || !s3) return 0;
	if (s1 == s2 && s2 == s3) return 1;
	return -1;
}


int PointSet::getPoint(int idx, LongInt &x, LongInt &y)
{
	if (idx >= (int)myPoints.size() || idx < 0) {
		return 0;
	}
	x = myPoints[idx].x; 
	y = myPoints[idx].y;
	return 1;
}

int PointSet::getSize() const
{
	return myPoints.size();
}

void PointSet::clear()
{
	myPoints.clear();
}