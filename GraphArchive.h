#pragma once
#include <stdlib.h>
#ifndef _ARCHIVE_
#define _ARCHIVE_
#include <vector>
namespace std
{
	class Archive
	{
	private:
		vector <size_t> topsort;
		vector <bool> used;
		vector <vector <size_t>> graph;
		vector <vector <size_t>> ungraph;

		void dfs_topsort(size_t u)
		{
			used[u] = 1;
			for (auto& v : graph[u])
			{
				if (!used[v])
					dfs_topsort(v);
			}
			topsort.push_back(u);
		}

		void dfs_fill(size_t u, size_t num)
		{
			used[u] = 1;
			archive[u] = num;
			for (auto& v : ungraph[u]) {
				if (!used[v])
					dfs_fill(v, num);
			}
		}

	public:
		//  vector <size_t> unarchive;
		vector <size_t> archive;
		Archive(vector <vector <size_t>>& graph)
		{
			size_t length = graph.size();
			archive.resize(length);
			this->graph = graph;
			this->ungraph.resize(length);
			for (size_t vertex = 0; vertex < length; vertex++)
			{
				for (auto& v : graph[vertex])
				{
					this->ungraph[v].push_back(vertex);
				}
			}
			topsort.reserve(length);
			used.resize(length);
			for (size_t i = 0; i < length; i++)
			{
				if (used[i])
					continue;
				dfs_topsort(i);
			}

			used.assign(length, false);
			reverse(topsort.begin(), topsort.end());

			size_t num = 0;
			for (auto& elem : topsort) 
			{
				if (!used[elem]) 
				{
					dfs_fill(elem, num);
					num++;
				}
			}
		}
	};
}
#endif