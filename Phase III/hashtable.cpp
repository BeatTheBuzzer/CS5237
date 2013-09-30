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
	int e;
	unsigned long long t;

	if (a > b) std::swap(a, b);

	t = ((((unsigned long long)a) << 31) | b) % PRIME;
	e = table[int(t)];
	while (e != -1) {
		if (node[e].a == a && node[e].b == b) {
			if (node[e].idx.first == -1) 
				node[e].idx.first = idx;
			else if (node[e].idx.first != idx && node[e].idx.second == -1) 
				node[e].idx.second = idx;
			return;
		}
		e = nextEdge[e];
	}
	nextEdge.push_back(table[int(t)]);
	node.push_back(bi(a, b, idx));
	table[t] = eg++;
}

void hashtable_bi::remove(int a, int b, int idx)
{
	int e;
	unsigned long long t;

	if (a > b) std::swap(a, b);
	t = ((((unsigned long long)a) << 31) | b) % PRIME;
	e = table[int(t)];;
	while (e != -1) {
		if (node[e].a == a && node[e].b == b) {
			if (node[e].idx.second == idx) 
				node[e].idx.second = -1;
			else if (node[e].idx.first == idx) {
				std::swap(node[e].idx.first, node[e].idx.second);
				node[e].idx.second = -1;
			}
			return;
		}
		e = nextEdge[e];
	}
}

void hashtable_bi::remove(int a, int b)
{
	int e;
	unsigned long long t;

	if (a > b) std::swap(a, b);
	t = ((((unsigned long long)a) << 31) | b) % PRIME;
	e = table[int(t)];;
	while (e != -1) {
		if (node[e].a == a && node[e].b == b) {
			node[e].idx = std::make_pair(-1, -1);
			return;
		}
		e = nextEdge[e];
	}
}

std::pair<int, int> hashtable_bi::getIdx(int a, int b)
{
	int e;
	unsigned long long t;
	std::pair<int, int> idx;

	if (a > b) std::swap(a, b);

	idx = std::make_pair(-1, -1);
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
	int e;
	unsigned long long t;

	if (a > b) std::swap(a, b);
	if (a > c) std::swap(a, c);
	if (b > c) std::swap(b, c);

	t = ((((unsigned long long)a) << 31) | b) % PRIME;
	t = ((t << 31) | c) % PRIME;
	e = table[int(t)];
	while (e != -1) {
		if (node[e].a == a && node[e].b == b && node[e].c == c) return;
		e = nextEdge[e];
	}
	nextEdge.push_back(table[int(t)]);
	node.push_back(tri(a, b, c, idx));
	table[t] = eg++;
}

void hashtable_tri::remove(int a, int b, int c)
{
	int e, ne;
	unsigned long long t;

	if (a > b) std::swap(a, b);
	if (a > c) std::swap(a, c);
	if (b > c) std::swap(b, c);

	t = ((((unsigned long long)a) << 31) | b) % PRIME;
	t = ((t << 31) | c) % PRIME;
	e = table[int(t)];
	if (e == -1) return;
	if (node[e].a == a && node[e].b == b && node[e].c == c) {
		table[int(t)] = nextEdge[e];
		return;
	}
	ne = nextEdge[e];
	while (ne != -1) {
		if (node[ne].a == a && node[ne].b == b && node[ne].c == c) {
			nextEdge[e] = nextEdge[ne];
			return;
		}
		e = ne;
		ne = nextEdge[e];
	}
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

