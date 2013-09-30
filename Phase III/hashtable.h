#ifndef _HASH_TABLE_H
#define _HASH_TABLE_H

#include <vector>
#include <algorithm>

#define DEFAULT_PRIME 104729

struct bi
{
	int a, b, idx;
	
	bi() { setVal(0, 0, 0); }
	bi(int a, int b, int idx) { setVal(a, b, idx); }
	void setVal(int a, int b, int idx) 
	{
		this->a = a;
		this->b = b;
		this->idx = idx;
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
	int getIdx(int a, int b);
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
	int getIdx(int a, int b, int c);
private:
	int eg, PRIME;
	std::vector<tri> node;
	std::vector<int> nextEdge;
	std::vector<int> table;
};

#endif

