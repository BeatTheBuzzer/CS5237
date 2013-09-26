#include "pointSet.h"

int PointSet::addPoint(LongInt x1, LongInt y1){
	struct MyPoint thisPoint;
	thisPoint.x = x1;
	thisPoint.y = y1;
	thisPoint.z = 0;

	myPoints.push_back(thisPoint);
	return (myPoints.size());
}
LongInt det3(LongInt x1, LongInt y1, LongInt w1,
	LongInt x2, LongInt y2, LongInt w2,
	LongInt x3, LongInt y3, LongInt w3){
	return x1*y2*w3+x2*y3*w1+x3*y1*w2-x1*y3*w2-x2*y1*w3-x3*y2*w1;
}

int signDet4(LongInt a11, LongInt a12, LongInt a13, LongInt a14,
	LongInt a21, LongInt a22, LongInt a23, LongInt a24,
	LongInt a31, LongInt a32, LongInt a33, LongInt a34,
	LongInt a41, LongInt a42, LongInt a43, LongInt a44){
	LongInt det = a11*det3(a22, a23, a24, a32, a33, a34, a42, a43, a44)-a21*det3(a12, a13,a14, a32, a33, a34, a42, a43,a44)+
		a31*det3(a12, a13, a14, a22, a23, a24, a42, a43, a44)-a41*det3(a12, a13, a14, a22, a23, a24, a32, a33, a34);
	if(det>0) return 1;
	else if(det<0) return -1;
	else return 0;
}
int PointSet::inCircle(int p1Idx, int p2Idx, int p3Idx, int pIdx) {
	LongInt xa=myPoints.at(p1Idx-1).x, ya=myPoints.at(p1Idx-1).y, za=xa*xa+ya*ya;
	LongInt xb=myPoints.at(p2Idx-1).x, yb=myPoints.at(p2Idx-1).y, zb=xb*xb+yb*yb;
	LongInt xc=myPoints.at(p3Idx-1).x, yc=myPoints.at(p3Idx-1).y, zc=xc*xc+yc*yc;
	LongInt xd=myPoints.at(pIdx-1).x, yd=myPoints.at(pIdx-1).y, zd=xd*xd+yd*yd;

	int det1 = signDet4(xa, ya, za, 1,
					xb, yb, zb, 1,
					xc, yc, zc, 1,
					xd, yd, zd, 1);
	int det2 = signDet(xa, ya, 1,
					   xb, yb, 1,
					   xc, yc, 1);
	int det=det1*det2;
	
	//if(det1 == 0 || det2 == 0) return 0;
	if(det>0)
		return 1;
	else if(det<0) return -1;
	else return 0;
}


int PointSet::inTri(int p1Idx, int p2Idx, int p3Idx, int pIdx) {
	LongInt temp[3][3],x[4],y[4];
	int i,j,k,det=10,now;
	p1Idx--;
	p2Idx--;
	p3Idx--;
	pIdx--;

	x[0]=myPoints[p1Idx].x;
	x[1]=myPoints[p2Idx].x;
	x[2]=myPoints[p3Idx].x;
	x[3]=myPoints[pIdx].x;

	y[0]=myPoints[p1Idx].y;
	y[1]=myPoints[p2Idx].y;
	y[2]=myPoints[p3Idx].y;
	y[3]=myPoints[pIdx].y;

	for (i=0;i<3;i++){
		for (j=0;j<3;j++){
			if (j==0) k=(i+1)%3;
			if (j==1) k=3;
			if (j==2) k=i;
			temp[j][0]=x[k];
			temp[j][1]=y[k];
			temp[j][2]=1;
		}
		now=signDet(temp);
		if (det==10)
			det=now;
		else if (now==0) return 0;
		else if (now!=det) return -1;
	}
	return 1;
}

int PointSet::inTri(int p1Idx, int p2Idx, int p3Idx, MyPoint pd){
	LongInt temp[3][3],x[4],y[4];
	int i,j,k,det=10,now;
	p1Idx--;
	p2Idx--;
	p3Idx--;

	x[0]=myPoints[p1Idx].x;
	x[1]=myPoints[p2Idx].x;
	x[2]=myPoints[p3Idx].x;
	x[3]=pd.x;

	y[0]=myPoints[p1Idx].y;
	y[1]=myPoints[p2Idx].y;
	y[2]=myPoints[p3Idx].y;
	y[3]=pd.y;

	for (i=0;i<3;i++){
		for (j=0;j<3;j++){
			if (j==0) k=(i+1)%3;
			if (j==1) k=3;
			if (j==2) k=i;
			temp[j][0]=x[k];
			temp[j][1]=y[k];
			temp[j][2]=1;
		}
		now=signDet(temp);
		if (det==10)
			det=now;
		else if (now==0) return 0;
		else if (now!=det) return -1;
	}
	return 1;
}