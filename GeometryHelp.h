#pragma once
#include <istream>
#ifndef _GDEC_
#error Must include GeometryDeclare before this file
#else
#if GLEVEL > 1
namespace std {
	istream& operator>>(istream& in, point& p) 
	{
		in >> p.x >> p.y;
		return in;
	}
	istream& operator>>(istream& in, circle& c)
	{
		in >> c.x >> c.y >> c.r;
		return in;
	}
	istream& operator>>(istream& in, line& l)
	{
		in >> l.a >> l.b >> l.c;
		point p, q;
		unline(l, p, q);
		l = line(p, q);
		return in;
	}
}
#endif
#endif