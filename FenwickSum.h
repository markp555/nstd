#pragma once
#include <utility>
#include <vector>
template <typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> _ = 0>
class SumFenwick
{
#ifdef _DEBUG
public:
#endif
	size_t n;
	std::vector <T> f;
public:
	template <typename T2, std::enable_if_t<std::is_convertible_v<T2,T>, int> x2 = 0>
	SumFenwick(const std::vector <T2>& a)
	{
		n = a.size();
		f.resize(n + 1);
		for (size_t i = 0; i < n; i++)
		{
			f[i + 1] = f[i] + (T)a[i];
		}

		for (int i = n; i > 0; i--)
		{
			f[i] -= f[i & (-i)];
		}
	}

	explicit SumFenwick(size_t sz)
	{
		n = sz;
		f.resize(sz + 1);
	}

	size_t size() const
	{
		return n;
	}

	T pref(size_t x) const
	{
		long long res = 0;
		while (x)
		{
			res += f[x];
			x -= (x & (~x + 1));
		}
		return res;
	}

	void add(size_t ind, T addval)
	{
		while (ind <= n)
		{
			f[ind] += addval;
			ind += (ind & (~ind + 1));
		}
	}
};