#ifndef _HASH_TABLE_H
#define _HASH_TABLE_H

#include <vector>
#include <algorithm>

#define DEFAULT_PRIME 104729

struct bi
{
	int a, b;
	std::pair<int, int> idx;
	
	bi() { setVal(0, 0); }
	bi(int a, int b, int idx1 = -1, int idx2 = -1) { setVal(a, b, idx1, idx2); }
	void setVal(int a, int b, int idx1 = -1, int idx2 = -1) 
	{
		this->a = a;
		this->b = b;
		this->idx = std::make_pair(idx1, idx2);
	}
};

struct tri
{
	int a, b, c, idx;
	
	tri() { setVal(0, 0, 0, 0); }
	tri(int a, int b, int c, int idx) { setVal(a, b, c, idx); }
	void setVal(int a, int b, int c, int idx) 
	{
		this->a = a;
		this->b = b;
		this->c = c;
		this->idx = idx;
	}
};

class hashtable_bi
{
public:
	hashtable_bi(int prime = DEFAULT_PRIME);
	void init(int prime = DEFAULT_PRIME);
	void insert(int a, int b, int idx);
	void remove(int a, int b, int idx);
	void remove(int a, int b);
	std::pair<int, int> getIdx(int a, int b);
private:
	int eg, PRIME;
	std::vector<bi> node;
	std::vector<int> nextEdge;
	std::vector<int> table;
};

class hashtable_tri
{
public:
	hashtable_tri(int prime = DEFAULT_PRIME);
	void init(int prime = DEFAULT_PRIME);
	void insert(int a, int b, int c, int idx);
	void remove(int a, int b, int c);
	int getIdx(int a, int b, int c);
private:
	int eg, PRIME;
	std::vector<tri> node;
	std::vector<int> nextEdge;
	std::vector<int> table;
};

#endif

