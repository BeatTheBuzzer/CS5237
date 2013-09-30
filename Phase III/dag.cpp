#include "dag.h"

bool operator < (const uo_tripple &A, const uo_tripple &B)
{
	if (A.a < B.a) return true;
	if (A.a > B.a) return false;
	if (A.b < B.b) return true;
	if (A.b > B.b) return false;
	return A.c < B.c;
}

void DAG::init(void *pointSet, int a, int b, int c)
{
	ps = (PointSet *)pointSet;

	eg = 0;
	table.clear();
	node.clear();
	nextEdge.clear();
	
	ancestor_node = uo_tripple(a, b, c);
	table[ancestor_node] = -1;
}

void DAG::add_edge(uo_tripple A, uo_tripple B)
{
	nextEdge.push_back(table[A]);
	node.push_back(B);
	table[A] = eg++;
}

bool DAG::insert_helper(uo_tripple N, int p)
{
	int e;

	if (ps->inTri(N.a, N.b, N.c, p) <= 0) return false;
	if ((e = table[N]) == -1) {
		uo_tripple ta(N.a, N.b, p);
		uo_tripple tb(N.a, N.c, p);
		uo_tripple tc(N.b, N.c, p);
		table[ta] = table[tb] = table[tc] = -1;
		add_edge(N, ta);
		add_edge(N, tb);
		add_edge(N, tc);
		insert_node = N;
		return true;
	}

	while (e != -1) {
		if (insert_helper(node[e], p)) 
			return true;
		e = nextEdge[e];
	}

	return false;
}

uo_tripple DAG::insert(int p)
{
	if (insert_helper(ancestor_node, p)) 
		return insert_node;
	return uo_tripple(0, 0, 0);
}

void DAG::flip(int eg_a, int eg_b, int p1, int p2)
{
	uo_tripple ta(eg_a, eg_b, p1);
	uo_tripple tb(eg_a, eg_b, p2);
	uo_tripple da(eg_a, p1, p2);
	uo_tripple db(eg_b, p1, p2);

	add_edge(ta, da);
	add_edge(ta, db);
	add_edge(tb, da);
	add_edge(tb, db);
	table[da] = table[db] = -1;
}


