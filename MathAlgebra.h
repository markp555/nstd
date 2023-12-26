#pragma once
#include <array>
#include <stdlib.h>

// Mark_Pr (c)

namespace std
{
	template<class _Number, size_t n, size_t m>
	struct matrix
	{
		_Number val[n][m];
		matrix()
		{
			for (size_t i = 0; i < n; i++)
			{
				for (size_t j = 0; j < m; j++)
				{
					val[i][j] = _Number();
				}
			}
		}
		matrix(initializer_list<initializer_list<_Number>> arr)
		{
			size_t tmp = 0;
			for (const initializer_list<_Number>* i = arr.begin(); i != arr.end(); i++, tmp++)
			{
				copy(i->begin(), i->end(), val[tmp]);
			}
		}

		_Number* operator[](int i)
		{
			return val[i];
		}

		const _Number* operator[](int i) const
		{
			return val[i];
		}

		/*operator const const _Number** () const
		{
			return val;
		}*/

		void operator-=(const matrix<_Number, n, m>& oth)
		{
			for (size_t i = 0; i < n; i++)
			{
				for (size_t j = 0; j < m; j++)
				{
					this->val[i][j] -= oth[i][j];
				}
			}
		}

		matrix<_Number, n, m> operator-(const matrix<_Number, n, m>& oth) const
		{
			matrix<_Number, n, m> tmp = *this;
			tmp -= oth;
			return tmp;
		}

		void operator+=(const matrix<_Number, n, m>& oth)
		{
			for (size_t i = 0; i < n; i++)
			{
				for (size_t j = 0; j < m; j++)
				{
					this->val[i][j] += oth[i][j];
				}
			}
		}

		matrix<_Number, n, m> operator+(const matrix<_Number, n, m>& oth) const
		{
			matrix<_Number, n, m> tmp = *this;
			tmp += oth;
			return tmp;
		}
	};

	template <class _Number, size_t n, size_t m, size_t k>
	matrix <_Number, n, k> operator*(const matrix<_Number, n, m>& m1,
		const matrix<_Number, m, k>& m2)
	{
		matrix <_Number, n, k> ans;
		for (size_t i = 0; i < n; i++)
		{
			for (size_t j = 0; j < k; j++)
			{
				for (size_t t = 0; t < k; t++)
				{
					ans[i][j] = ans[i][j] + m1[i][t] * m2[t][j];
				}
			}
		}
		return ans;
	}

	template <class T>
	T binpow(const T& obj, uint32_t pow)
	{
		if (pow == 1)
			return obj;
		if (pow % 2 == 0)
		{
			T tmp = binpow(obj, pow >> 1);
			return tmp * tmp;
		}
		return binpow(obj, pow - 1) * obj;
	}
}