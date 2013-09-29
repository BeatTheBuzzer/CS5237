#include "trist.h"


Trist::Trist()
{
	en_[0] = 1;
	en_[1] = 2;
	en_[2] = 0;
	en_[3] = 5;
	en_[4] = 3;
	en_[5] = 4;

}

int Trist::noTri(){
	return myTris.size();
}

int Trist::makeTri(int pIndex1,int pIndex2,int pIndex3,bool autoMerge){
	TriRecord tr1;
	tr1.vi_[0]=pIndex1;
	tr1.vi_[1]=pIndex2;
	tr1.vi_[2]=pIndex3;
	int triIdx = myTris.size();
	myTris.push_back(tr1);
	if (autoMerge){
		// automatically establish the fnext pointers to its neigbhours
		for(int i=0;i<6;i++){
			OrTri tri = triIdx<<3|i;
			OrTri neighbour = neighboursTri(tri);
			if(neighbour != -1)
				fmerge(tri, neighbour);
		}
	}
	return myTris.size();
}

void Trist::delTri(OrTri ef){
	fdetach(ef);
	myTris.erase(myTris.begin()+(ef>>3));
}

void Trist::eraseAllTris(){
	myTris.clear();
}
		
OrTri Trist::enext(OrTri ef){
	int tVersion = ef & 7, tIdx = (ef >> 3);
	return (tIdx<<3)|en_[tVersion];
}

OrTri Trist::sym(OrTri ef){
	int tVersion = ef & 7, tIdx = (ef >> 3);
	return (tIdx<<3)|(tVersion+3)%6;
}

OrTri Trist::fnext(OrTri ef){
	int tVersion = ef & 7;
	FIndex fIdx = ef >> 3;
	return myTris[fIdx].fnext_[tVersion];
}

void Trist::getVertexIdx(OrTri ef, int& pIdx1,int& pIdx2,int& pIdx3){
	int tVersion = ef & 7;
	FIndex fIdx = ef >> 3;
	switch (tVersion){
	case 0:
		pIdx1=myTris[fIdx].vi_[0];
		pIdx2=myTris[fIdx].vi_[1];
		pIdx3=myTris[fIdx].vi_[2];
		break;
	case 1:
		pIdx1=myTris[fIdx].vi_[1];
		pIdx2=myTris[fIdx].vi_[2];
		pIdx3=myTris[fIdx].vi_[0];
		break;
	case 2:
		pIdx1=myTris[fIdx].vi_[2];
		pIdx2=myTris[fIdx].vi_[0];
		pIdx3=myTris[fIdx].vi_[1];
		break;
	case 3:
		pIdx1=myTris[fIdx].vi_[1];
		pIdx2=myTris[fIdx].vi_[0];
		pIdx3=myTris[fIdx].vi_[2];
		break;
	case 4:
		pIdx1=myTris[fIdx].vi_[2];
		pIdx2=myTris[fIdx].vi_[1];
		pIdx3=myTris[fIdx].vi_[0];
		break;
	case 5:
		pIdx1=myTris[fIdx].vi_[0];
		pIdx2=myTris[fIdx].vi_[2];
		pIdx3=myTris[fIdx].vi_[1];
		break;
	default:
		break;
	}
}

int Trist::org(OrTri ef){
	int tVersion = ef & 7, ret;
	FIndex fIdx = ef >> 3;
	switch (tVersion){
	case 0:
	case 5:
		ret=myTris[fIdx].vi_[0];
		break;
	case 1:
	case 3:
		ret=myTris[fIdx].vi_[1];
		break;
	case 2:
	case 4:
		ret=myTris[fIdx].vi_[2];
		break;
	}
	return ret;
}

int Trist::dest(OrTri ef){
	int tVersion = ef & 7, ret;
	FIndex fIdx = ef >> 3;
	switch (tVersion){
	case 2:
	case 3:
		ret=myTris[fIdx].vi_[0];
		break;
	case 0:
	case 4:
		ret=myTris[fIdx].vi_[1];
		break;
	case 1:
	case 5:
		ret=myTris[fIdx].vi_[2];
		break;
	}
	return ret;
}

void Trist::fmerge(OrTri abc, OrTri abd){
	// glue two neighbouring triangles, result abd = fnext(abc)
	int idx1 = abc>>3, v1 = abc & 7;
	int idx2 = abd>>3, v2 = abd & 7;
	myTris[idx1].fnext_[v1] = abd;
	myTris[idx2].fnext_[v2] = abc;
}
void Trist::fdetach(OrTri abc){
	// detach triangle abc with all its neighbours (undo fmerge)
	int idx = abc>>3;
	for(int i=0;i<6;i++){
		OrTri abd = myTris[idx].fnext_[i];
		if(abd>0){
			int abdIdx = abd>>3, abdVer = abd & 7;
			myTris[abdIdx].fnext_[abdVer] = -1;
		}
	}
}

void Trist::incidentTriangles(int ptIndex,int& noOrTri, OrTri* otList){
	// A suggested function: you may want this function to return all the OrTri
}

OrTri Trist::neighboursTri(OrTri abc){
	int a = org(abc), b = dest(abc);
	for(int i=0;i<myTris.size();i++){
		if( (i != abc>>3) && 
			(myTris[i].vi_[0] == a || myTris[i].vi_[1] == a || myTris[i].vi_[2] == a) &&
			(myTris[i].vi_[0] == b || myTris[i].vi_[1] == b || myTris[i].vi_[2] == b)){
				if(myTris[i].vi_[0] == a && myTris[i].vi_[1] == b)
					return i<<3|0;
				else if(myTris[i].vi_[1] == a && myTris[i].vi_[2] == b)
					return i<<3|1;
				else if(myTris[i].vi_[2] == a && myTris[i].vi_[0] == b)
					return i<<3|2;
				else if(myTris[i].vi_[1] == a && myTris[i].vi_[0] == b)
					return i<<3|3;
				else if(myTris[i].vi_[2] == a && myTris[i].vi_[1] == b)
					return i<<3|4;
				else if(myTris[i].vi_[0] == a && myTris[i].vi_[2] == b)
					return i<<3|5;
		}
	}
	return -1;
}