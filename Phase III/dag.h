#ifndef _DAG_H
#define _DAG_H

#include "basicsP2\trist.h"
#include "basicsP2\pointSetArray.h"

#include <map>
#include <vector>
#include <algorithm>

struct quad_indices
{
	int a, b, c, d;
	quad_indices() {}
	quad_indices(int a, int b, int c, int d) {
		this->a = a;
		this->b = b;
		this->c = c;
		this->d = d;
	}
};

struct uo_tripple
{
	int a, b, c;
	uo_tripple() {}
	uo_tripple(int u, int v, int t)
	{
		a = u;
		b = v;
		c = t;
		arrange();
	}
	void arrange()
	{
		if (a > b) std::swap(a, b);
		if (a > c) std::swap(a, c);
		if (b > c) std::swap(b, c);
	}
};

bool operator < (const uo_tripple &A, const uo_tripple &B);

class DAG
{
public:
	void init(void *pointSet, void *trist, int a, int b, int c);
	quad_indices insert(int p);
	void flip(int eg_a, int eg_b, int p1, int p2);
private:
	int eg;
	std::vector<uo_tripple> node;
	std::vector<int> nextEdge;
	std::map<uo_tripple, int> table;

	PointSet *ps;
	Trist *myTrist;

	uo_tripple ancestor_node;
	quad_indices insert_node;

	void add_edge(uo_tripple A, uo_tripple B);
	bool insert_helper(uo_tripple N, int p);
	bool split4_helper(uo_tripple N, int p);

	bool isInEdge(int eg_a, int eg_b, int p);
	int getAnotherPoint(int a, int b, int c, int u, int v);
};


#endif
