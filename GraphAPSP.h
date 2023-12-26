#pragma once
#include <stdlib.h>
namespace GraphAPSP
{
	const char* name = "All ­Pairs Shortest Paths";
	const char* clnm = "";
	const char* func = "Floyd; getPath";

	const char* about = "";
}
#ifndef _APSP_
#define _APSP_
#include <vector>
#include "GraphSSSP.h"

namespace std
{
	using __tgrapg__ = vector<vector<long long>>;

	/// <summary>
	/// Floyd Algorithm
	/// </summary>
	/// <comment>O(N3)</comment>
	/// <comment>throw NegativeCycle();</comment>
	/// <param name="tg">Graph in Table View</param>
	/// <returns>prevlst</returns>
	vector <vector <size_t>> Floyd(__tgrapg__& tg)
	{
		size_t n = tg.size();
		vector <vector <size_t>> prev(n, vector <size_t> (n));
		for (size_t i = 0; i < n; i++)
		{
			for (size_t j = 0; j < n; j++)
			{
				prev[i][j] = i;
			}
		}
		for (size_t temp = 0; temp < n; temp++)
		{
			for (size_t i = 0; i < n; i++)
			{
				for (size_t j = 0; j < n; j++)
				{
					if (tg[i][temp] != INT64_MAX && tg[temp][j] != INT64_MAX &&
						tg[i][temp] + tg[temp][j] < tg[i][j])
					{
						tg[i][j] = tg[i][temp] + tg[temp][j];
						prev[i][j] = prev[temp][j];
					}
				}
			}
		}
		return prev;
	}

	void __getpath__(size_t i, size_t j, vector <size_t>& ans, const vector <vector <size_t>>& prevlst)
	{
		if (i != j)
			__getpath__(i, prevlst[i][j], ans, prevlst);
		ans.push_back(j);
	}

	/// <summary>
	/// Return Floyd Algorithm Path
	/// </summary>
	/// <param name="from">From vertex</param>
	/// <param name="to">To vertex</param>
	/// <param name="prevlst">Prevlist from Floyd Algorithm</param>
	/// <returns>Path; [0] = from, [last] = to</returns>
	vector <size_t> getPath(size_t from, size_t to, const vector <vector <size_t>>& prevlst)
	{
		vector <size_t> ans;
		__getpath__(from, to, ans, prevlst);
		return ans;
	}
}
#endif