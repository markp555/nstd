#pragma once
// Mark Privodnov (c)
#include <stdint.h>
#include <stdlib.h>
#include <utility>
#include <functional>
#ifdef _DEBUG
#include <xmmintrin.h>
#endif

namespace std
{
	namespace Math {
		uint64_t gcd(int64_t a, int64_t b, int64_t& x, int64_t& y) {
			if (b == 0) {
				x = 1;
				y = 0;
				return a;
			}
			int64_t x1, y1;
			uint64_t ans = gcd(b, a % b, x1, y1);
			x = y1;
			y = x1 - (a / b) * y1;
			return ans;
		}
		uint64_t multiple(uint64_t a, uint64_t b, uint64_t m) {
			return (a * b) % m;
		}
		uint64_t div(uint64_t a, uint64_t b, int m) {
			int64_t x, y;
			uint64_t ans = gcd(b, m, x, y);
			if (ans != 1)
				abort();
			x %= m;
			x += m;
			x %= m;
			return multiple(a, x, m);
		}
	}
	namespace MathTool
	{
//#define _STD_MATHTOOL_NUMBER_DECLAREFUNCTION_270623FEVGPBDTATWH(x) \
//template <typename T,enable_if_t<is_integral<T>::value,bool> = true> \
//number<MOD> operator x(const T& num) const\
//	{\
//		auto temp = *this;\
//		temp x= (number<MOD>)num;\
//		return temp;\
//	}

		template <const size_t MOD>
		struct number
		{
			static_assert(MOD > 0, "MOD must be > 0");
		private:
			uint64_t value;

		public:
			number(uint64_t val) :value(val% MOD) {}
			number() :value(0) {};

			void operator*=(number<MOD> nt)
			{
				value *= (uint64_t)nt;
				value %= MOD;
			}

			void operator/=(number<MOD> nt)
			{
				value = Math::div(value, nt, MOD);
			}

			void operator+=(number<MOD> nt)
			{
				value += (uint64_t)nt;
				if (value >= MOD)
					value -= MOD;
			}

			void operator-=(number<MOD> nt)
			{
				if (value < nt)
					value += (MOD - nt);
				else
					value -= nt;
			}

			number <MOD> operator++(int)
			{
				number <MOD> tmp = *this;
				(*this) += 1;
				return tmp;
			}

			number <MOD> operator--(int)
			{
				number <MOD> tmp = *this;
				(*this) -= 1;
				return tmp;
			}

			template <typename T, enable_if_t<is_integral<T>::value, bool> = true>
			number<MOD> operator+(const T& num) const
			{
				auto temp = *this;
				temp += (number<MOD>)num;
				return temp;
			}
			template <typename T, enable_if_t<is_integral<T>::value, bool> = true>
			number<MOD> operator-(const T& num) const
			{
				auto temp = *this;
				temp -= (number<MOD>)num;
				return temp;
			}
			template <typename T, enable_if_t<is_integral<T>::value, bool> = true>
			number<MOD> operator *(const T& num) const
			{
				auto temp = *this;
				temp *= (number<MOD>)num;
				return temp;
			}
			template <typename T, enable_if_t<is_integral<T>::value, bool> = true>
			number<MOD> operator /(const T& num) const
			{
				auto temp = *this;
				temp /= (number<MOD>)num;
				return temp;
			}

			template <typename T, enable_if_t<is_integral<T>::value, bool> = true>
			number<MOD> operator^ (const T& nt) const
			{
				if (nt == 0)
					return 1;
				if (nt == 1)
					return (*this);
				if (nt & 1)
				{
					return ((*this) ^ (nt - 1)) * (*this);
				}
				number<MOD>tmp = (*this) ^ (nt >> 1);
				return tmp * tmp;
			}

			template <typename T, enable_if_t<is_integral<T>::value, bool> = true>
			number<MOD> operator^= (const T& nt) const
			{
				(*this) = ((*this) ^ nt);
			}

			inline uint64_t& to64()
			{
				return value;
			}

			inline uint64_t to64() const
			{
				return value;
			}

			bool operator==(number<MOD>nt)const
			{
				return to64() == nt.to64();
			}

			template <typename T, enable_if_t<is_integral<T>::value, bool> = true>
			bool operator==(const T& num)
			{
				return to64() == num;
			}
#ifdef _MATHNUMBER_USE_CONVERTABLE_
			inline operator uint64_t() const
			{
				return value;
			}
#else
#pragma message("Define _MATHNUMBER_USE_CONVERTABLE_ to auto convert numbers");
		protected:
			inline operator uint64_t() const
			{
				return value;
			}
#endif
		};

		template <const size_t MOD>
		number<MOD> operator*(number<MOD> n1, number<MOD> n2)
		{
			n1 *= n2;
			return n1;
		}

		template <const size_t MOD>
		number<MOD> operator/(number<MOD> n1, number<MOD> n2)
		{
			n1 /= n2;
			return n1;
		}

		template <const size_t MOD>
		number<MOD> operator+(number<MOD> n1, number<MOD> n2)
		{
			n1 += n2;
			return n1;
		}

		template <const size_t MOD>
		number<MOD> operator-(number<MOD> n1, number<MOD> n2)
		{
			n1 -= n2;
			return n1;
		}

		struct fraction
		{
			uint64_t a;
			uint64_t b;

			void normalize()
			{
				int64_t unused[2];
				uint64_t g = Math::gcd(a, b, unused[0], unused[1]);
				a /= g;
				b /= g;
			}
#if 0
			int operator<=>(fraction y) const
			{
				fraction x = *this;
				if (x.a / x.b > y.a / y.b)
					return 1;
				if (x.a / x.b < y.a / y.b)
					return -1;
				x.a %= x.b;
				y.a %= y.b;
				if (x.a == 0 && y.a > 0)
					return -1;
				if (x.a == 0 && y.a == 0)
					return 0;
				if (x.a > 0 && y.a == 0)
					return 1;
				swap(x.a, x.b);
				swap(y.a, y.b);
				return -(x <=> y);
			}
#endif
		};

		int64_t analyze(int64_t val, int64_t d, function<bool(int64_t tmp)> f)
		{
			int64_t st = val - d, en = val + d + 1;
			while (st < en)
			{
				if (f(st))
					return st;
				st++;
			}
			return en;
		}
	}

	// a/b
	MathTool::fraction make_fraction(int64_t a, int64_t b)
	{
		return { (uint64_t)a,(uint64_t)b };
	}
}
