#ifndef TRISTH
#define TRISTH

#include "../hashtable.h"

#include <vector>
#include <map>
#include <algorithm>

/*

  For a triangle abc, if version 0 is abc

  version 0 abc     (v:012)
  version 1 bca		(v:120)
  version 2 cab		(v:201)
  version 3 bac		(v:102)
  version 4 cba		(v:210)
  version 5 acb		(v:021)

  enext cycles   0 > 1 > 2 > 0 ,  5 > 4 > 3 > 5
  sym cycles  i <> i + 3 % 6

  org  = ver < 3 ? v[ver] : v[(ver+1)%3]
  dest = ver < 3 ? v[(ver+1)%3] : v[ver-3] 

*/

typedef  int OrTri;  // The OrTri data structure for a triangle
typedef  int FIndex; // The index of a triangle Hint: NOT a triangle if it's negative
                     // You should be able to make all the triangle indices to be from 0 to n - 1 (n = number of triangles)
class Trist;

class TriRecord {
	protected:
		int vi_[3];
		OrTri fnext_[6];
	friend Trist;
};

class Trist {
protected:
	int en_[6];
	std::vector<TriRecord> myTris;
	hashtable_bi hash_edge2indices;
	hashtable_tri hash_triangle2index;
public:
	Trist();
	int noTri();
	int makeTri(int pIndex1, int pIndex2, int pIndex3, bool autoMerge = false);
	void delTri(OrTri);
	void eraseAllTris();
	OrTri enext(OrTri ef);
	OrTri sym(OrTri ef);
	OrTri fnext(OrTri ef);
	void getVertexIdx(OrTri, int& pIdx1, int& pIdx2, int& pIdx3);
	int org(OrTri);
	int dest(OrTri);
	void fmerge(OrTri abc, OrTri abd);
	void fdetach(OrTri abc);
	FIndex getTriangleIndex(int a, int b, int c);
	OrTri FIndex2OrTri(FIndex f, int a, int b);
	std::pair<int, int> fromEdge2Indices(int eg_a, int eg_b);
private:
	void fmerge_helper(int ea, int eb);
};



#endif
