#pragma once
#include <math.h>
#include <utility>
#include <iostream>
#include <algorithm>
#include <vector>

namespace std
{
	template <typename Num = long long>
	class GPointT {
	protected:
		Num x, y;

		pair<Num, Num> value() const { return { x, y }; }

	public:
		GPointT() {}
		GPointT(Num x, Num y) : x(x), y(y) {}
		GPointT(istream& in) { in >> (*this); }

		using GPoint = GPointT<Num>;

		friend istream& operator>>(istream& in, GPoint& G)
		{
			in >> G.x >> G.y;
			return in;
		}
		friend ostream& operator<<(ostream& out, GPoint& G)
		{
			return (out << G.x << G.y);
		}
#ifdef _CRT_SECURE_NO_WARNINGS
		friend void ReadPoint(GPoint& p) { scanf("%lf %lf", &p.x, &p.y); }
		friend void WritePoint(const GPoint& p) { printf("%lf %lf", p.x, p.y); }
#endif
		// VECTOR inline members
		friend GPoint operator+(const GPoint& p1, const GPoint& p2) { return { p1.x + p2.x, p1.y + p2.y }; }
		friend GPoint operator-(const GPoint& p1, const GPoint& p2) { return { p1.x - p2.x, p1.y - p2.y }; }
		friend GPoint operator*(const GPoint& p1, const Num& p2) { return { p1.x * p2, p1.y * p2 }; }

		friend Num operator&(const GPoint& p1, const GPoint& p2) { return p1.x * p2.x + p1.y * p2.y; }  // |p1|*|p2|*cos
		friend Num operator*(const GPoint& p1, const GPoint& p2) { return p1.x * p2.y - p1.y * p2.x; }  // |p1|*|p2|*sin

		friend Num dist2(const GPoint& p) { return p.x * p.x + p.y * p.y; }

		friend bool collinear(const GPoint& p1, const GPoint& p2, const GPoint& p3) { return ((p2 - p1) * (p3 - p1)) == 0; }

		// VECTOR EDITABLE functions
		GPoint operator+=(const GPoint& p) { x += p.x; y += p.y; return p; }
		GPoint operator-=(const GPoint& p) { x -= p.x; y -= p.y; return p; }
		Num operator*=(const Num& n) { x *= n; y *= n; return n; }

		// POINT operator<,>,<=,>=,==,!=
		friend bool operator<(const GPoint& p1, const GPoint& p2) { return p1.value() < p2.value(); }
		friend bool operator<=(const GPoint& p1, const GPoint& p2) { return p1.value() <= p2.value(); }
		friend bool operator==(const GPoint& p1, const GPoint& p2) { return p1.value() == p2.value(); }
		friend bool operator!=(const GPoint& p1, const GPoint& p2) { return p1.value() != p2.value(); }
		friend bool operator>=(const GPoint& p1, const GPoint& p2) { return p1.value() >= p2.value(); }
		friend bool operator>(const GPoint& p1, const GPoint& p2) { return p1.value() > p2.value(); }

		// DOUBLE arithmetic
		friend double dist(const GPoint& p) { return hypotl(p.x, p.y); }
		friend double angle(const GPoint& p1, const GPoint& p2) { return atan2((double)(p1 & p2) / dist(p1) / dist(p2), (double)(p1 * p2) / dist(p1) / dist(p2)); }
		friend double angle(const GPoint& a, const GPoint& o, const GPoint& b) { return angle(a - o, b - o); }

		// Access to protected class members
		Num& X() { return x; }
		Num& Y() { return y; }
		Num X() const { return x; }
		Num Y() const { return y; }

		friend struct polygon;
	};

	// INTEGER library !!!
	struct
	{
		enum CONVEX_STYLE
		{
			ANGLE_EQUAL_180,
			ANGLE_LESS_180
		} CONVEX = ANGLE_EQUAL_180;
	} _GOPTIONS_POLYGON;

	// INTEGER library !!!
	struct polygon : public vector<GPointT<long long>> {
		polygon() {}
		polygon(size_t n) : vector<GPointT<long long>>(n, { 0, 0 }) {}
		polygon(initializer_list<GPointT<long long>> lst) : vector<GPointT<long long>>(lst)
		{
			if (!empty()) push_back(front());
		}

		inline const GPointT<long long>& get(size_t ind) const
		{
			return this->operator[](ind);
		}

		long long area2() const
		{
			long long ans = 0;
			size_t len = size() - 1;
			for (size_t i = 0; i < len; i++)
			{
				ans += (get(i).x * get(i + 1).y - get(i + 1).x * get(i).y);
			}
			return llabs(ans);
		}

		double area() const { return (double)area2() / 2; }

		polygon convex()
		{
			GPointT<long long> pt = *min_element(begin(), end());
			sort(begin(), end(), [pt](const GPointT<long long>& a, const GPointT<long long>& b) {
				long long vp = (a - pt) * (b - pt);
				if (vp > 0) return true;
				if (vp < 0) return false;
				return dist2(pt - a) < dist2(pt - b);
				});
			polygon s;
			s.push_back(pt);
			for (size_t i = 0; i < size(); i++)
			{
				if (get(i) == pt) continue;
				while (s.size() > 1 &&
					((s[s.size() - 2] - s.back()) * (s.back() - get(i))) <= (_GOPTIONS_POLYGON.CONVEX == _GOPTIONS_POLYGON.ANGLE_EQUAL_180 ? -1 : 0))
					s.pop_back();
				s.push_back(get(i));
			}
			if (!s.empty()) s.push_back(s.front());
			return s;
		}

		polygon convex_nosort()
		{
			GPointT<long long> pt = *min_element(begin(), end());

			polygon s;
			s.push_back(pt);
			for (size_t i = 0; i < size(); i++)
			{
				if (get(i) == pt) continue;
				while (s.size() > 1 &&
					((s[s.size() - 2] - s.back()) * (s.back() - get(i))) <= (_GOPTIONS_POLYGON.CONVEX == _GOPTIONS_POLYGON.ANGLE_EQUAL_180 ? -1 : 0))
					s.pop_back();
				s.push_back(get(i));
			}
			if (!s.empty()) s.push_back(s.front());
			return s;
		}

		bool contain(GPointT<long long>& p) {
			if (size() == 2) return back() == p;
			if (size() == 3)
			{
				long long xn = min(get(0).x, get(1).x), xx = max(get(0).x, get(1).x);
				long long yn = min(get(0).y, get(1).y), yx = max(get(0).y, get(1).y);
				return collinear(get(0), get(1), p) && xn <= p.x && p.x <= xx &&
					yn <= p.y && p.y <= yx;
			}
			long long need = area2();
			long long cur = 0;
			for (size_t i = 0; i < size() - 1; i++)
			{
				cur += polygon({ p, get(i), get(i + 1) }).area2();
			}
			return cur == need;
		}

		value_type& getx(int val)
		{
			if (val < 0)
				val += polysize();
			if (val >= polysize())
				val -= polysize();
			if (val < 0 || val >= polysize())
				val = (val % polysize() + polysize()) % polysize();
			return (*this)[val];
		}

		void normalize()
		{
			if (size() == 1)
				push_back(front());
			else if (front() != back())
				push_back(front());
		}

		auto compare_function() const
		{
			GPointT<long long> pt = *min_element(begin(), end());
			return [pt](const GPointT<long long>& a, const GPointT<long long>& b) {
				long long vp = (a - pt) * (b - pt);
				if (vp > 0) return true;
				if (vp < 0) return false;
				return dist2(pt - a) < dist2(pt - b);
			};
		}

		inline int polysize()
		{
			if (size() > 1 && front() == back())
				return size() - 1;
			return size();
		}

		template <typename T, enable_if_t<is_arithmetic_v<T>, int> = 0>
		static T gcd(T a, T b)
		{
			while (b != 0)
			{
				a %= b;
				std::swap(a, b);
			}
			return a;
		}

		template <typename T, enable_if_t<is_arithmetic_v<T>, int> = 0>
		static void compose(T& a, T& b)
		{
			T g = gcd(a, b);
			a /= g;
			b /= g;
			if (a < 0 || (a == 0 && b < 0))
				a *= -1, b *= -1;
		}
	};

	template <typename Num>
	GPointT<Num> make_point(Num x, Num y)
	{
		return GPointT<Num>(x, y);
	}

	// Helper functions
	enum AngleInformation
	{
		ANGLE_FROM_0_TO_90 = 1,
		ANGLE_FROM_90_TO_180 = 2,
		ANGLE_FROM_180_TO_270 = 3,
		ANGLE_FROM_270_TO_360 = 4
	};

	// Angle AOB
	template <typename Num>
	AngleInformation AngleEx(const GPointT<Num>& a, const GPointT<Num>& o,
		const GPointT<Num>& b)
	{
		if ((a - o) * (b - o) >= 0)
			if (((a - o) & (b - o)) > 0)
				return ANGLE_FROM_0_TO_90;
			else
				return ANGLE_FROM_90_TO_180;
		else if (((a - o) & (b - o)) < 0)
			return ANGLE_FROM_180_TO_270;
		else
			return ANGLE_FROM_270_TO_360;
	}

	struct GOptions
	{
		long double EPS = 0;
	} _GOPTIONS_DOUBLE;

	//// Double geometry
	//template <typename T = double, enable_if_t<!is_integral_v<T>,int> = 0>
	//struct GCircleT
	//{
	//	using GPoint = GPointT<T>;
	//	using GCircle = GCircleT<T>;
	//	GPoint p;
	//	T R;

	//	operator const GPoint& () const
	//	{
	//		return p;
	//	}

	//	template <typename T2,enable_if_t<is_convertible_v<T2,T>,int> = 0>
	//	friend bool operator&(const GCircle& c, const GPointT<T2>& p) const
	//	{
	//		return fabs(dist2(c - GPoint(p.X(), p.Y())) - R * R) < _GOPTIONS_DOUBLE.EPS;
	//	}

	//	template <typename T2, enable_if_t<is_convertible_v<T2, T>, int> = 0>
	//	friend bool operator&(const GPointT<T2>& p, const GCircle& c) const
	//	{
	//		return fabs(dist2(c - GPoint(p.X(), p.Y())) - R * R) < _GOPTIONS_DOUBLE.EPS;
	//	}

	//	GCircleT() {}
	//	GCircleT(const GPoint& c, const T& r) :p(c), R(r) {}
	//	GCircleT(const GPoint& a, const GPoint& b, const GPoint& c)
	//	{

	//	}
	//};

	//template <typename T>
	//GCircleT<T> make_circle(const GPointT<T>& a, const GPointT<T>& b, const GPointT<T>& c)
	//{
	//	return GCircleT<T>(a, b, c);
	//}
}