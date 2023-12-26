#pragma once
#ifndef GLEVEL
#error GLEVEL must be defined
#endif
#ifndef _GDEC_
#define _GDEC_
#include <tuple>
#include <stdlib.h>
#include <vector>

namespace G
{
	using namespace std;

	const double EPS = 1e-9;
	const double PI = 3.141592653589793;

	struct doubleval
	{
		double __value__;
		doubleval(double value) : __value__(value) {}
		doubleval() : __value__(0) {}

		operator double&()
		{
			return __value__;
		}

		operator const double& () const
		{
			return __value__;
		}
	};

	struct point
	{
		doubleval x, y, z;
		point() : x(0), y(0), z(0) {}
		point(double q1, double q2) :x(q1), y(q2), z(0) {}
		point(double q1, double q2, double q3) :x(q1), y(q2), z(q3) {}

		tuple <doubleval, doubleval, doubleval> getValue() const
		{
			return { x,y,z };
		}

		tuple <doubleval, doubleval, doubleval> getValue()
		{
			return { x,y,z };
		}
	};

	struct line
	{
		doubleval a, b, c;
		line() :a(0), b(0), c(0) {}
		line(double q1, double q2, double q3) :a(q1), b(q2), c(q3) {}

		tuple <doubleval, doubleval, doubleval> getValue() const
		{
			return { a,b,c };
		}

		tuple <doubleval, doubleval, doubleval> getValue()
		{
			return { a,b,c };
		}

		line(const point& p1, const point& p2)
		{
			if (p1.x == p2.x)
			{
				a = 1;
				b = 0;
				c = -p1.x;
			}
			else
			{
				a = -(((double)p1.y - (double)p2.y) / ((double)p1.x - (double)p2.x));
				b = 1;
				c = -(a * p1.x) - p1.y;
			}
		}
	};
}

namespace GP
{
	using namespace G;

	struct Vector
	{
		doubleval x, y;
		Vector() :x(0), y(0) {}
		Vector(double q1, double q2) :x(q1), y(q2) {}

		tuple <doubleval, doubleval> getValue()
		{
			return { x,y };
		}

		tuple <doubleval, doubleval> getValue() const
		{
			return { x,y };
		}

		Vector(const point& p1, const point& p2)
		{
			x = p2.x - p1.x;
			y = p2.y - p1.y;
		}
	};

	struct deg;
	struct rad : public doubleval { rad(const deg& d); rad(double d) :doubleval(d) {} };
	struct deg : public doubleval { deg(const rad& r); deg(double d) :doubleval(d) {} };

	struct circle
	{
		doubleval r, x, y;
		circle() :r(0), x(0), y(0) {}
		circle(double q1, double q2, double q3) :r(q1), x(q2), y(q3) {}

		tuple <doubleval, doubleval, doubleval> getValue() const
		{
			return { r,x,y };
		}

		tuple <doubleval, doubleval, doubleval> getValue()
		{
			return { r,x,y };
		}
	};

	struct triangle
	{
		point a, b, c;
		triangle() : a(), b(), c() {}
		triangle(const point& p1, const point& p2, const point& p3) :a(p1), b(p2), c(p3) {}

		tuple <point, point, point> getValue() const {
			return { a,b,c };
		}

		tuple <point, point, point> getValue() {
			return { a,b,c };
		}

		point operator[](short unsigned int ind) const
		{
			switch (ind)
			{
			case 1:
				return a;
			case 2:
				return b;
			case 3:
				return c;
			default:
				break;
			}
		}

		point& operator[](short unsigned int ind)
		{
			switch (ind)
			{
			case 1:
				return a;
			case 2:
				return b;
			case 3:
				return c;
			default:
				break;
			}
		}
	};
}

namespace GPP
{
	using namespace GP;

	using polygon = vector <point>;
}

namespace std
{
#if GLEVEL == 3
	using namespace GPP;
#elif GLEVEL == 2
	using namespace GP;
#elif GLEVEL == 1
	using namespace G;
#else
	// Geometry disabled
#endif
}

#endif

