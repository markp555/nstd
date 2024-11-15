#pragma once
#include <stdlib.h>
namespace GraphSSSP
{
	const char* name = "SingleSource Shortest Paths";
	const char* clnm = "";
	const char* func = "Deijkstra; FloidBellman; BFS";

	const char* about = "";
}
#ifndef _SSSP_
#define _SSSP_
#include <vector>
#include <utility>
#include <queue>
namespace std
{
	namespace GraphHelp
	{
		const long long INF = 1e18;

		using __edge__ = pair<size_t, long long>;
		using __graph__ = vector<vector<__edge__>>;

		/// <summary>
		/// BFS Algorithm
		/// </summary>
		/// <comment>O(N + V)</comment>
		/// <param name="g">Graph</param>
		/// <param name="v">Start vertex</param>
		/// <returns>{ distlst, prevlst }</returns>
		pair <vector <long long>, vector<size_t>> BFS(const __graph__& g, size_t v)
		{
			size_t n = g.size();
			vector <long long> dist(n, INF);
			vector <size_t> prev(n, -1);

			queue <size_t> q;

			dist[v] = 0;
			q.push(v);
			while (!q.empty())
			{
				size_t u = q.front();
				q.pop();
				for (auto& t : g.at(u))
				{
					if (dist[t.first] != INF)
						continue;
					dist[t.first] = dist[u] + t.second;
					prev[t.first] = u;
					q.push(t.first);
				}
			}

			return { dist, prev };
		}

		/// <summary>
		/// Dijkstra Algorithm
		/// </summary>
		/// <comment> O((N + M) log N)</comment>
		/// <param name="g">Graph</param>
		/// <param name="s">Start vertex</param>
		/// <returns>{ distlst, prevlst }</returns>
		pair <vector <long long>, vector <size_t>> Dijkstra(const __graph__& g, size_t s)
		{
			size_t n = g.size();
			vector <long long> dist(n, INF);
			vector <size_t> prev(n, -1);
			priority_queue<pair<long long, size_t>, vector < pair<long long, size_t>>, greater< pair<long long, size_t>>> pq;

			dist[s] = 0;
			pq.push({ 0,s });
			while (!pq.empty())
			{
				long long d = pq.top().first;
				size_t v = pq.top().second;
				pq.pop();

				if (d > dist[v])
					continue;

				for (auto& u : g.at(v))
				{
					if (d + u.second < dist[u.first])
					{
						dist[u.first] = d + u.second;
						prev[u.first] = v;
						pq.push({ dist[u.first],u.first });
					}
				}
			}
			return { dist, prev };
		}

		struct NegativeCycle : public exception
		{
			NegativeCycle()
			{

			}
			virtual const char* what()
			{
				return "Algorithm find cycle with negative weight!\nAlgorithm FordBellman";
			}
		};

		/// <summary>
		/// Ford Bellman Algorithm
		/// </summary>
		/// <comment>O(NM)</comment>
		/// <comment>throw NegativeCycle();</comment>
		/// <param name="g">Graph</param>
		/// <param name="s">Start vertex</param>
		/// <returns>{ mindist, prevlst }</returns>
		pair <vector <long long>, vector <size_t>> FordBellman(const __graph__& g, size_t s)
		{
			size_t n = g.size();
			vector <long long> dist(n);
			vector <size_t> prevlst(n, -1);

			dist[s] = 0;

			for (size_t temp = 0; temp < n - 1; temp++)
			{
				for (size_t u = 0; u < n; u++)
				{
					for (auto& ed : g.at(u))
					{
						if (dist[u] != INF && dist[ed.first] > dist[u] + ed.second)
						{
							dist[ed.first] = dist[u] + dist[ed.second];
							prevlst[ed.first] = u;
						}
					}
				}
			}

			for (size_t u = 0; u < n; u++)
			{
				for (auto& ed : g.at(u))
				{
					if (dist[u] != INF && dist[ed.first] > dist[u] + ed.second)
					{
						throw NegativeCycle();
					}
				}
			}

			return { dist, prevlst };
		}
	}
}
#endif
