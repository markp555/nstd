#pragma once
#include <stdlib.h>
namespace UnionFind
{
	const char* name = "UFDC";
	const char* clnm = "UnionFind";
	const char* func = "findSet; unionSet; setSize; numOfSet; isSameSet";

	const char* about = "";
}
#ifndef _UFDC_
#define _UFDC_
#include <vector>
namespace std
{
	class UnionFind
	{
	private:
		size_t sets;
		vector <size_t> rank, tmp, length;
	public:
		UnionFind(size_t n)
		{
			rank.assign(n, 0);
			tmp.assign(n, 0);
			length.assign(n, 1);
			sets = n;
			for (size_t i = 0; i < n; i++)
			{
				tmp[i] = i;
			}
		}

		size_t findSet(size_t elem)
		{
			if (tmp[elem] == elem)
				return elem;
			tmp[elem] = findSet(tmp[elem]);
			return tmp[elem];
		}

		size_t unionSet(size_t i, size_t j)
		{
			size_t x = findSet(i), y = findSet(j);
			if (x == y)
				return -1;
			sets--;
			if (rank[x] > rank[y])
			{
				tmp[y] = x;
				length[x] += length[y];
			}
			else
			{
				tmp[x] = y;
				length[y] += length[x];
				if (rank[x] == rank[y])
					rank[y]++;
			}
			return 0;
		}

		size_t setSize(size_t ind)
		{
			return length[ind];
		}

		size_t numOfSet()
		{
			return sets;
		}

		bool isSameSet(size_t i, size_t j)
		{
			return findSet(i) == findSet(j);
		}
	};
}
#endif