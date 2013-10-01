#include "dag.h"

bool operator < (const uo_tripple &A, const uo_tripple &B)
{
	if (A.a < B.a) return true;
	if (A.a > B.a) return false;
	if (A.b < B.b) return true;
	if (A.b > B.b) return false;
	return A.c < B.c;
}

void DAG::init(void *pointSet, void *trist, int a, int b, int c)
{
	ps = (PointSet *)pointSet;
	myTrist = (Trist *)trist;

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

bool DAG::split4_helper(uo_tripple N, int p)
{
	int eg_a, eg_b;
	int a, b, c, u, v;
	std::pair<int, int> indices;
	uo_tripple M, ta, tb, tc, td;

	if (isInEdge(N.a, N.b, p)) eg_a = N.a, eg_b = N.b, u = N.c;
	else if (isInEdge(N.a, N.c, p)) eg_a = N.a, eg_b = N.c, u = N.b;
	else eg_a = N.b, eg_b = N.c, u = N.a;

	indices = myTrist->fromEdge2Indices(eg_a, eg_b);
	if (indices.second == -1) return false;
	myTrist->getVertexIdx(indices.first << 3, a, b, c);
	v = getAnotherPoint(a, b, c, eg_a, eg_b);
	if (v == u) {
		myTrist->getVertexIdx(indices.second << 3, a, b, c);
		v = getAnotherPoint(a, b, c, eg_a, eg_b);
	}
	M = uo_tripple(eg_a, eg_b, v);
	ta = uo_tripple(eg_a, p, u);
	tb = uo_tripple(eg_b, p, u);
	tc = uo_tripple(eg_a, p, v);
	td = uo_tripple(eg_b, p, v);
	table[ta] = table[tb] = table[tc] = table[td] = -1;
	add_edge(N, ta);
	add_edge(N, tb);
	add_edge(M, tc);
	add_edge(M, td);
	insert_node = quad_indices(eg_a, eg_b, u, v);

	return true;
}

bool DAG::insert_helper(uo_tripple N, int p)
{
	int e, inTriVal;
	uo_tripple ta, tb, tc;

	inTriVal = ps->inTri(N.a, N.b, N.c, p);
	if (inTriVal < 0) return false;
	if ((e = table[N]) == -1) {
		if (inTriVal == 0) {
			return split4_helper(N, p);
		}
		ta = uo_tripple(N.a, N.b, p);
		tb = uo_tripple(N.a, N.c, p);
		tc = uo_tripple(N.b, N.c, p);
		table[ta] = table[tb] = table[tc] = -1;
		add_edge(N, ta);
		add_edge(N, tb);
		add_edge(N, tc);
		insert_node = quad_indices(N.a, N.b, N.c, -1);
		return true;
	}

	while (e != -1) {
		if (insert_helper(node[e], p)) 
			return true;
		e = nextEdge[e];
	}

	return false;
}

quad_indices DAG::insert(int p)
{
	if (insert_helper(ancestor_node, p)) 
		return insert_node;
	return quad_indices(-1, -1, -1, -1);
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


bool DAG::isInEdge(int eg_a, int eg_b, int p)
{
	return ps->sign_xmul(eg_a, eg_b, p) == 0;
}

int DAG::getAnotherPoint(int a, int b, int c, int u, int v)
{
	int p;

	if (a != u && a != v) p = a;
	else if (b != u && b != v) p = b;
	else p = c;

	return p;
}