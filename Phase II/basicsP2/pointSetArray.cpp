#include "pointSetArray.h"

int PointSetArray::getPoint(int pIndex, LongInt &x1,LongInt &y1)
{
	return PointSet::getPoint(pIndex, x1, y1);
}

int PointSetArray::noPt()
{
	return PointSet::getSize();
}

void PointSetArray::eraseAllPoints()
{
	PointSet::clear();
}