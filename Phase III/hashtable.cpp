#include "hashtable.h"

hashtable_bi::hashtable_bi(int prime)
{
	init(prime);
}

void hashtable_bi::init(int prime)
{
	eg = 0;
	table.clear();
	table.assign(prime, -1);
	node.clear();
	nextEdge.clear();
	PRIME = prime;
}

void hashtable_bi::insert(int a, int b, int idx)
{
	unsigned long long t;

	if (a > b) std::swap(a, b);

	t = ((((long long)a) << 31) | b) % PRIME;
	nextEdge.push_back(table[int(t)]);
	node.push_back(bi(a, b, idx));
	table[t] = eg++;
}

int hashtable_bi::getIdx(int a, int b)
{
	int e, idx;
	unsigned long long t;

	if (a > b) std::swap(a, b);

	idx = -1;
	t = ((((unsigned long long)a) << 31) | b) % PRIME;
	e = table[int(t)];
	while (e != -1) {
		if (node[e].a == a && node[e].b == b) {
			idx = node[e].idx;
			break;
		}
		e = nextEdge[e];
	}

	return idx;
}

hashtable_tri::hashtable_tri(int prime)
{
	init(prime);
}

void hashtable_tri::init(int prime)
{
	eg = 0;
	table.clear();
	table.assign(prime, -1);
	node.clear();
	nextEdge.clear();
	PRIME = prime;
}

void hashtable_tri::insert(int a, int b, int c, int idx)
{
	unsigned long long t;

	if (a > b) std::swap(a, b);
	if (a > c) std::swap(a, c);
	if (b > c) std::swap(b, c);

	t = ((((unsigned long long)a) << 31) | b) % PRIME;
	t = ((t << 31) | c) % PRIME;
	nextEdge.push_back(table[int(t)]);
	node.push_back(tri(a, b, c, idx));
	table[t] = eg++;
}

int hashtable_tri::getIdx(int a, int b, int c)
{
	int e, idx;
	unsigned long long t;

	if (a > b) std::swap(a, b);
	if (a > c) std::swap(a, c);
	if (b > c) std::swap(b, c);

	idx = -1;
	t = ((((unsigned long long)a) << 31) | b) % PRIME;
	t = ((t << 31) | c) % PRIME;
	e = table[int(t)];
	while (e != -1) {
		if (node[e].a == a && node[e].b == b && node[e].c == c) {
			idx = node[e].idx;
			break;
		}
		e = nextEdge[e];
	}

	return idx;
}

