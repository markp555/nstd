#pragma once
#ifndef _GDEC_
#error Must include GeometryDeclare before GeometryFunctions
#else
#include <cmath>
#include <math.h>

namespace G
{
	using namespace std;

	bool operator<(const doubleval& d1, const doubleval& d2) { return (d1 - d2) < -EPS; }
	bool operator>(const doubleval& d1, const doubleval& d2) { return (d2 - d1) < -EPS; }
	bool operator==(const doubleval& d1, const doubleval& d2) { return fabs(d2 - d1) < EPS; }

	bool operator<(const point& p1, const point& p2) { return p1.getValue() < p2.getValue(); }
	bool operator==(const point& p1, const point& p2) { return p1.getValue() == p2.getValue(); }

	doubleval dist(const point& p1, const point& p2) { return hypot(p1.x - p2.x, p1.y - p2.y); }

	bool operator||(const line& l1, const line& l2) { return (l1.a == l2.a) && (l1.b == l2.b); }
	bool operator==(const line& l1, const line& l2) { return l1.getValue() == l2.getValue(); }
}

namespace GP
{
	doubleval operator*=(Vector& v, doubleval val) { v.x *= val; v.y *= val; return val; }
	Vector operator*(const Vector& v, doubleval val) { Vector tmp = v; tmp *= val; return tmp; }
	point operator|(const point& p, const Vector& v) { return { p.x + v.x,p.y + v.y,p.z }; }
	doubleval operator&(const Vector& v1, const Vector& v2) { return v1.x * v2.x + v1.y * v2.y; }
	doubleval operator*(const Vector& v1, const Vector& v2) { return v1.x * v2.y - v1.y * v2.x; }

	bool operator&&(const point& p, const line& l) { return static_cast<doubleval>(l.a * p.x + l.b * p.y + l.c) == static_cast<doubleval>(0); }

	point translate(const Vector& v, const point& p) { return p | v; }

	point rotate(const point& p, doubleval angle)
	{
		doubleval rad = angle * PI / 180;
		return point(p.x * cos(rad) - p.y * sin(rad), p.x * sin(rad) + p.y * cos(rad));
	}

	point intersect(const line& l1, const line& l2)
	{
		if (l1 || l2)
			throw exception("Lines are paralel/same");
		point ans;
		ans.x = (l2.b * l1.c - l1.b * l2.c) / (l2.a * l1.b - l1.a * l2.b);
		if (fabs(l1.b) > EPS)
			ans.y = -(l1.a * ans.x + l1.c);
		else
			ans.y = -(l2.a * ans.x + l2.c);
		return ans;
	}

	doubleval distToLine(const point& p, const point& a, const point& b, point* res = nullptr)
	{
		point c;
		Vector ap(a, p), ab(a, b);
		doubleval u = (ab & ap) / (ab & ab);
		c = a | (ab * u);
		if (res != nullptr)
			*res = c;
		return dist(c, p);
	}

	doubleval distToSegment(const point& p, const point& a, const point& b, point* res = nullptr)
	{
		Vector ap(a, p), ab(a, b);
		doubleval u = (ap & ab) / (ab & ab);
		if (u < static_cast<doubleval>(0.0))
		{
			if (res != nullptr)
				*res = a;
			return dist(p, a);
		}
		if (u > static_cast<doubleval>(1.0))
		{
			if (res != nullptr)
				*res = b;
			return dist(p, b);
		}
		return distToLine(p, a, b, res);
	}

	rad::rad(const deg& d) { this->__value__ = d * PI / 180; }
	deg::deg(const rad& r) { this->__value__ = r * 180 / PI; }

	// AOB
	rad angle(const point& a, const point& o, const point& b)
	{
		Vector oa(o, a), ob(o, b);
		return acos((oa & ob) / sqrt((oa & oa) * (ob & ob)));
	}

	// r is on p-q
	bool collinear(const point& p, const point& q, const point& r)
	{
		Vector pq(p, q), pr(p, r);
		return static_cast<doubleval>(fabs(pq * pr)) == static_cast<doubleval>(0);
	}

	enum INSIDE
	{
		INDOOR = 1,
		ON = 2,
		OUTDOOR = 3
	};

	INSIDE inside(const point& p, const circle& c)
	{
		doubleval dx = p.x - c.x;
		doubleval dy = p.y - c.y;
		doubleval val1 = dx * dx + dy * dy, val2 = c.r * c.r;
		if (val1 == val2)
			return INSIDE::ON;
		if (val1 < val2)
			return INSIDE::INDOOR;
		if (val1 > val2)
			return INSIDE::OUTDOOR;
	}

	// returns true if point r is on the left side of line pq
	bool ccw(const point& p, const point& q, const point& r) {
		return Vector(p, q) * Vector(p, r) > static_cast<doubleval>(0);
	}

	void unline(const line& l, point& p1, point& p2)
	{
		if (l.b == static_cast <doubleval>(0))
		{
			p1.x = (-l.c) / l.a;
			p1.y = 0;
			p2.x = (-l.c) / l.a;
			p2.y = 1;
			return;
		}
		doubleval t = (l.a * l.a) + (l.b * l.b);
		p1.x = -(l.a * l.c) / t;
		p1.y = -(l.b * l.c) / t;
		p2 = p1;
		p2.x += l.b;
		p2.y -= l.a;
	}
}

namespace GPP
{
	// length(t) = perimetr of t
	// area(t) = area of t

	doubleval length(const circle& c) { return 2 * PI * c.r; }
	doubleval length(const triangle& t) { return dist(t.a, t.b) + dist(t.b, t.c) + dist(t.c, t.a); }
	doubleval area(const circle& c) { return PI * c.r * c.r; }
	doubleval area(const triangle& t)
	{ 
		doubleval ab = dist(t.a, t.b), bc = dist(t.b, t.c), ac = dist(t.c, t.a), p = length(t) / static_cast<doubleval>(2);
		return sqrt(p) * sqrt(p - ab) * sqrt(p - bc) * sqrt(p - ac);
	}

	doubleval RInCircle(const triangle& t)
	{
		return 2 * area(t) / length(t);
	}

	/// <summary>
	/// In circle for triangle
	/// </summary>
	/// <param name="t">Triangle</param>
	/// <returns>Circle</returns>
	circle InCircle(const triangle& t)
	{
		circle tmp;
		tmp.r = RInCircle(t);
		line l1, l2;
		doubleval ratio = dist(t.a, t.b) / dist(t.a, t.c);
		point p = t.b | (Vector(t.b, t.c) * (ratio / (1 + ratio)));
		l1 = line(t.a, p);

		ratio = dist(t[2], t[1]) / dist(t[2], t[3]);
		point p2 = t[1] | (Vector(t[1], t[3]) * (ratio / (1 + ratio)));
		l2 = line(t[2], p2);

		point inter = intersect(l1, l2);
		tmp.x = inter.x;
		tmp.y = inter.y;
		return tmp;
	}

	doubleval ROutCircle(const triangle& t)
	{
		return dist(t[1], t[2]) * dist(t[2], t[3]) * dist(t[3], t[1]) / (4 * area(t));
	}

	/// <summary>
	/// Out circle for triangle
	/// </summary>
	/// <param name="t">Triangle</param>
	/// <returns>Circle</returns>
	circle OutCircle(const triangle& t)
	{
		circle ans;
		ans.r = ROutCircle(t);
		point p1 = t[1], p2 = t[2], p3 = t[3];
		doubleval a = p2.x - p1.x, b = p2.y - p1.y;
		doubleval c = p3.x - p1.x, d = p3.y - p1.y;
		doubleval e = a * (p1.x + p2.x) + b * (p1.y + p2.y);
		doubleval f = c * (p1.x + p3.x) + d * (p1.y + p3.y);
		doubleval g = 2.0 * (a * (p3.y - p2.y) - b * (p3.x - p2.x));
		ans.x = (d * e - b * f) / g;
		ans.y = (a * f - c * e) / g;
		return ans;
	}

	doubleval length(const polygon& p)
	{
		doubleval ans;
		size_t len = p.size() - 1;
		for (size_t i = 0; i < len; i++)
		{
			ans += dist(p[i], p[i + 1]);
		}
		return ans;
	}

	doubleval area(const polygon& p)
	{
		doubleval ans;
		size_t len = p.size() - 1;
		for (size_t i = 0; i < len; i++)
		{
			doubleval x1 = p[i].x, x2 = p[i + 1].x;
			doubleval y1 = p[i].y, y2 = p[i + 1].y;
			ans += (x1 * y2 - x2 * y1);
		}
		return fabs(ans) / static_cast<double>(2);
	}

	/// <summary>
	/// Is convex
	/// </summary>
	/// <param name="p">Polygon</param>
	/// <returns>True, if polygon is convex</returns>
	bool convex(const polygon& p)
	{
		size_t len = p.size() - 1;
		if (len < 3)
			return false;
		bool flag = ccw(p[0], p[1], p[2]);
		for (size_t i = 1; i < len; i++)
		{
			if (ccw(p[i], p[i + 1], p[(i + 2 == len + 1) ? 1 : i + 2]) != flag)
				return 0;
		}
		return true;
	}

	/// <summary>
	/// In polygon
	/// </summary>
	/// <param name="p">Point</param>
	/// <param name="g">Polygon</param>
	/// <returns>True, if Point in Polygon</returns>
	bool operator&&(const point& p, const polygon& g)
	{
		size_t len = g.size() - 1;
		doubleval ans;
		for (auto& i : g)
		{
			if (i == p)
				return 1;
		}
		for (size_t i = 0; i < len; i++)
		{
			if (ccw(p, g[i], g[i + 1]))
				ans += angle(g[i], p, g[i + 1]);
			else
				ans -= angle(g[i], p, g[i + 1]);
		}
		return static_cast<doubleval>(fabs(ans) - 2 * PI) == static_cast<doubleval>(0);
	}

	/// <summary>
	/// Translate triangle to polygon
	/// </summary>
	/// <param name="t">Triangle</param>
	/// <returns>Polygon</returns>
	polygon toPolygon(const triangle& t)
	{
		return { t[1],t[2],t[3],t[1] };
	}

	/// <summary>
	/// Cut polygon by line a->b
	/// </summary>
	/// <param name="p">Polygon</param>
	/// <param name="a">Point a</param>
	/// <param name="b">Point b</param>
	/// <returns>"Cutted" polygon</returns>
	polygon cutPolygon(const polygon& Q, const point& a, const point& b)
	{
		polygon P;
		size_t len = Q.size() - 1;
		for (size_t i = 0; i <= len; i++)
		{
			doubleval left1 = Vector(a, b) * Vector(a, Q[i]), left2 = 0;
			if (i != len)
				left2 = Vector(a, b) * Vector(a, Q[i]);
			if (left1 < static_cast<doubleval>(0))
				P.push_back(Q[i]);
			if (static_cast<doubleval>(left1 + left2) < static_cast<doubleval>(0))
				P.push_back(intersect(line(Q[i], Q[i + 1]), line(a, b)));
		}
		return P;
	}

	/// <summary>
	/// Return bisector of angle AOB
	/// </summary>
	/// <param name="a">A</param>
	/// <param name="o">O</param>
	/// <param name="b">B</param>
	/// <returns></returns>
	line bisector(const point& a, const point& o, const point& b)
	{
		doubleval ratio = dist(o, b) / dist(o, a);
		point p = b | (Vector(b, a) * (ratio / (1 + ratio)));
		return line(o, p);
	}

	/// <summary>
	/// Return tangents for circle and point
	/// </summary>
	/// <param name="c">circle</param>
	/// <param name="p">point</param>
	vector <point> tangent(const circle& c, const point& p)
	{
		return {};
	}

	/// <summary>
	/// Return intersect points line p-q with circle c
	/// </summary>
	/// <param name="c">c</param>
	/// <param name="p">p</param>
	/// <param name="q">q</param>
	/// <returns>intersect points</returns>
	vector <point> intersect(const circle& c, const point& p, const point& q)
	{
		point tmp;
		doubleval dst = distToLine(point(c.x, c.y), p, q, &tmp);
		if (dst > c.r)
			return {};
		else if (dst == c.r)
			return { tmp };
		else {
			line pq(p, q);
			Vector l(pq.a, pq.b), l2(-pq.b, pq.a);
			doubleval t = sqrt(c.r * c.r - dst * dst);
			l2 *= 1 / dist(point(0, 0), point(l2.x, l2.y));
			l2 *= t;
			l = { -l2.x, -l2.y };
			return { tmp | l, tmp | l2 };
		}
	}

	/// <summary>
	/// Return intersect of 2 circles. WARNING It has addtional information
	/// </summary>
	/// <param name="c1">Circle</param>
	/// <param name="c2">Circle</param>
	/// <returns>If size() == 3: [0].z = x; [1].z = h [2] = tmp; 2 intersect points</returns>
	vector <point> intersect(const circle& c1, const circle& c2) {
		if (c1.getValue() == c2.getValue())
			throw exception();
		point c1p(c1.x, c1.y), c2p(c2.x, c2.y);
		doubleval dst = dist(c1p, c2p);
		if (dst > static_cast<doubleval>(c1.r + c2.r))
			return {};
		if (dst < static_cast<doubleval>(fabs(c1.r - c2.r)))
			return {};
		if (dst == static_cast<doubleval>(c1.r + c2.r)) {
			Vector v(c1p, c2p);
			v *= 1 / (dist({ 0, 0 }, { v.x, v.y }));
			v *= c1.r;
			Vector v2 = v;
			v2 *= -1;
			point p1 = c1p | v, p2 = c1p | v2;
			if (inside(p1, c2) == INSIDE::ON)
				return { p1 };
			else
				return { p2 };
		}
		doubleval x = (c1.r * c1.r - c2.r * c2.r + dst * dst) / (2 * dst);
		doubleval h = sqrt(c1.r * c1.r - x * x);
		line tmp(c1p, c2p);
		Vector l(c1p, c2p);
		l *= 1 / (dist({ 0, 0 }, { l.x, l.y }));
		l *= x;
		if (inside(c1p | l, c2) == INSIDE::OUTDOOR)
			l *= -1;
		point m = c1p | l;
		Vector v1 = Vector(tmp.a, tmp.b) * (1 / dist({ 0,0 }, { tmp.a,tmp.b })), v2 = v1 * -1;
		v1 *= h;
		v2 *= h;
		point p1 = m | v1, p2 = m | v2;
		if (p1 == p2)
			return { p1 };
		p1.z = x;
		p2.z = h;
		return { p1, p2, m };
	}
}
#endif