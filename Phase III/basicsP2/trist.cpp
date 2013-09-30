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

bool Trist::getMap(FIndex f1, FIndex f2, OrTri &abc, OrTri &abd)
{
	int i, u, v, v1[3], v2[3];
	int idp1[2], idp2[2], idmap[22];

	f1 = f1 - 1;
	f2 = f2 - 1;

	for (i = 0; i < 3; i++) {
		v1[i] = myTris[f1].vi_[i];
		v2[i] = myTris[f2].vi_[i];
	}

	idp1[1] = idp2[1] = -1;
	idmap[1] = 0; idmap[12] = 1; idmap[20] = 2;
	idmap[10] = 3; idmap[21] = 4; idmap[2] = 5;

	for (i = 0; i < 2; i++) {
		for (u = 0; u < 3; u++) {
			for (v = 0; v < 3; v++) {
				if (v1[u] != -1 && v1[u] == v2[v]) {
					idp1[i] = u;
					idp2[i] = v;
					v1[u] = v2[v] = -1;
					break;
				}
			}
			if (v < 3) break;
		}
	}
	if (idp1[1] < 0) return false;

	abc = ((f1 + 1) << 3) | idmap[idp1[0] * 10 + idp1[1]];
	abd = ((f2 + 1) << 3) | idmap[idp2[0] * 10 + idp2[1]];
	return true;
}

int Trist::makeTri(int pIndex1,int pIndex2,int pIndex3,bool autoMerge){
	TriRecord tr1;
	OrTri abc, abd;

	int i, u, v;

	tr1.vi_[0]=pIndex1;
	tr1.vi_[1]=pIndex2;
	tr1.vi_[2]=pIndex3;
	for (i = 0; i < 6; i++) tr1.fnext_[i] = -1;

	myTris.push_back(tr1);
	triMap[_uo_tripple(pIndex1, pIndex2, pIndex3)] = myTris.size();
	if (autoMerge){
		// automatically establish the fnext pointers to its neigbhours
		v = myTris.size();
		for (u = 1; u < v; u++) {
			if (myTris[u-1].vi_[0] == -1) continue;
			if (getMap(u, v, abc, abd)) fmerge(abc, abd);
		}
	}
	return myTris.size();
}


FIndex Trist::getTriangleIndex(int a, int b, int c)
{
	_uo_tripple ut(a, b, c);
	if (triMap.find(ut) == triMap.end()) return -1;
	return triMap[ut];
}

OrTri Trist::FIndex2OriTri(FIndex f, int a, int b)
{
	int j, id[2], idm[22];

	f = f - 1;
	for (j = 0; j < 3; j++) {
		if (myTris[f].vi_[j] == a) {
			id[0] = j;
			break;
		}
	}
	if (j == 3) return -1;
	for (j = 0; j < 3; j++) {
		if (myTris[f].vi_[j] == b) {
			id[1] = j;
			break;
		}
	}
	if (j == 3) return -1;

	idm[1] = 0; idm[12] = 1; idm[20] = 2;
	idm[10] = 3; idm[21] = 4; idm[2] = 5;
	return ((f + 1) << 3) | idm[id[0] * 10 + id[1]];
}

void Trist::delTri(OrTri ef){
	int u, v, w;

	getVertexIdx(ef, u, v, w);
	fdetach(ef);
	myTris[(ef>>3)-1].vi_[0] = -1;
	triMap.erase(_uo_tripple(u, v, w));
}

void Trist::eraseAllTris(){
	myTris.clear();
	triMap.clear();
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
	FIndex fIdx = (ef >> 3)-1;
	return myTris[fIdx].fnext_[tVersion];
}

void Trist::getVertexIdx(OrTri ef, int& pIdx1,int& pIdx2,int& pIdx3){
	int tVersion = ef & 7;
	FIndex fIdx = (ef >> 3)-1;
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
	FIndex fIdx = (ef >> 3)-1;
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
	FIndex fIdx = (ef >> 3)-1;
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
	myTris[(abc >> 3) - 1].fnext_[abc & 7] = abd;
	myTris[(sym(abc) >> 3) - 1].fnext_[sym(abc) & 7] = sym(abd);
	myTris[(abd >> 3) - 1].fnext_[abd & 7] = abc;
	myTris[(sym(abd) >> 3) - 1].fnext_[sym(abd) & 7] = sym(abc);
}

void Trist::fdetach(OrTri abc){
	// detach triangle abc with all its neighbours (undo fmerge)
	OrTri abd;
	FIndex idx1;

	idx1 = (abc >> 3) - 1;
	for (int i = 0; i < 6; i++) {
		abd = myTris[idx1].fnext_[i];
		if (abd < 0) continue;
		myTris[(abd >> 3) - 1].fnext_[abd & 7] = -1;
		myTris[idx1].fnext_[i] = -1;
	}
}

void Trist::incidentTriangles(int ptIndex,int& noOrTri, OrTri* otList){
	// A suggested function: you may want this function to return all the OrTri
}

void Trist::flipping(int i, int j, int idx,int idx1,int idx2, int idx3){
	myTris[i-1].vi_[0] = idx;
	myTris[i-1].vi_[1] = idx1;
	myTris[i-1].vi_[2] = idx3;
	myTris[j-1].vi_[0] = idx;
	myTris[j-1].vi_[1] = idx2;
	myTris[j-1].vi_[2] = idx3;
}
