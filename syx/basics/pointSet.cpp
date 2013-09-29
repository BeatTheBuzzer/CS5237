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
	int i,j,k,l;
	LongInt x[4],y[4],mat1[4][4],mat2[3][3],temp[3][3],det1;
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

	for (i=0;i<4;i++){
		mat1[i][0]=x[i];
		mat1[i][1]=y[i];
		mat1[i][2]=x[i]*x[i]+y[i]*y[i];
		mat1[i][3]=1;
	}
	det1=0;
	for (i=0;i<4;i++){
		l=0;
		for (j=0;j<4;j++){
			if (i==j) continue;
			for (k=0;k<3;k++)
				temp[k][l]=mat1[k+1][j];
			l++;
		}
		if (i%2)
			det1=det1-mat1[0][i]*Det(temp);
		else det1=det1+mat1[0][i]*Det(temp);
	}
	for (i=0;i<3;i++){
		mat2[i][0]=x[i];
		mat2[i][1]=y[i];
		mat2[i][2]=1;
	}
	return det1.sign()*signDet(mat2);
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