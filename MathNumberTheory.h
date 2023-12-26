#pragma once
namespace MathNumberTheory
{
	const char* name = "Number Theory";
	const char* clnm = "";
	const char* func = "isPrime, sieve";

	const char* about = "";
}

#ifndef _NUMBERT_
#define _NUMBERT_
#include <bitset>
#include <vector>

namespace std
{
	namespace MathTool
	{
		bitset <10000010>		__sieveprime;
		long long				__sievesize = 1e7;
		long long				__x, __y;

		vector <long long> __sieve()
		{
			vector <long long> ans;

			__sieveprime.set();
			__sieveprime[0] = __sieveprime[1] = 0;

			for (size_t i = 2; i < __sievesize; i++)
			{
				if (!__sieveprime[i])
					continue;
				for (size_t j = i * i; j < __sievesize; j+=i)
				{
					__sieveprime[j] = 0;
				}
				ans.push_back(i);
			}

			return ans;
		}

		bool __isprime(long long n, vector <long long> primes)
		{
			if (n < __sievesize)
				return __sieveprime[n];
			vector <long long>::const_iterator ci = primes.cbegin();
			while ((*ci) * (*ci) <= n)
			{
				if (n % (*ci) == 0)
					return 0;
				ci++;
			}
			return 1;
		}

		vector <long long> __primefactors(long long n, vector <long long> primes)
		{
			vector <long long> ans;
			vector <long long>::const_iterator ci = primes.cbegin();
			while ((*ci) * (*ci) <= n)
			{
				while (n % (*ci) == 0)
				{
					ans.push_back(*ci);
					n /= (*ci);
				}
				ci++;
			}
			return ans;
		}

		long long __gcd(long long a, long long b)
		{
			if (b == 0)
				return a;
			return __gcd(b, a % b);
		}

		long long __lcm(long long a, long long b)
		{
			return a * (b / __gcd(a, b));
		}

		void __extgcd(long long a, long long b)
		{
			if (b == 0)
			{
				__x = 1;
				__y = 0;
				return;
			}
			long long x1 = __y;
			long long y1 = __x - (a / b) * __y;
			__x = x1;
			__y = y1;
		}
	}

	using __ll = long long;

	__ll gcd(__ll a, __ll b)
	{
		if (b == 0)
			return a;
		return gcd(b, a % b);
	}

	__ll lcm(__ll a, __ll b)
	{
		return a * (b / gcd(a, b));
	}
}
#endif