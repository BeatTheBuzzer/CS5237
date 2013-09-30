#include "trist.h"


/*
 * edge next: 0->1->2->0 3->5->4->3
*/
Trist::Trist()
{
	en_[0] = 1;
	en_[1] = 2;
	en_[2] = 0;
	en_[3] = 5;
	en_[4] = 3;
	en_[5] = 4;
}



/*
 * return the number of triangles
*/
int Trist::noTri()
{
	return (int)triVec.size();
}



/*
 *	Add a triangle into the Trist with the three point indices
 *	Moreover, automatically establish the fnext pointers to its neigbhours if autoMerge = true
*/
int Trist::makeTri(int pIndex1, int pIndex2, int pIndex3, bool autoMerge)
{
	TriRecord newRecord;

	newRecord.vi_[0] = pIndex1;
	newRecord.vi_[1] = pIndex2;
	newRecord.vi_[2] = pIndex3;
	triVec.push_back(newRecord);

	if (autoMerge) {
		//establish the fnext pointers to its neigbhours
	}

	return 1;
}



/*
 * Delete a triangle, but you can assume that this is ONLY used by the IP operation
 * You may want to make sure all its neighbours are detached (below)	
 */
void Trist::delTri(OrTri ot)
{
	int idx = ot >> 3;
	triVec.erase(triVec.begin() + idx);

	// detached all its neighbours
}



/*
 * return the three indices of the three vertices by OrTri
 */
void Trist::getVertexIdx(OrTri ot, int &pIdx1, int &pIdx2, int &pIdx3)
{
	int idx = ot >> 3;
	TriRecord tr = triVec[idx];
	
	pIdx1 = tr.vi_[0];
	pIdx2 = tr.vi_[1];
	pIdx3 = tr.vi_[2];
}



/*
 * the index of the first vertex of OrTri, e.g. org(bcd) => b
 */
int Trist::org(OrTri ot)
{
	int idx = ot >> 3;

	return triVec[idx].vi_[0];
}



/*
 * the index of the second vertex of OrTri, e.g. org(bcd) => c
 */
int Trist::dest(OrTri ot)
{
	int idx = ot >> 3;

	return triVec[idx].vi_[1];
}



