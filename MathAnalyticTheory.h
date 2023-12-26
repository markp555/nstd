#pragma once
namespace MathAnalyticTheory
{
	const char* name = "";
	const char* clnm = "";
	const char* func = "";

	const char* about = "";
}

#ifndef _ANALYTIC_
#define _ANALYTIC_
#include <stdlib.h>
#include <utility>

namespace std
{
	namespace MathTool
	{
		template <class Arg>
		using __func1 = Arg(*)(const Arg&);
		template <class Arg>
		using __func2 = Arg(*)(const Arg&, const Arg&);

		/// <summary>
		/// Find function Cycle
		/// </summary>
		/// <typeparam name="T">Argument type</typeparam>
		/// <param name="x0">Start value</param>
		/// <param name="f">Function</param>
		/// <returns>{ startval, length } </returns>
		template <typename T>
		pair <T, size_t> findFunctionCycle(const T& x0, __func1 <T> f)
		{
			T turtle = f(x0);
			T hare = f(f(x0));
			while (turtle != hare)
			{
				turtle = f(turtle);
				hare = f(f(hare));
			}

			T mu = x0;
			hare = x0;
			while (turtle != hare)
			{
				turtle = f(turtle);
				hare = f(hare);
				mu = f(mu);
			}

			size_t lambda = 1;
			hare = f(turtle);
			while (hare != turtle)
			{
				hare = f(hare);
				lambda++;
			}

			return { mu, lambda };
		}
	}
}

#endif