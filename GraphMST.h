#pragma once
#ifndef _MST_
#define _MST_
#include <stdlib.h>
namespace GraphMST
{
	const char* name = "Minimum Spanning Tree";
	const char* clnm = "";
	const char* func = "MST";

	const char* about = "";
}
#include <vector>
#include <utility>
#include <algorithm>

#include "UnionFind.h"

namespace std
{
	/// <summary>
	/// { cost, { u, v } }
	/// </summary>
	using __edge = pair<int, pair<size_t, size_t>>;
	using __graph = vector<__edge>;

	/// <summary>
	/// Minimum Spanning Tree
	/// </summary>
	/// <param name="G">Input graph</param>
	/// <param name="n">Graph size</param>
	/// <returns> { mincost, edges}</returns>
	pair <long long, __graph> MST(size_t n, __graph G)
	{
		sort(G.begin(), G.end());
		long long cost = 0;
		__graph ans;
		UnionFind UF(n);
		for (auto& ed : G)
		{
			if (!UF.isSameSet(ed.second.second, ed.second.first))
			{
				UF.unionSet(ed.second.first,ed.second.second);
				ans.push_back(ed);
				cost += ed.first;
			}
		}
		return { cost, ans };
	}


}
#endif