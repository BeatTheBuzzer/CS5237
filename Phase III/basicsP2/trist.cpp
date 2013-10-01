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

int Trist::noTri()
{
	return myTris.size();
}

void Trist::fmerge_helper(int ea, int eb)
{
	OrTri abc, abd;
	std::pair<int, int> idx_pair;

	idx_pair = hash_edge2indices.getIdx(ea, eb);
	if (idx_pair.second == -1) return;
	abc = FIndex2OrTri(idx_pair.first, ea, eb);
	abd = FIndex2OrTri(idx_pair.second, ea, eb);
	fmerge(abc, abd);
}

int Trist::makeTri(int pIndex1, int pIndex2, int pIndex3, bool autoMerge)
{
	int i, idx;
	TriRecord tr1;
	
	tr1.vi_[0]=pIndex1;
	tr1.vi_[1]=pIndex2;
	tr1.vi_[2]=pIndex3;
	for (i = 0; i < 6; i++) tr1.fnext_[i] = -1;

	myTris.push_back(tr1);
	idx = myTris.size();
	hash_triangle2index.insert(pIndex1, pIndex2, pIndex3, idx);
	hash_edge2indices.insert(pIndex1, pIndex2, idx);
	hash_edge2indices.insert(pIndex1, pIndex3, idx);
	hash_edge2indices.insert(pIndex2, pIndex3, idx);
	if (autoMerge){
		fmerge_helper(pIndex1, pIndex2);
		fmerge_helper(pIndex1, pIndex3);
		fmerge_helper(pIndex2, pIndex3);
	}
	return myTris.size();
}


FIndex Trist::getTriangleIndex(int a, int b, int c)
{
	return hash_triangle2index.getIdx(a, b, c);
}

OrTri Trist::FIndex2OrTri(FIndex f, int a, int b)
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

/*
	In order to prevent indices from changing, 
	we only set vi_[0] = -1 for the related 
	triangle.
*/
void Trist::delTri(OrTri ef)
{
	int u, v, w, idx;

	getVertexIdx(ef, u, v, w);
	fdetach(ef);
	myTris[(ef>>3)-1].vi_[0] = -1;
	idx = hash_triangle2index.getIdx(u, v, w);
	hash_triangle2index.remove(u, v, w);
	hash_edge2indices.remove(u, v, idx);
	hash_edge2indices.remove(u, w, idx);
	hash_edge2indices.remove(v, w, idx);
}

void Trist::eraseAllTris()
{
	myTris.clear();
	hash_edge2indices.init();
	hash_triangle2index.init();
}
		
OrTri Trist::enext(OrTri ef)
{
	int tVersion = ef & 7, tIdx = (ef >> 3);
	return (tIdx<<3)|en_[tVersion];
}

OrTri Trist::sym(OrTri ef)
{
	int tVersion = ef & 7, tIdx = (ef >> 3);
	return (tIdx<<3)|(tVersion+3)%6;
}

OrTri Trist::fnext(OrTri ef)
{
	int tVersion = ef & 7;
	FIndex fIdx = (ef >> 3)-1;
	return myTris[fIdx].fnext_[tVersion];
}

void Trist::getVertexIdx(OrTri ef, int& pIdx1, int& pIdx2, int& pIdx3)
{
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

int Trist::org(OrTri ef)
{
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

int Trist::dest(OrTri ef)
{
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

void Trist::fmerge(OrTri abc, OrTri abd)
{
	myTris[(abc >> 3) - 1].fnext_[abc & 7] = abd;
	myTris[(sym(abc) >> 3) - 1].fnext_[sym(abc) & 7] = sym(abd);
	myTris[(abd >> 3) - 1].fnext_[abd & 7] = abc;
	myTris[(sym(abd) >> 3) - 1].fnext_[sym(abd) & 7] = sym(abc);
}

void Trist::fdetach(OrTri abc)
{
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

std::pair<int, int> Trist::fromEdge2Indices(int eg_a, int eg_b)
{
	return hash_edge2indices.getIdx(eg_a, eg_b);
}